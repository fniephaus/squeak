/****************************************************************************
*   PROJECT: Mac window, memory, keyboard interface for netscape plugin logic
*   FILE:    sqMacNSPlugin.c
*   CONTENT: 
*
*   AUTHOR:  John Maloney, John McIntosh, and others.
*   ADDRESS: 
*   EMAIL:   johnmci@smalltalkconsulting.com
*   RCSID:   $Id: sqMacNSPlugin.c,v 1.18 2004/09/22 18:55:24 johnmci Exp $
*
*   NOTES: See change log below.
*	1/4/2002   JMM Some carbon cleanup
*
*****************************************************************************/
/********** Notes on Browser Plugin VM ************
How it Works:

The browser plugin VM allows Squeak to be run as a plug-in under
version 4.0 and later of either Netscape Navigator or Internet Explorer.
To use it, you must translate a version of interp.c with the browser plugin
hooks. 

Note for fullscreen support NPP_HandleEvent does not return control, it consumes 
events from waitnextevent. When fullscreen mode is turn off then control is 
returned to the browser.

Relationship of this file to sqMacWindow.c:

One can think of this file as specializing and extending sqMacWindow.c
for running within a browser. Certain methods in  sqMacWindow.c are
replaced by functions defined here. The originals are removed from
sqMacWindow.c by defining the BROWSERPLUGIN flag in that file when compiling.
Here is a list of the functions overridden:

	ioExit()
	ioScreenSize()
	ioSetFullScreen()

In addition, ioProcessEvents() becomes a noop and main() is completely
omitted when sqMacWindow.c is compiled for use in the browser plugin VM.

 Thing busted 2000 July 26th
 squeak quit restart has issues with change file open
 

June/July 2000 johnmci@smalltalkconsulting.com Reviewed code readded comments, added FullScreen Support
				fixed issues with Carbon, general code cleaning.

Sept 27th 2000 johnmci@smalltalkconsulting.com added logic to have more flexibility in memory size.
				Fix issue with volume ID, don't use 0
				Fix issue were IE lies about the frame size as it figures out the frame size in real time durning rendering
				Added check for file: in URL logic to disallow
				Fixed problem in NPP_URLNotify, must call notify complete logic
				Ensure NP_memFree is called for plugin arguments
				Add logic for URLPosting
Feb 8th 2001    johnmci@smalltalkconsulting.com Rewrote to use Apple multithreaded library and ported Squeak VM 3.0
Feb 26th 2001   JMM, allocate from application heap if we can do it, fix issue with null event not tracking button state.
Mar 23rd 2001   JMM, start interpreter when we get setwindow to fix netscape race condition
Apri 5th 2001   JMM 3.0.11, use checkForModifierKeys logic versus feb 26th eventAvail hack, watch button too.
April 27th 2001 JMM 3.0.14, posturl gets nil versus empty char for window name.
April 30th 2001 JMM 3.0.15, window is nil, if so make post work correctly, fix issue with post failure too
May 16th 2001	JMM 3.0.19, fix broken 3.0.17 for netscape, fixed set/restore port logic, obey plugin size hints 
May 27th 2001	JMM 3.0.22, fix bug in mailto: string size, added logic to deal with back behavior in IE to avoid crashes.
Jun 	 2001	JMM 3.0.23, added seteventhook logic to enable HW acceleration plugin to get notification of events
Aug      2001	JMM 3.0.24  rework of security logic, remove explicit call 
May	     2002   JMM 3.2.7b1 Ok lets see if the sucker will compile again
 3.7.0bx Nov 24th, 2003 JMM gCurrentVMEncoding
 July 31st, 2004 JMM 3.7.4b2 Add imageName and URL redirection. 
**********/

#include "sq.h"
#include "sqMacUIEvents.h"
#include "sqMacFileLogic.h"
#include "FilePlugin.h"
#include "sqMacTime.h"
#include "npapi.h"
#include "sqMacUIConstants.h"
#include "sqMacImageIO.h"
#include "sqMacEncoding.h"
#include "sqMacMemory.h"

#include <Events.h>
#include <Files.h>
#include <Gestalt.h>
#include <Notification.h>
#include <MacWindows.h>
#include <Movies.h>
#include <Folders.h>
#include <string.h>
#include <ctype.h>
#include <Threads.h>

#if defined ( __APPLE__ ) && defined ( __MACH__ )
    #include <pthread.h>
#endif

#if TARGET_API_MAC_CARBON
    #define EnableMenuItemCarbon(m1,v1)  EnableMenuItem(m1,v1);
    #define DisableMenuItemCarbon(m1,v1)  DisableMenuItem(m1,v1);
#else
    #define EnableMenuItemCarbon(m1,v1)  EnableItem(m1,v1);
    #define DisableMenuItemCarbon(m1,v1)  DisableItem(m1,v1);
#endif


/********** Compilation Options:
*
*	ENABLE_URL_FETCH
*	  Define this to compile primitives to fetch URL's via the browser.
*
*	  Warning: Fetching file URL's through the URL fetch mechanism allows
*	  potentially untrusted code to read files outside of the Squeak
*	  file system "sandbox", a serious breach of privacy. 
*	  There are few uses for use for the browser URL fetch
*	  mechanism, since you can always fetch URL's from remote servers
*	  using Squeak's own HTTPSocket.
*
**********/
#define IMAGE_NAME "SqueakPlugin.image"

/*** Exported Primitives ***/

#pragma export on
int primitivePluginBrowserReady(void);
#pragma export off

#ifdef ENABLE_URL_FETCH
#pragma export on
int primitivePluginDestroyRequest(void);
int primitivePluginRequestFileHandle(void);
int primitivePluginRequestState(void);
int primitivePluginRequestURL(void);
int primitivePluginRequestURLStream(void);
int primitivePluginPostURL(void);
#pragma export off
#endif

/* Constants */

#define MAX_STRING_LENGTH 255
#define MAX_ARG_STRING_LENGTH 100
#define STATUS_BAD_HANDLE -1
#define STATUS_IDLE 0
#define STATUS_IN_PROGRESS 1
#define STATUS_FAILED 2
#define STATUS_SUCCEEDED 3
#define STARTINGsqueakHeapMBytes 20*1024*1024

//#define PLUGIN_TRACE 1

#if PLUGIN_TRACE
//int printOnOSXPascal(unsigned char *msg);
//#define PLUGINDEBUGSTR(msg) printOnOSXPascal(msg);
#define PLUGINDEBUGSTR(msg)		DebugStr(msg)
#else
#define PLUGINDEBUGSTR
#endif


/*** Imported Variables ***/

extern unsigned char *memory;
extern WindowPtr stWindow;
extern int gButtonIsDown;
extern int getFullScreenFlag();
extern int setInterruptKeycode(int value);
extern int setFullScreenFlag(int value);

extern struct VirtualMachine *interpreterProxy;

extern int thisSession;  /* from sqFilePrims.c: */
#if defined ( __APPLE__ ) && defined ( __MACH__ )
pthread_mutex_t  gEventDrawLock,gEventNSAccept;
extern pthread_mutex_t gEventQueueLock;
extern TMTask    gTMTask;
#endif 

/*** Local Variables ***/

int			exitRequested	= false;
Rect    	gSavePortPortRect;
RgnHandle   gSavePortClipRgn;
CGrafPtr	gOldPort		= nil;
int			needsUpdate		= false;
NPWindow* 	netscapeWindow	= nil;
int			pluginArgCount	= 0;
char		*pluginArgName[MAX_ARG_STRING_LENGTH];
char		*pluginArgValue[MAX_ARG_STRING_LENGTH];
Boolean     ignoreFirstEvent=false,gIWasRunning=false;
int			squeakHeapMBytes = STARTINGsqueakHeapMBytes;  /* default heap size, override via the "memory" EMBED tag */
char		squeakPluginImageName[256] = IMAGE_NAME;
char		failureURL[1024] = "";
NPP			thisInstance	= nil;
WindowPtr gAFullscreenWindow = nil;
char        rememberMemoryString[128]="";
extern Boolean         gAllowAccessToFilePlease;


#define URL_REQUEST_COUNT 100

typedef struct {
	int id;  /* used to associate a request with its outcome */
	int status;
	int semaIndex;
	char fileName[MAX_STRING_LENGTH + 1];
	char *buffer;
} URLRequestRecord;

URLRequestRecord urlRequests[URL_REQUEST_COUNT];
int nextRequestID = 1;

/*** Functions Imported from sqMacWindow ***/

int ioSetFullScreenRestore();

/*** From VM ***/
int checkImageVersionFromstartingAt(sqImageFile f, squeakFileOffsetType imageOffset);
int getLongFromFileswap(sqImageFile f, int swapFlag);
extern int inputSemaphoreIndex;
int recordMouseEvent(EventRecord *theEvent, int theButtonState);
int MouseModifierState(EventRecord *theEvent);
extern eventMessageHook messageHook ;
extern eventMessageHook postMessageHook;
int recordKeyboardEvent(EventRecord *theEvent, int keyType);
extern Boolean  gThreadManager;
OSErr   createNewThread();
extern PixMapHandle	stPixMap;
extern ThreadID  gSqueakThread;
int checkForModifierKeys();

/*** Local Functions ***/

int  CaseInsensitiveMatch(char *s1, char *s2);
void EndDraw(void);
void ExitCleanup(void);
int  FindIdleURLRequest(void);
int  InitFilePaths(void);
int  InitFilePathsViaDomain(SInt16 domain);
void InitURLRequestTable(void);
int  IsPrefixedBy(char *s, char *prefix);
void OpenFileReadOnly(SQFile *f, char *fileName);
void ReadSqueakImage(void);
void StartDraw(void);
int  StringToInteger(char *s);
void URLRequestCompleted(int notifyData, const char* fileName);
int  URLRequestCreate(char *url, char *target, int semaIndex);
void URLRequestDestroy(int requestHandle);
void URLRequestFailed(int notifyData, int reason);
char * URLRequestFileName(int requestHandle);
int  URLRequestStatus(int requestHandle);
int parseMemorySize(int baseSize, char *src);
int AbortIfFileURL(char *url);
int URLPostCreate(char *url, char *buffer, char * window,int semaIndex);
NP_Port	  *getNP_Port(void);
void waitAFewMilliseconds(void);
void GetTempFSSpec(FSSpec *spec);


/*** Initialize/Shutdown ***/

/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_Initialize:
 * Provides global initialization for a plug-in, and returns an error value. 
 *
 * This function is called once when a plug-in is loaded, before the first instance
 * is created. Allocate any memory or resources shared by all
 * instances of your plug-in at this time. After the last instance has been deleted,
 * NPP_Shutdown will be called, where you can release any memory or
 * resources allocated by NPP_Initialize. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/
NPError NPP_Initialize(void) {
	exitRequested = false;
	needsUpdate = false;
	netscapeWindow = nil;
	pluginArgCount = 0;
	thisInstance = nil;
	InitURLRequestTable();
	return NPERR_NO_ERROR;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_GetJavaClass:
 * New in Netscape Navigator 3.0. 
 *
 * NPP_GetJavaClass is called during initialization to ask your plugin
 * what its associated Java class is. If you don't have one, just return
 * NULL. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/
jref
NPP_GetJavaClass(void)
{
	return (jref) NULL;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_Shutdown:
 * Provides global deinitialization for a plug-in. 
 * 
 * This function is called once after the last instance of your plug-in is destroyed.
 * Use this function to release any memory or resources shared across all
 * instances of your plug-in. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/

void NPP_Shutdown(void) {
	exitRequested = true;
#if defined ( __APPLE__ ) && defined ( __MACH__ )
    pthread_mutex_unlock(&gEventDrawLock);
#endif
	ExitCleanup();
}

/*** Instance Create/Destroy ***/
/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_New:
 * Creates a new instance of a plug-in and returns an error value. 
 * 
 * NPP_New creates a new instance of your plug-in with MIME type specified
 * by pluginType. The parameter mode is NP_EMBED if the instance was created
 * by an EMBED tag, or NP_FULL if the instance was created by a separate file.
 * You can allocate any instance-specific private data in instance->pdata at this
 * time. The NPP pointer is valid until the instance is destroyed. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/

NPError NPP_New(NPMIMEType pluginType, NPP instance, uint16 mode,
  int16 argc, char* argn[], char* argv[], NPSavedData* saved) {
	int i;

	/* only one Squeak instance can be active at a time */
	if (thisInstance != nil) return NPERR_GENERIC_ERROR;

	for (i = 0; i < MAX_ARG_STRING_LENGTH; i++) {
		pluginArgName[i] = pluginArgValue[i] = "";
	}

	/* record plugin arguments */
	pluginArgCount = argc;
	for (i = 0; (i < argc) && (i < MAX_ARG_STRING_LENGTH); i++) {
		pluginArgName[i] = (char *) NPN_MemAlloc(strlen(argn[i]) + 1);
		strcpy(pluginArgName[i], argn[i]);
		pluginArgValue[i] = (char *) NPN_MemAlloc(strlen(argv[i]) + 1);
		strcpy(pluginArgValue[i], argv[i]);
		if (CaseInsensitiveMatch(pluginArgName[i], "imagename")) {
			strcpy(squeakPluginImageName,pluginArgValue[i]);
		}
		if (CaseInsensitiveMatch(pluginArgName[i], "failureurl")) {
			strcpy(failureURL,pluginArgValue[i]);
		}
		if (CaseInsensitiveMatch(pluginArgName[i], "memory")) {
			strcpy(rememberMemoryString,pluginArgValue[i]);
		}
	}

	memory = nil;
	thisInstance = instance;
	ReadSqueakImage();
	if (!memory) return NPERR_GENERIC_ERROR;
	gSavePortClipRgn = NewRgn();
	
	
	return NPERR_NO_ERROR;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_Destroy:
 * Deletes a specific instance of a plug-in and returns an error value. 

 * NPP_Destroy is called when a plug-in instance is deleted, typically because the
 * user has left the page containing the instance, closed the window, or quit the
 * application. You should delete any private instance-specific information stored
 * in instance->pdata. If the instance being deleted is the last instance created
 * by your plug-in, NPP_Shutdown will subsequently be called, where you can
 * delete any data allocated in NPP_Initialize to be shared by all your plug-in's
 * instances. Note that you should not perform any graphics operations in
 * NPP_Destroy as the instance's window is no longer guaranteed to be valid. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/
NPError NPP_Destroy(NPP instance, NPSavedData** save) {
	
	exitRequested = true;
#if defined ( __APPLE__ ) && defined ( __MACH__ )
    pthread_mutex_unlock(&gEventDrawLock);
#endif
	ExitCleanup();
	return NPERR_NO_ERROR;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_SetWindow:
 * Sets the window in which a plug-in draws, and returns an error value. 
 * 
 * NPP_SetWindow informs the plug-in instance specified by instance of the
 * the window denoted by window in which the instance draws. This NPWindow
 * pointer is valid for the life of the instance, or until NPP_SetWindow is called
 * again with a different value. Subsequent calls to NPP_SetWindow for a given
 * instance typically indicate that the window has been resized. If either window
 * or window->window are NULL, the plug-in must not perform any additional
 * graphics operations on the window and should free any resources associated
 * with the window. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/
NPError NPP_SetWindow(NPP instance, NPWindow* window) {
	NP_Port* port;
        OSErr   err;
	
	if (window == NULL || window->window == NULL)
		return NPERR_NO_ERROR;
                
	if (window->width == 0 && window->height == 0) {
		if (gIWasRunning) {
			gIWasRunning = false;
			NPP_Destroy(instance, null);
		}
		return NPERR_NO_ERROR;
	}

            
	netscapeWindow = window;
	port = (NP_Port *) netscapeWindow->window;
	stWindow = GetWindowFromPort(port->port);
	needsUpdate	= true;

	if (gIWasRunning)
            return NPERR_NO_ERROR;
        gIWasRunning = true;

	ioLoadFunctionFrom(NULL, "DropPlugin");

        gThreadManager = true;
#if defined ( __APPLE__ ) && defined ( __MACH__ )
    pthread_mutex_lock(&gEventDrawLock);
#endif
        err = createNewThread();
        if (err != noErr)
            gThreadManager = false;

	if (gSqueakThread != kNoThreadID)
	    SetThreadState (gSqueakThread,kReadyThreadState,kNoThreadID); //OK start Squeak (os-9)

	return NPERR_NO_ERROR;
}

/*** Streaming ***/

/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_NewStream:
 * Notifies an instance of a new data stream and returns an error value. 
 * 
 * NPP_NewStream notifies the instance denoted by instance of the creation of
 * a new stream specifed by stream. The NPStream* pointer is valid until the
 * stream is destroyed. The MIME type of the stream is provided by the
 * parameter type. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/
NPError NPP_NewStream(
  NPP instance, NPMIMEType type,
  NPStream *stream, NPBool seekable, uint16 *stype) {
  /* Call to load the initial URL and to handle explicit URL fetch requests. */

	*stype = NP_ASFILEONLY;  /* ask Netscape to cache file and call NPP_StreamAsFile */
	return NPERR_NO_ERROR;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_DestroyStream:
 * Indicates the closure and deletion of a stream, and returns an error value. 
 * 
 * The NPP_DestroyStream function is called when the stream identified by
 * stream for the plug-in instance denoted by instance will be destroyed. You
 * should delete any private data allocated in stream->pdata at this time. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/
NPError NPP_DestroyStream(NPP instance, NPStream *stream, NPError reason) {
	return NPERR_NO_ERROR;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_StreamAsFile:
 * Provides a local file name for the data from a stream. 
 * 
 * NPP_StreamAsFile provides the instance with a full path to a local file,
 * identified by fname, for the stream specified by stream. NPP_StreamAsFile is
 * called as a result of the plug-in requesting mode NP_ASFILEONLY or
 * NP_ASFILE in a previous call to NPP_NewStream. If an error occurs while
 * retrieving the data or writing the file, fname may be NULL. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/
void NPP_StreamAsFile(NPP instance, NPStream *stream, const char* fname) {
	int handle = ((int) stream->notifyData) & 0xFF;
	
	if (fname != null) {
		if ((handle < 0) || (handle >= URL_REQUEST_COUNT)) {
			return;
		}
		if (urlRequests[handle].id == (((int) stream->notifyData) >> 8)) {
			if (fname != null) 
			    strncpy(urlRequests[handle].fileName, fname, MAX_STRING_LENGTH);
		}
	} else {
		urlRequests[handle].status = STATUS_FAILED;
	}
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_WriteReady:
 * Returns the maximum number of bytes that an instance is prepared to accept
 * from the stream. 
 * 
 * NPP_WriteReady determines the maximum number of bytes that the
 * instance will consume from the stream in a subsequent call NPP_Write. This
 * function allows Netscape to only send as much data to the instance as the
 * instance is capable of handling at a time, allowing more efficient use of
 * resources within both Netscape and the plug-in. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/
int32 NPP_WriteReady(NPP instance, NPStream *stream) {
  /* not used, because we use ASFILEONLY mode */

	return 100000;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_Write:
 * Delivers data from a stream and returns the number of bytes written. 
 * 
 * NPP_Write is called after a call to NPP_NewStream in which the plug-in
 * requested a normal-mode stream, in which the data in the stream is delivered
 * progressively over a series of calls to NPP_WriteReady and NPP_Write. The
 * function delivers a buffer buf of len bytes of data from the stream identified
 * by stream to the instance. The parameter offset is the logical position of
 * buf from the beginning of the data in the stream. 
 * 
 * The function returns the number of bytes written (consumed by the instance).
 * A negative return value causes an error on the stream, which will
 * subsequently be destroyed via a call to NPP_DestroyStream. 
 * 
 * Note that a plug-in must consume at least as many bytes as it indicated in the
 * preceeding NPP_WriteReady call. All data consumed must be either processed
 * immediately or copied to memory allocated by the plug-in: the buf parameter
 * is not persistent. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/
int32 NPP_Write(NPP instance, NPStream *stream, int32 offset, int32 len, void *buffer) {
  /* not used, because we use ASFILEONLY mode */

	return len;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_URLNotify:
 * Notifies the instance of the completion of a URL request. 
 * 
 * NPP_URLNotify is called when Netscape completes a NPN_GetURLNotify or
 * NPN_PostURLNotify request, to inform the plug-in that the request,
 * identified by url, has completed for the reason specified by reason. The most
 * common reason code is NPRES_DONE, indicating simply that the request
 * completed normally. Other possible reason codes are NPRES_USER_BREAK,
 * indicating that the request was halted due to a user action (for example,
 * clicking the "Stop" button), and NPRES_NETWORK_ERR, indicating that the
 * request could not be completed (for example, because the URL could not be
 * found). The complete list of reason codes is found in npapi.h. 
 * 
 * The parameter notifyData is the same plug-in-private value passed as an
 * argument to the corresponding NPN_GetURLNotify or NPN_PostURLNotify
 * call, and can be used by your plug-in to uniquely identify the request. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/
void NPP_URLNotify(NPP instance, const char* url, NPReason reason, void* notifyData) {
	/*if (reason != NPRES_DONE) 
		URLRequestFailed((int) notifyData, reason);
	else*/
		
		URLRequestCompleted((int) notifyData, null);

}

/*** Printing ***/

void NPP_Print(NPP instance, NPPrint* printInfo) {
	/* printing is not supported */
}

/*** Event Handling ***/

/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_HandleEvent:
 * Mac-only, but stub must be present for Windows
 * Delivers a platform-specific event to the instance. 
 * 
 * On the Macintosh, event is a pointer to a standard Macintosh EventRecord.
 * All standard event types are passed to the instance as appropriate. In general,
 * return TRUE if you handle the event and FALSE if you ignore the event. 
 * Note special logic for full screen support
 +++++++++++++++++++++++++++++++++++++++++++++++++*/

int16 NPP_HandleEvent(NPP instance, void *rawEvent) {
	EventRecord *eventPtr = (EventRecord*) rawEvent;
	EventRecord	theEvent;
	int				ok;
    Boolean windowActive=true;
    GrafPtr         rememberFrontWindow=null;
    Boolean			rememberWindowOnce=true;
    
#if defined ( __APPLE__ ) && defined ( __MACH__ )
	pthread_mutex_lock(&gEventNSAccept);
    pthread_mutex_unlock(&gEventDrawLock);
#endif

    if (rememberWindowOnce) { //Remember who the front window is
        rememberWindowOnce = false;
        rememberFrontWindow = (GrafPtr) FrontWindow();
    }

	if (gThreadManager)
SqueakYieldToAnyThread(); //Give some time up, needed for Netscape
 
	do {
	
	    if (rememberFrontWindow == (GrafPtr)  FrontWindow())
			windowActive = true;
		else 
			windowActive = false;

    	if (exitRequested) {
    		exitRequested = false;
    		ExitCleanup();
#if defined ( __APPLE__ ) && defined ( __MACH__ )
			pthread_mutex_unlock(&gEventNSAccept);
#endif
    		return false;
    	}

    	if ((thisInstance == nil) || (eventPtr == NULL)) {
    		/* no instance or no event; do nothing */
#if defined ( __APPLE__ ) && defined ( __MACH__ )
			pthread_mutex_unlock(&gEventNSAccept);
#endif
    		return false;
    	}

		if(!((messageHook) && (messageHook(eventPtr)))) {
	    	switch (eventPtr->what) {
	    		case mouseDown:
					if (!windowActive) 
					    break;
	                gButtonIsDown = true;				    
					if(inputSemaphoreIndex) {
			    		StartDraw();
#if I_AM_CARBON_EVENT
        pthread_mutex_lock(&gEventQueueLock);
#endif                                        
						recordMouseEvent(eventPtr,MouseModifierState(eventPtr));
#if I_AM_CARBON_EVENT
        pthread_mutex_unlock(&gEventQueueLock);
#endif
						EndDraw();
						break;
					}
					recordMouseDown(eventPtr);
	    		break;

				case mouseUp:
					if (!windowActive) 
					    break;
					gButtonIsDown = false;
					if(inputSemaphoreIndex) {
			    		StartDraw();
#if I_AM_CARBON_EVENT
        pthread_mutex_lock(&gEventQueueLock);
#endif                                        
						recordMouseEvent(eventPtr,MouseModifierState(eventPtr));
#if I_AM_CARBON_EVENT
        pthread_mutex_unlock(&gEventQueueLock);
#endif                                        
			    		EndDraw();
						break;
					}
					recordModifierButtons(eventPtr);
				break;

	    		case keyDown:
	    		case autoKey:
	  				if(inputSemaphoreIndex) {
#if I_AM_CARBON_EVENT
        pthread_mutex_lock(&gEventQueueLock);
#endif                                        
						recordKeyboardEvent(eventPtr,EventKeyDown);
#if I_AM_CARBON_EVENT
        pthread_mutex_unlock(&gEventQueueLock);
#endif                                        
						break;
					}
					recordModifierButtons(eventPtr);
	  			    recordKeystroke(eventPtr);
	    		break;

				case keyUp:
					if(inputSemaphoreIndex) {
#if I_AM_CARBON_EVENT
        pthread_mutex_lock(&gEventQueueLock);
#endif                                        
						recordKeyboardEvent(eventPtr,EventKeyUp);
#if I_AM_CARBON_EVENT
        pthread_mutex_unlock(&gEventQueueLock);
#endif                                        
					}
				break;

	    		case updateEvt:
	    			needsUpdate = true;
	    		break;
	    		
	    		case nullEvent :
					{
	    				if(inputSemaphoreIndex && windowActive) {
	    					eventPtr->modifiers = checkForModifierKeys();
	    		    		StartDraw();
#if I_AM_CARBON_EVENT
        pthread_mutex_lock(&gEventQueueLock);
#endif                                        
	     					recordMouseEvent(eventPtr,MouseModifierState(eventPtr));     
#if I_AM_CARBON_EVENT
        pthread_mutex_unlock(&gEventQueueLock);
#endif                                        
					
	     					EndDraw();
	    		 		}
					}
	    		break;    		
				case activateEvt:
#if defined ( __APPLE__ ) && defined ( __MACH__ )
				{
				/* serious hack to show safari after hiding, 
				issue with safari 1.1.1 not seeing activate when full screen terminates */
				
				ProcessSerialNumber psn = { 0, kCurrentProcess }; 
				OSStatus err;
				if (!IsProcessVisible(&psn)) {
					err = ShowHideProcess (&psn,true);
					SetFrontProcess(&psn);
				}
				}
				pthread_mutex_lock(&gEventDrawLock);
				pthread_mutex_unlock(&gEventNSAccept);
#endif
				return false;
				break;
	    	}
		}
    	if (needsUpdate && (netscapeWindow != nil) && (memory)) {
     		needsUpdate = false;
    		if (getFullScreenFlag()) {
    		    //BeginUpdate((WindowPtr) eventPtr->message);
				BeginUpdate(FrontWindow());
				fullDisplayUpdate();  /* ask VM to call ioShowDisplay */
				EndUpdate(FrontWindow());
    		    //EndUpdate((WindowPtr) eventPtr->message);
#if defined ( __APPLE__ ) && defined ( __MACH__ )
				pthread_mutex_lock(&gEventDrawLock);
				pthread_mutex_unlock(&gEventNSAccept);
#endif
				return true;
     		}
			else {
             fullDisplayUpdate();  /* ask VM to call ioShowDisplay */
 			}
#if defined ( __APPLE__ ) && defined ( __MACH__ )
			if (!getFullScreenFlag()) {
				ProcessSerialNumber psn = { 0, kCurrentProcess }; 
				OSStatus err;
				err = ShowHideProcess (&psn,true);
				waitAFewMilliseconds();

#if defined ( __APPLE__ ) && defined ( __MACH__ )
				pthread_mutex_lock(&gEventDrawLock);
				pthread_mutex_unlock(&gEventNSAccept);
#endif
				return false;
			}
#endif
    	}
		
		if(postMessageHook) postMessageHook(eventPtr);
    	
    	/* JMM HUH if (ignoreFirstEvent  &&  getFullScreenFlag()) {
    	    ignoreFirstEvent = false;
#if defined ( __APPLE__ ) && defined ( __MACH__ )
            pthread_mutex_lock(&gEventDrawLock);
			pthread_mutex_unlock(&gEventNSAccept);
#endif
    	    return false;
    	} */
	if (getFullScreenFlag()) {
#if defined ( __APPLE__ ) && defined ( __MACH__ )
            pthread_mutex_lock(&gEventDrawLock);
			pthread_mutex_unlock(&gEventNSAccept);
#endif
     	    ok = WaitNextEvent(everyEvent, &theEvent,1,null);
#if defined ( __APPLE__ ) && defined ( __MACH__ )
            pthread_mutex_unlock(&gEventDrawLock);
			pthread_mutex_lock(&gEventNSAccept);
#endif
            eventPtr = &theEvent;
    		SqueakYieldToAnyThread();
    	}
	} while (getFullScreenFlag());
                    
        
#if defined ( __APPLE__ ) && defined ( __MACH__ )
    pthread_mutex_lock(&gEventDrawLock);
	pthread_mutex_unlock(&gEventNSAccept);
#endif
	return true;
}

/*** Drawing ***/

void EndDraw(void) {
        if (exitRequested) 
            return;
	SetOrigin(gSavePortPortRect.left, gSavePortPortRect.top);
	SetClip(gSavePortClipRgn);
	SetPort((GrafPtr) gOldPort);
#if defined ( __APPLE__ ) && defined ( __MACH__ )
	UnlockPortBits((GrafPtr)GetWindowPort(stWindow));
    pthread_mutex_unlock(&gEventDrawLock);
#endif 
}

void StartDraw(void) {
	NP_Port* port;
	Rect clipRect;
	
#if defined ( __APPLE__ ) && defined ( __MACH__ )
    pthread_mutex_lock(&gEventDrawLock);
#endif
        if (exitRequested) {
        #if defined ( __APPLE__ ) && defined ( __MACH__ )
            pthread_mutex_unlock(&gEventDrawLock);
        #endif 
            return;
        }
	port = (NP_Port *) netscapeWindow->window;

	/* save old graphics port and switch to ours */
	GetPort((GrafPtr *) &gOldPort);
	stWindow = (WindowPtr) port->port;
#if defined ( __APPLE__ ) && defined ( __MACH__ )
	LockPortBits((GrafPtr)GetWindowPort(stWindow));
#endif
	SetPort((GrafPtr) port->port);

	/* save old drawing environment */
	GetPortBounds(port->port,&gSavePortPortRect);
	GetClip(gSavePortClipRgn);

	/* setup our drawing environment */
	SetOrigin(port->portx, port->porty);
	clipRect.top    = netscapeWindow->clipRect.top    + port->porty;
	clipRect.left   = netscapeWindow->clipRect.left   + port->portx;
	clipRect.bottom = netscapeWindow->clipRect.bottom + port->porty;
	clipRect.right  = netscapeWindow->clipRect.right  + port->portx;
	if (clipRect.top == 0 && clipRect.left ==0 && clipRect.bottom==0 && clipRect.right==0) {
		// Not sure what to do IE is lying... this gets the full screen, not a table cell GetPortBounds(GetWindowPort(stWindow),&clipRect);
	}
	ClipRect(&clipRect);
	BackColor(whiteColor);  /* needed to avoid funny colors */
}


int ioShowDisplay(
	int dispBitsIndex, int width, int height, int depth,
	int affectedL, int affectedR, int affectedT, int affectedB) {

	Rect		dstRect = { 0, 0, 0, 0 };
	Rect		srcRect = { 0, 0, 0, 0 };
	static RgnHandle	maskRect = nil;
    
	if (stWindow == nil || exitRequested) {
		return 0;
	}
	
	StartDraw();
        if (exitRequested) 
            return 0;
    
	dstRect.right	= width;
	dstRect.bottom	= height;
	srcRect.right	= width;
	srcRect.bottom	= height;

	(*stPixMap)->baseAddr = (void *) dispBitsIndex;
	/* Note: top three bits of rowBytes indicate this is a PixMap, not a BitMap */
	(*stPixMap)->rowBytes = (((((width * depth) + 31) / 32) * 4) & 0x1FFF) | 0x8000;
	(*stPixMap)->bounds = srcRect;
	(*stPixMap)->pixelSize = depth;

    if (depth<=8) { /*Duane Maxwell <dmaxwell@exobox.com> fix cmpSize Sept 18,2000 */
    	(*stPixMap)->cmpSize = depth;
    	(*stPixMap)->cmpCount = 1;
    } else if (depth==16) {
    	(*stPixMap)->cmpSize = 5;
    	(*stPixMap)->cmpCount = 3;
    } else if (depth==32) {
    	(*stPixMap)->cmpSize = 8;
    	(*stPixMap)->cmpCount = 3;
    }

	/* create a mask region so that only the affected rectangle is copied */
	if (maskRect == nil) 
	maskRect = NewRgn();
	SetRectRgn(maskRect, affectedL, affectedT, affectedR, affectedB);

	CopyBits((BitMap *) *stPixMap, GetPortBitMapForCopyBits(GetWindowPort(stWindow)), &srcRect, &dstRect, srcCopy, maskRect);
#if defined ( __APPLE__ ) && defined ( __MACH__ )
	QDFlushPortBuffer ((CGrafPtr)stWindow, maskRect);
#endif
	EndDraw();
	return 0;
}

NP_Port * getNP_Port(void) {
    return (NP_Port *) netscapeWindow->window;
}


/*** Image File Reading ***/

void ReadSqueakImage(void) {
	sqImageFile f;
	char msg[500];
    int swapBytes;
    int dataSize;
    int headerStart;
    int headerSize;
    
	plugInInit(squeakPluginImageName);
	InitFilePaths();

	/* read the image file and allocate memory for Squeak heap */
	if (CaseInsensitiveMatch(imageName,
		"Problems finding the Internet folder in the Squeak Preference folder or finding the SqueakPlugin.image"))
		f = NULL;
	else
		f = sqImageFileOpen(imageName, "rb");
		
	if (f == NULL) {
		if (failureURL[0] != 0x00) {
			// July 31/2004 added per Michaels request for squeakland camp in chicago
			NPN_GetURL(thisInstance, failureURL, "_self");
		} else {
		strcpy(msg, "Could not open Squeak image file \"");
		strcat(msg, imageName);
		strcat(msg, "\"");
		plugInNotifyUser(msg);
		}
		return;
	}
	
	//Cheat and peek ahead to get the image size so we can calculate the memory required 
	
	swapBytes = checkImageVersionFromstartingAt(f, 0);
	headerStart = (sqImageFilePosition(f)) - 4;
	headerSize = getLongFromFileswap(f, swapBytes);
	dataSize = getLongFromFileswap(f, swapBytes);
	
	//Close then reopen to reset file position
	
	sqImageFileClose(f);  
	f = sqImageFileOpen(imageName, "rb");

	squeakHeapMBytes = parseMemorySize(dataSize, rememberMemoryString);
	if (squeakHeapMBytes == 0) 
	    squeakHeapMBytes = STARTINGsqueakHeapMBytes;
	if (squeakHeapMBytes < sqGetAvailableMemory())
		squeakHeapMBytes = sqGetAvailableMemory();
	    
	readImageFromFileHeapSizeStartingAt(f, squeakHeapMBytes, 0);
	sqImageFileClose(f);
	setInterruptKeycode(515);  /* ctrl-C, since Netscape blocks cmd-. */
	setFullScreenFlag(false); //Note image can be saved with true
	SetUpTimers();
}

/*** URL Requests ***/
int	CFNetworkGoGetURL(NPP instance, const char* url, const char* window, void* notifyData);

int URLRequestCreate(char *url, char *target, int semaIndex) {
  /* Start a URL request and return its index. Return -1 if there were
     no idle request handles. */

	int handle, notifyData;
    char mail[8];
    NPError err;
    
    if (thisInstance == null) return -1;
    handle = FindIdleURLRequest();
    if (handle < 0) return handle;
    
	urlRequests[handle].id = nextRequestID++;
	urlRequests[handle].status = STATUS_IN_PROGRESS;
	urlRequests[handle].semaIndex = semaIndex;
	urlRequests[handle].buffer = null;

	notifyData = (urlRequests[handle].id << 8) + handle;
	strncpy(mail,url,7);
	mail[7] = 0x00;
#if defined ( __APPLE__ ) && defined ( __MACH__ )
    	err = CFNetworkGoGetURL(thisInstance, url, target, (void *) notifyData);
#else
	if (CaseInsensitiveMatch(mail,"mailto:"))
    	NPN_GetURLNotify(thisInstance, url, "_self", (void *) notifyData);
	else
    	NPN_GetURLNotify(thisInstance, url, target, (void *) notifyData);
#endif

	return handle;
}

int URLPostCreate(char *url, char *buffer, char * window,int semaIndex) {
  /* Start a URL request and return its index. Return -1 if there were
     no idle request handles. */

	int handle, notifyData;
#if !(defined ( __APPLE__ ) && defined ( __MACH__ ))
	NPError error;
#endif
	
	if (thisInstance == null) return -1;
	handle = FindIdleURLRequest();
	if (handle < 0) return handle;
	urlRequests[handle].id = nextRequestID++;
	urlRequests[handle].status = STATUS_IN_PROGRESS;
	urlRequests[handle].semaIndex = semaIndex;
	urlRequests[handle].buffer = buffer;

	notifyData = (urlRequests[handle].id << 8) + handle;
#if defined ( __APPLE__ ) && defined ( __MACH__ )
        {
        OSStatus		status; //kURLDisplayProgressFlag
        URLState                currentState;
        URLReference            URLRef;
        
        status = URLNewReference(url, &URLRef);
        if (status == noErr) {
            FSSpec        tempFileSpec;
            char fileName[MAX_STRING_LENGTH + 1];
            
             // The URLSetProperty function enables you to set those property values identified by the following
            // constants: kURLPassword, kURLUserName, kURLPassword, kURLHTTPRequestMethod, kURLHTTPRequestHeader,
            // kURLHTTPRequestBody, and kURLHTTPUserAgent.
            URLSetProperty(URLRef, kURLHTTPRequestMethod, "POST", 4);
            URLSetProperty(URLRef, kURLHTTPRequestBody, buffer, strlen(buffer));
            GetTempFSSpec(&tempFileSpec);
            status = URLOpen(URLRef, &tempFileSpec, 0, NULL, NULL, NULL);
            currentState = kURLNullState;
            while (status == noErr && !(currentState == kURLCompletedState || currentState == kURLErrorOccurredState)){
                URLIdle();
                status = URLGetCurrentState (URLRef, &currentState);
            }
            URLGetError(URLRef,&status);
            URLDisposeReference(URLRef);
            if (status != noErr) 
                return -1;
            PathToFile(fileName,MAX_STRING_LENGTH, &tempFileSpec,gCurrentVMEncoding);
            URLRequestCompleted(notifyData, fileName);
        } else
            return -1;
        }
        
#else
	error = NPN_PostURLNotify(thisInstance, url, window, strlen(buffer), buffer, false, (void *) notifyData);
	if (error != NPERR_NO_ERROR) {
		return -1;
	}
#endif
        
	
	return handle;
}

void URLRequestDestroy(int requestHandle) {
  /* Clear the url request with the given handle. */

	if ((requestHandle < 0) || (requestHandle >= URL_REQUEST_COUNT)) {
		return;
	}
	urlRequests[requestHandle].id = 0;
	urlRequests[requestHandle].status = STATUS_IDLE;
	urlRequests[requestHandle].semaIndex = 0;
        
        if (urlRequests[requestHandle].fileName[0] != NULL) {
            sqFileDeleteNameSize((int) &urlRequests[requestHandle].fileName, strlen(urlRequests[requestHandle].fileName));
            urlRequests[requestHandle].fileName[0] = 0x00;
        }
        
	urlRequests[requestHandle].fileName[0] = 0;
	urlRequests[requestHandle].buffer = null;

}

char * URLRequestFileName(int requestHandle) {
  /* Return the filename associated with the url request with the given
     handle. Return null if the request status is not STATUS_SUCCEEDED. */

	if ((requestHandle < 0) ||
		(requestHandle >= URL_REQUEST_COUNT) ||
		(urlRequests[requestHandle].status != STATUS_SUCCEEDED)) {
			return null;
	}

	return urlRequests[requestHandle].fileName;
}

int URLRequestStatus(int requestHandle) {
  /* Return the status of the url request with the given handle. */

	if ((requestHandle < 0) ||
		(requestHandle >= URL_REQUEST_COUNT) ||
		(urlRequests[requestHandle].status < STATUS_IN_PROGRESS)) {
			return STATUS_BAD_HANDLE;
	}
	return urlRequests[requestHandle].status;
}

int FindIdleURLRequest(void) {
  /* Return the index of an idle request or -1 if there are none. */
	int i;

	for (i = 0; i < URL_REQUEST_COUNT; i++) {
		if (urlRequests[i].status == STATUS_IDLE) {
			return i;
		}
	}
	return -1;
}

void URLRequestCompleted(int notifyData, const char* fileName) {
  /* Record that the given request has completed, caching the result in
     the given file name. */

	int handle = notifyData & 0xFF;

	if ((handle < 0) || (handle >= URL_REQUEST_COUNT)) {
		return;
	}
	if (urlRequests[handle].id == (notifyData >> 8)) {
                if (fileName != null) {
		    strncpy(urlRequests[handle].fileName, fileName, MAX_STRING_LENGTH);
                }
		if (urlRequests[handle].buffer != null) {
			NPN_MemFree(urlRequests[handle].buffer);
			urlRequests[handle].buffer = null;
		}
		if (urlRequests[handle].status != STATUS_SUCCEEDED) {
			if (urlRequests[handle].status != STATUS_FAILED) {
				PLUGINDEBUGSTR("\pRequest marked Succeeded;g;");
				urlRequests[handle].status = STATUS_SUCCEEDED;
			}
			if (urlRequests[handle].status == STATUS_FAILED) {
				PLUGINDEBUGSTR("\pRequest marked Failed;g;");
			}
			signalSemaphoreWithIndex(urlRequests[handle].semaIndex);
		}
	}
}

void URLRequestFailed(int notifyData, int reason) {
  /* Record that the given URL request failed. */

	int handle = notifyData & 0xFF;

	if ((handle < 0) || (handle >= URL_REQUEST_COUNT)) {
		return;
	}
	if (urlRequests[handle].id == (notifyData >> 8)) {
		if (urlRequests[handle].buffer != null) {
			NPN_MemFree(urlRequests[handle].buffer);
			urlRequests[handle].buffer = null;
		}
		/* Note: For local files, we're informed that there was a network
		   error (but only after NPP_StreamAsFile has reported success).
		   We could allow local files to be read through the URL request
		   mechanism but NOT failing here if the status has already been
		   set to success. But we don't want to allow the URL mechanism
		   to be used as a loophole to read files outside the sandbox, so
		   we let this fail. */
		if (urlRequests[handle].status != STATUS_SUCCEEDED) {
			PLUGINDEBUGSTR("\pRequest marked Failed;g;");
			urlRequests[handle].status = STATUS_FAILED;
			signalSemaphoreWithIndex(urlRequests[handle].semaIndex);
		}
	}
}

void InitURLRequestTable(void) {
  /* Initialize the table of URL requests. */
	int i;

	for (i = 0; i < URL_REQUEST_COUNT; i++) {
		URLRequestDestroy(i);
	}
	nextRequestID = 1;
}

/*** Squeak I/O Support and Memory Allocation ***/

int ioExit(void) {
  /* Request that we stop running plugin. */

	ioSetFullScreenRestore();
	exitRequested = true;
	return 0;
}

int ioScreenSize(void) {
	int w = 0, h = 0;
	Rect bounds;
	
	if (netscapeWindow != nil) {
		w = netscapeWindow->width;
		h = netscapeWindow->height;
	}
	    
	if (w == 0 && h == 0) { 
	    GetPortBounds(GetWindowPort(stWindow),&bounds);
		w = bounds.right - bounds.left;
		h = bounds.bottom - bounds.top;
	}
	
	return (w << 16) | (h & 0xFFFF);  /* w is high 16 bits; h is low 16 bits */
}

/* Full Screen logic */

Ptr gRestorableStateForScreen = nil;
NP_Port	  gFullScreenNPPort;
NPWindow  *oldNetscapeWindow,gFullScreenNPWindow;
WindowPtr oldStWindow;
int printOnOSX(char *string);
int printOnOSXNumber(int foo);


int ioSetFullScreen(int fullScreen) {
	short desiredWidth,desiredHeight;
    GDHandle   dominantGDevice;
	
	if (fullScreen) {
	    if (getFullScreenFlag()) return 0;
	    
		desiredWidth = 0;
		desiredHeight = 0;
		oldNetscapeWindow = netscapeWindow;
		oldStWindow = stWindow;
		dominantGDevice = getThatDominateGDevice();
		setFullScreenFlag(true);  //JMM Moved before to test
		BeginFullScreen	(&gRestorableStateForScreen,
								dominantGDevice,
								 &desiredWidth,
								 &desiredHeight,
								 &gAFullscreenWindow,
								 nil,
								 fullScreenAllowEvents);
		stWindow = gAFullscreenWindow;
		gFullScreenNPPort.port = GetWindowPort(gAFullscreenWindow);
		gFullScreenNPPort.portx = 0;
		gFullScreenNPPort.porty = 0;
		gFullScreenNPWindow.window =  &gFullScreenNPPort;
		gFullScreenNPWindow.x = 0;
		gFullScreenNPWindow.y = 0;
		gFullScreenNPWindow.width = desiredWidth;
		gFullScreenNPWindow.height = desiredHeight;
		gFullScreenNPWindow.clipRect.top = 0;
		gFullScreenNPWindow.clipRect.left = 0;
		gFullScreenNPWindow.clipRect.bottom = desiredHeight;
		gFullScreenNPWindow.clipRect.right = desiredWidth;
    	
    	netscapeWindow = &gFullScreenNPWindow;
    	ignoreFirstEvent = true;
 	} else {
	    if (!getFullScreenFlag()) return 0;
		setFullScreenFlag(false);
        ioSetFullScreenRestore();
	}
	return 0;
}

int  ioSetFullScreenRestore()
{
	ProcessSerialNumber psn = { 0, kCurrentProcess }; 
	OSErr   err;
	
	if (gRestorableStateForScreen != nil) {
		EndFullScreen(gRestorableStateForScreen,nil);
	    if (gAFullscreenWindow == nil) 
		    return 0;
	    gRestorableStateForScreen = nil;
	    netscapeWindow = oldNetscapeWindow;
	    stWindow = oldStWindow;
		
#if defined ( __APPLE__ ) && defined ( __MACH__ )
		err = ShowHideProcess (&psn,false);
		waitAFewMilliseconds();
#endif
	}
	return 0;
}

/*** File and Access Paths ***/

int InitFilePathsViaDomain(SInt16 domain) {
	short vRefNum;
	char imageInPreferenceFolder[256];
	long dirID;
	OSErr err;
	FSSpec fileSpec;
	char path[VMPATH_SIZE+1];
	
	/* clear all path and file names */
        SetShortImageNameViaString(squeakPluginImageName,gCurrentVMEncoding);

	/* get the path to the sytem folder preference area*/
	err = FindFolder(domain, kPreferencesFolderType, kDontCreateFolder, &vRefNum, &dirID);
	if (err != noErr) {
		return err;
	}
	
	// Look for folder, if not found abort */
	strcpy(imageInPreferenceFolder,":Squeak:Internet:");
	strcat(imageInPreferenceFolder,squeakPluginImageName);
	CopyCStringToPascal(imageInPreferenceFolder,(unsigned char *) imageInPreferenceFolder);
	err = FSMakeFSSpecCompat(vRefNum, dirID,(unsigned char *) imageInPreferenceFolder , &fileSpec);
	if (err != noErr) {
		/* New Behavior try to find the SqueakLand Folder in the Application's Folder */
		err = FindFolder(domain, kApplicationsFolderType, kDontCreateFolder, &vRefNum, &dirID);
		if (err != noErr) 
			goto error;
		strcpy(imageInPreferenceFolder,":SqueakLand:Squeak:Internet:");
		strcat(imageInPreferenceFolder,squeakPluginImageName);
		CopyCStringToPascal(imageInPreferenceFolder,(unsigned char *) imageInPreferenceFolder);
		err = FSMakeFSSpecCompat(vRefNum, dirID,(unsigned char *) imageInPreferenceFolder , &fileSpec);
		if (err != noErr)		
			goto error;
	}	
	/* set the vmPath */
	SetVMPath(&fileSpec);
	getVMPathWithEncoding(path,gCurrentVMEncoding);
	strcat(path, squeakPluginImageName);
	SetImageNameViaString(path,gCurrentVMEncoding);
	return noErr;
	
	error: 
	SetImageNameViaString("Problems finding the Internet folder in the Squeak Preference folder or finding the SqueakPlugin.image",gCurrentVMEncoding);
	return err;
	
}

int InitFilePaths() {
	static const SInt16 domain[] = {kOnSystemDisk,kUserDomain, kLocalDomain, kNetworkDomain, kSystemDomain, 0}; 
	int error;
	SInt32 domainIndex=0;
	
	do {
		error = InitFilePathsViaDomain(domain[domainIndex]);
		if (error == noErr) 
			return noErr;
		domainIndex++;
	} while (domain[domainIndex] != 0); 
        return error;
}

int IsPrefixedBy(char *s, char *prefix) {
  /* Return true if the given string begins with or equals the given prefix. */
	int i;

	for (i = 0; prefix[i] != 0; i++) {
		if (s[i] != prefix[i]) 
			return false;
	}
	return true;
}

int primitivePluginBrowserReady(void) {
	/* Args: none.
	   Always return true on Macintosh. */

	PLUGINDEBUGSTR("\pPrimitiveCallBrowserReady;g;");
	interpreterProxy->pop(1);
	interpreterProxy->pushBool(1);
	return 0;
}


/*** Optional URL Fetch Primitives ***/
#ifdef ENABLE_URL_FETCH
#if defined ( __APPLE__ ) && defined ( __MACH__ )

int	CFNetworkGoGetURL(NPP instance, const char* url, const char* window, void* notifyData)
{
    OSStatus error;
    FSSpec  tempFileSpec;
    char fileName[MAX_STRING_LENGTH + 1];
    
    GetTempFSSpec(&tempFileSpec);
    error = URLSimpleDownload (url,&tempFileSpec,NULL,NULL,NULL,notifyData);
    PathToFile(fileName,MAX_STRING_LENGTH, &tempFileSpec,gCurrentVMEncoding);
    URLRequestCompleted((int) notifyData, fileName);
	return 0;
}

void GetTempFSSpec(FSSpec *spec) {
    char tempName[1024+1];
    CFURLRef    sillyThing;
    CFStringRef filePath;
    
    strcpy(tempName,tmpnam(0));
    
    filePath = CFStringCreateWithBytes(kCFAllocatorDefault,(UInt8 *) tempName,strlen(tempName),kCFStringEncodingUTF8,false);
    sillyThing = CFURLCreateWithFileSystemPath (kCFAllocatorDefault,filePath,kCFURLPOSIXPathStyle,false);
    CFRelease(filePath);
    filePath = CFURLCopyFileSystemPath (sillyThing, kCFURLHFSPathStyle);
    CFRelease(sillyThing);
    CFStringGetCString (filePath,tempName, 1024, kCFStringEncodingMacRoman);
    CFRelease(filePath);
    makeFSSpec(tempName, strlen(tempName),spec);
}

#endif
int primitivePluginDestroyRequest(void) {
	/* Args: handle.
	   Destroy the given request. */

	int handle;

	PLUGINDEBUGSTR("\pPrimitiveCallDestroyRequest;g;");
	handle = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	URLRequestDestroy(handle);
	interpreterProxy->pop(1);
	return 0;
}

int primitivePluginRequestFileHandle(void) {
	/* Args: handle.
	   Return a file handle for the completed request. Fail if
	   the request handle is not valid or hasn't successfully completed. */

	int handle, fileObj;
	char *fileName;
	SQFile *filePtr;

	PLUGINDEBUGSTR("\pPrimitiveCallRequestFileHandle;g;");
	handle = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}

	fileName = URLRequestFileName(handle);
	if (fileName == null) {
		interpreterProxy->success(false);
		return null;
	}

	fileObj = interpreterProxy->instantiateClassindexableSize(
		interpreterProxy->classByteArray(), sizeof(SQFile));
	filePtr = (SQFile *) (fileObj + 4);

	/* Note: OpenFileReadOnly() allows any file on the disk to be read via a file URL.
	   However, we are using it for now because we are not sure where the Netscape cache
	   folder will be--it's folder might even be user-name dependent. sqFileOpen() will
	   only allow opening files within the sandbox, which would be better. Here's the
	   sandbox-safe alternative:
		  sqFileOpen(filePtr, (int) fileName, strlen(fileName), false);
	*/
	OpenFileReadOnly(filePtr, fileName);

	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(2);
	interpreterProxy->push(fileObj);
	return 0;
}

int primitivePluginRequestState(void) {
	/* Args: handle.
	   Return true if the request succeeded, false if it failed, and nil if
	   it is still in progress. Fail if the request handle is not valid. */

	int handle;
	int status;
	int resultObj;

	PLUGINDEBUGSTR("\pPrimitiveCallRequestState;g;");
	handle = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}

	status = URLRequestStatus(handle);
	if (status == STATUS_BAD_HANDLE) {
		interpreterProxy->success(false);
		return null;
	}

	/* default return object: (nil means "in progress") */
	resultObj = interpreterProxy->nilObject();

	if (status == STATUS_FAILED) {
		resultObj = interpreterProxy->falseObject();
	}
	if (status == STATUS_SUCCEEDED) {
		resultObj = interpreterProxy->trueObject();
	}

	interpreterProxy->pop(2);
	interpreterProxy->push(resultObj);
	return 0;
}

int primitivePluginRequestURL(void) {
	/* Args: url, target, semaphoreIndex.
	   Start a URL request to fetch the given URL to the given target.
	   (See the Netscape Plugin programmer's manual for possible targets.)
	   Return a handle that can be used to identify this request. Fail if
	   there are already too many outstanding requests. */

	char *urlPtr;
	char *targetPtr;
	int semaIndex;
	int urlObj, urlSize;
	int targetObj, targetSize;
	char url[MAX_STRING_LENGTH + 1];
	char target[MAX_STRING_LENGTH + 1];
	int i;
	int handle;

	PLUGINDEBUGSTR("\pPrimitiveCallRequestURL;g;");
	semaIndex = interpreterProxy->stackIntegerValue(0);
	targetObj = interpreterProxy->stackObjectValue(1);
	urlObj = interpreterProxy->stackObjectValue(2);
	interpreterProxy->success(interpreterProxy->isBytes(targetObj));
	interpreterProxy->success(interpreterProxy->isBytes(urlObj));
	if (interpreterProxy->failed()) {
		return null;
	}
	urlSize = interpreterProxy->stSizeOf(urlObj);
	urlPtr = interpreterProxy->firstIndexableField(urlObj);
	targetSize = interpreterProxy->stSizeOf(targetObj);
	targetPtr = interpreterProxy->firstIndexableField(targetObj);

	/* copy url into a C string */
	if (urlSize > MAX_STRING_LENGTH) urlSize = MAX_STRING_LENGTH;
	for (i = 0; i < urlSize; i++) {
		url[i] = urlPtr[i];
	}
	url[urlSize] = 0;

	interpreterProxy->success(AbortIfFileURL(url));
	if (interpreterProxy->failed()) {
		return null;
	}
	
	/* copy target into a C string */
	if (targetSize > MAX_STRING_LENGTH) targetSize = MAX_STRING_LENGTH;
	for (i = 0; i < targetSize; i++) {
		target[i] = targetPtr[i];
	}
	target[targetSize] = 0;

	handle = URLRequestCreate(url, target, semaIndex);
	interpreterProxy->success(handle >= 0);

	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(4);
	interpreterProxy->pushInteger(handle);
	return 0;
}

int primitivePluginPostURL(void) {
	/* Args: url, target, semaphoreIndex.
	   Start a URL request to post the given URL to the given target.
	   (See the Netscape Plugin programmer's manual for possible targets.)
	   Return a handle that can be used to identify this request. Fail if
	   there are already too many outstanding requests. */

	char *urlPtr;
	char *targetPtr;
	char *bufferPtr,*buffer;
	int semaIndex;
	int urlObj, urlSize;
	int targetObj, targetSize;
	int bufferObj, bufferSize;
	char url[MAX_STRING_LENGTH + 1];
	char target[MAX_STRING_LENGTH + 1];
	int i;
	int handle;

	PLUGINDEBUGSTR("\pPrimitiveCallPostURL;g;");
	semaIndex = interpreterProxy->stackIntegerValue(0);
	bufferObj = interpreterProxy->stackObjectValue(1);
	targetObj = interpreterProxy->stackObjectValue(2);
	urlObj = interpreterProxy->stackObjectValue(3);
	
	if (targetObj == interpreterProxy->nilObject()) {
	} else {
	    interpreterProxy->success(interpreterProxy->isBytes(targetObj));
	}
	interpreterProxy->success(interpreterProxy->isBytes(urlObj));
	interpreterProxy->success(interpreterProxy->isBytes(bufferObj));
	if (interpreterProxy->failed()) {
		return null;
	}
	urlSize = interpreterProxy->stSizeOf(urlObj);
	urlPtr = interpreterProxy->firstIndexableField(urlObj);
	if (targetObj == interpreterProxy->nilObject()) {
	} else {
    	targetSize = interpreterProxy->stSizeOf(targetObj);
    	targetPtr = interpreterProxy->firstIndexableField(targetObj);
    }
    bufferSize = interpreterProxy->stSizeOf(bufferObj);
	bufferPtr = interpreterProxy->firstIndexableField(bufferObj);

	/* copy url into a C string */
	if (urlSize > MAX_STRING_LENGTH) urlSize = MAX_STRING_LENGTH;
	for (i = 0; i < urlSize; i++) {
		url[i] = urlPtr[i];
	}
	url[urlSize] = 0;

	interpreterProxy->success(AbortIfFileURL(url));
	if (interpreterProxy->failed()) {
		return null;
	}
	
	if (targetObj == interpreterProxy->nilObject()) {
    	target[0] = 0;
	} else {
    	/* copy target into a C string */
    	if (targetSize > MAX_STRING_LENGTH) targetSize = MAX_STRING_LENGTH;
    	for (i = 0; i < targetSize; i++) {
    		target[i] = targetPtr[i];
    	}
    	target[targetSize] = 0;
    }
    
	/* copy over the post buffer which might be large*/
	
	buffer = (char *) NPN_MemAlloc(bufferSize+1);
	for (i = 0; i < bufferSize; i++) {
		buffer[i] = bufferPtr[i];
	}
	buffer[bufferSize] = 0;

	if (targetObj == interpreterProxy->nilObject())
		handle = URLPostCreate(url, buffer,nil,semaIndex) ;
	else
		handle = URLPostCreate(url, buffer,target,semaIndex) ;
	
	interpreterProxy->success(handle >= 0);

	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(5);
	interpreterProxy->pushInteger(handle);
	return 0;
}

int primitivePluginRequestURLStream(void) {
	/* Args: url, semaphoreIndex.
	   Start a URL request to fetch the given URL. Return a handle that
	   can be used to identify this request. Fail if there are already
	   too many outstanding requests. */

	int urlObj, urlSize, semaIndex;
	char *urlPtr;
	char url[MAX_STRING_LENGTH + 1];
	int handle, i;

	PLUGINDEBUGSTR("\pPrimitiveCallRequestURLStream;g;");
	semaIndex = interpreterProxy->stackIntegerValue(0);
	urlObj = interpreterProxy->stackObjectValue(1);
	interpreterProxy->success(interpreterProxy->isBytes(urlObj));
	if (interpreterProxy->failed()) {
		return null;
	}
	urlSize = interpreterProxy->stSizeOf(urlObj);
	urlPtr = interpreterProxy->firstIndexableField(urlObj);

	/* copy url into a C string */
	if (urlSize > MAX_STRING_LENGTH) urlSize = MAX_STRING_LENGTH;
	for (i = 0; i < urlSize; i++) {
		url[i] = urlPtr[i];
	}
	url[urlSize] = 0;

	interpreterProxy->success(AbortIfFileURL(url));
	if (interpreterProxy->failed()) {
		return null;
	}

	handle = URLRequestCreate(url, null, semaIndex);
	interpreterProxy->success(handle >= 0);

	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(3);
	interpreterProxy->pushInteger(handle);
	return 0;
}

void OpenFileReadOnly(SQFile *f, char *MacfileName) {
	/* Opens the given file for reading using the supplied sqFile
	   structure. This is a simplified version of sqFileOpen() that
	   avoids the "sandbox" access check, since the browser's
	   cache folder is outside the Squeak sandbox. That is why
	   we only allow reading of this file. Sets the primitive
	   failure flag if not successful. */
    char fileName[1024];
    
    if (*MacfileName == NULL) {
        interpreterProxy->success(false);
        return;
    }
    sqFilenameFromStringOpen(fileName,(long) MacfileName, strlen(MacfileName));
	f->file = fopen(fileName, "rb");

	f->writable = false;

	if (f->file == NULL) {
		f->sessionID = 0;
		f->fileSize = 0;
		interpreterProxy->success(false);
		return;
	} else {
		f->sessionID = thisSession;
		/* compute and cache file size */
		fseeko(f->file, 0, SEEK_END);
		f->fileSize = ftello(f->file);
		fseeko(f->file, 0, SEEK_SET);
	}
	f->lastOp = 0;
}

#endif

/*** Other ***/

int CaseInsensitiveMatch(char *s1, char *s2) {
  /* Return true if the two strings are the same, not considering case. */
	int len, i, c1, c2;

	len = strlen(s1);
	if (strlen(s2) != len) return false;
	for (i = 0; i < len; i++) {
		c1 = s1[i];
		c2 = s2[i];
		if ((c1 >= 'a') && (c1 <= 'z')) {
			c1 = c1 - ('a' - 'A');
		}
		if ((c2 >= 'a') && (c2 <= 'z')) {
			c2 = c2 - ('a' - 'A');
		}
		if (c1 != c2) return false;
	}
	return true;
}

int StringToInteger(char *s) {
  /* Answer the integer resulting from converting the given
     string, assumed to be decimal integer. */
	int sign, n, i, ch;

	sign = 1;
	n = 0;
	i = 0;
	while ((ch = s[i++]) != 0) {
		if ((ch == '-') && (i == 1)) {
			sign = -1;
		} else {
			if ((ch >= '0') && (ch <= '9')) {
				n = (10 * n) + (ch - '0');
			} else {
				return sign * n;
			}
		}
	}
	return sign * n;
}

void ExitCleanup(void) {
  /* Clean up and stop running plugin. */
	int i;

#if defined ( __APPLE__ ) && defined ( __MACH__ )
        pthread_mutex_lock(&gEventDrawLock);
#endif
	if (thisInstance == nil) return;
	thisInstance = nil;
	exitRequested = true;
#if defined ( __APPLE__ ) && defined ( __MACH__ )
        pthread_mutex_unlock(&gEventDrawLock);
#endif	
	/* do { This hangs things, not sure what to do about outstanding URL requests...
		Boolean URLFetchInProgress;
		URLFetchInProgress = false;
		for (i = 0; i < URL_REQUEST_COUNT; i++) {
			if (urlRequests[i].status == STATUS_IN_PROGRESS) {
				URLFetchInProgress = true;
				break;
			}
		}
		if (URLFetchInProgress) {
			SystemTask();
			YieldToThread(gSqueakThread);
		}
	} while (URLFetchInProgress); */
	
	while(gSqueakThread != kNoThreadID && YieldToThread(gSqueakThread) == noErr){};
	
#if defined ( __APPLE__ ) && defined ( __MACH__ )
        if (gTMTask.tmAddr) {
        
            RemoveTimeTask((QElemPtr)&gTMTask);
            DisposeTimerUPP(gTMTask.tmAddr);
            gTMTask.tmAddr = NULL;
        }
#endif 
        
	plugInShutdown();
	ioSetFullScreenRestore();
	if (pluginArgCount != 0) {
		for(i=0;i<pluginArgCount;i++) {
			NPN_MemFree(pluginArgName[i]);
			NPN_MemFree(pluginArgValue[i]);
		}
		pluginArgCount = 0;
	}
	NPP_Initialize();  /* reset local variables */
        ignoreFirstEvent=false;
        gIWasRunning=false;
}

/*** Interpreter Hooks ***/

int plugInNotifyUser(char *msg) {
  /* Notify the user that there was a problem starting Squeak. */
	unsigned char *		notificationMsg = malloc(256);
	NMRec		*notifyRec = malloc(sizeof(NMRec));

	CopyCStringToPascal(msg,notificationMsg); /* copy message, since notification is asynchronous */

	notifyRec->qType = nmType;
	notifyRec->nmMark = false;			/* no mark in applications menu */
	notifyRec->nmIcon = nil;				/* no menu bar icon */
	notifyRec->nmSound = (Handle) -1;	/* -1 means system beep */
	notifyRec->nmStr = notificationMsg;
	notifyRec->nmResp = (NMUPP) -1;		/* -1 means remove notification when user confirms */

	/* add to notification queue */
	NMInstall(notifyRec);
	return 0;
}

void plugInSetStartTime(void) {
}

int plugInTimeToReturn(void) {
    if (exitRequested)
        return true;
    return false;
}

int parseMemorySize(int baseSize, char *src)
{
	char buf[50], *tmp;
	int imageSize = 0, requestedSize;

	while(*src) {
		switch(*src) {
			case ' ': /* white spaces; ignore */
			case '"':
				src++; break;
			case '*': /* multiple of image size */
				tmp = buf; src++;
				while(*src && isdigit(*src)) *(tmp++) = *(src++); /* integer part */
				if(*src == '.') { /* fraction part */
					*(tmp++) = *(src++);
					while(*src && isdigit(*src)) *(tmp++) = *(src++);
				}
				*(tmp++) = 0;
				imageSize += (int) (baseSize * atof(buf));
				break;
			case '+': /* additional space in bytes */
				tmp = buf; src++;
				while(*src && isdigit(*src)) *(tmp++) = *(src++);
				*(tmp++) = 0;
				if (imageSize == 0) 
					imageSize = baseSize;
				requestedSize = atoi(buf);
				imageSize += (requestedSize <= 1000) ? requestedSize*1024*1024 : requestedSize;
				break;
			default: /* absolute size */
				tmp = buf;
				*(tmp++) = *(src++);
				while(*src && isdigit(*src)) *(tmp++) = *(src++);
				*(tmp++) = 0;
				requestedSize = atoi(buf);
				imageSize = (requestedSize <= 1000) ? requestedSize*1024*1024 : requestedSize;
		}
	}
	return imageSize;
}

int AbortIfFileURL(char *url)
{   char lookFor[6];
	int placement=0;
	
	lookFor[5] = 0x00;
	while (true) {
		if (*url == 0x00) break;
		if (*url == ' ') {
			url++;
		} else {
		  lookFor[placement++] = *url++;
		  if (placement == 5) break;
		}
	}
	return !CaseInsensitiveMatch(lookFor,"file:");
}

#if defined ( __APPLE__ ) && defined ( __MACH__ )
pthread_mutex_t sleepLock;
pthread_cond_t  sleepLockCondition;
void waitAFewMilliseconds()
{
    static Boolean doInitialization=true;
    const int	   realTimeToWait = 16;
    struct timespec tspec;
    int err;
    
    if (doInitialization) {
        doInitialization = false;
        pthread_mutex_init(&sleepLock, NULL);
        pthread_cond_init(&sleepLockCondition,NULL);
    }

    tspec.tv_sec=  realTimeToWait / 1000;
    tspec.tv_nsec= (realTimeToWait % 1000)*1000000;
    
    err = pthread_mutex_lock(&sleepLock);
    err = pthread_cond_timedwait_relative_np(&sleepLockCondition,&sleepLock,&tspec);	
    err = pthread_mutex_unlock(&sleepLock); 
}

#ifdef JMMFoo

/* LowRunAppleScript compiles and runs an AppleScript
    provided as text in the buffer pointed to by text.  textLength
    bytes will be compiled from this buffer and run as an AppleScript
    using all of the default environment and execution settings.  If
    resultData is not NULL, then the result returned by the execution
    command will be returned as typeChar in this descriptor record
    (or typeNull if there is no result information).  If the function
    returns errOSAScriptError, then resultData will be set to a
    descriptive error message describing the error (if one is
    available).  */
static OSStatus LowRunAppleScript(const void* text, long textLength,
                                    AEDesc *resultData) {
    ComponentInstance theComponent;
    AEDesc scriptTextDesc;
    OSStatus err;
    OSAID scriptID, resultID;

        /* set up locals to a known state */
    theComponent = NULL;
    AECreateDesc(typeNull, NULL, 0, &scriptTextDesc);
    scriptID = kOSANullScript;
    resultID = kOSANullScript;

        /* open the scripting component */
    theComponent = OpenDefaultComponent(kOSAComponentType,
                    typeAppleScript);
    if (theComponent == NULL) { err = paramErr; goto bail; }

        /* put the script text into an aedesc */
    err = AECreateDesc(typeChar, text, textLength, &scriptTextDesc);
    if (err != noErr) goto bail;

        /* compile the script */
    err = OSACompile(theComponent, &scriptTextDesc,
                    kOSAModeNull, &scriptID);
    if (err != noErr) goto bail;

        /* run the script */
    err = OSAExecute(theComponent, scriptID, kOSANullScript,
                    kOSAModeNull, &resultID);

        /* collect the results - if any */
    if (resultData != NULL) {
        AECreateDesc(typeNull, NULL, 0, resultData);
        if (err == errOSAScriptError) {
            OSAScriptError(theComponent, kOSAErrorMessage,
                        typeChar, resultData);
        } else if (err == noErr && resultID != kOSANullScript) {
            OSADisplay(theComponent, resultID, typeChar,
                        kOSAModeNull, resultData);
        }
    }
bail:
    AEDisposeDesc(&scriptTextDesc);
    if (scriptID != kOSANullScript) OSADispose(theComponent, scriptID);
    if (resultID != kOSANullScript) OSADispose(theComponent, resultID);
    if (theComponent != NULL) CloseComponent(theComponent);
    return err;
}


    /* SimpleRunAppleScript compiles and runs the AppleScript in
    the c-style string provided as a parameter.  The result returned
    indicates the success of the operation. */
static OSStatus SimpleRunAppleScript(const char* theScript) {
    return LowRunAppleScript(theScript, strlen(theScript), NULL);
}

static pokeAtSafari(void) {
   SimpleRunAppleScript(
       "tell application \"Finder\"\n"
       "  activate\n"
       "  select folder \"Documents\" of startup disk\n"
       "  open selection\n"
       "end tell");
}

/* example:
   SimpleRunAppleScript(
       "tell application \"Finder\"\n"
       "  activate\n"
       "  select folder \"Documents\" of startup disk\n"
       "  open selection\n"
       "end tell");
   */

#endif
#endif