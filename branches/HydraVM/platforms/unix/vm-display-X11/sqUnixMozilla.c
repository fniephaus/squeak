/* sqUnixMozilla.c -- support for Squeak Netscape plugin
 *
 * Author: Bert Freudenberg <bert@isg.cs.uni-magdeburg.de>
 * 
 * Last edited: 2007-09-07 13:53:35 by piumarta on emilia
 *
 * Originally based on Andreas Raab's sqWin32PluginSupport
 * 
 * Notes: The plugin window handling stuff is in sqXWindow.c.
 *        browserProcessCommand() is called when data is available
 */

#include "sq.h"

#if defined(USE_X11)

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>

#undef DEBUG

#ifdef DEBUG
void DPRINT(char *format, ...)
{
  static int debug= 42;
  if (42 == debug) 
    debug= (NULL != getenv("NPSQUEAK_DEBUG"));

  if (!debug)
    {
      return;
    }
  else
    {
      static FILE *file= 0;
      if (!file) 
	{
	  file= fopen("/tmp/npsqueak.log", "a+");
	}

      if (file) {
	va_list ap;
	va_start(ap, format);
	vfprintf(file, format, ap);
	va_end(ap);
	fflush(file);
      }
    }
}
#else
void DPRINT(char *format, ...) { }
#endif

/* from sqXWindow.c */
extern Display* stDisplay;
extern Window   stWindow;
extern Window   browserWindow;
extern Window   stParent;
extern int      browserPipes[2];

/* from interpret.c */
sqInt isBytes(sqInt);
sqInt byteSizeOf(sqInt);

/* prototypes */

static void browserReceive(void *buf, size_t count);
static void browserSend(const void *buf, size_t count);
static void browserSendInt(int value);
static void browserReceiveData(INTERPRETER_ARG);
static void browserGetURLRequest(int id, char* url, int urlSize,
				char* target, int targetSize);
static void browserPostURLRequest(int id, char* url, int urlSize, 
				 char* target, int targetSize, 
				 char* postData, int postDataSize);

typedef struct sqStreamRequest {
  char *localName;
  int semaIndex;
  int state;
} sqStreamRequest;

#define MAX_REQUESTS 128

#define SQUEAK_READ 0
#define SQUEAK_WRITE 1

#define inBrowser\
  (-1 != browserPipes[SQUEAK_READ])

#define CMD_BROWSER_WINDOW 1
#define CMD_GET_URL        2
#define CMD_POST_URL       3
#define CMD_RECEIVE_DATA   4

static sqStreamRequest *requests[MAX_REQUESTS];



/* primitives called from Squeak */



/*
  primitivePluginBrowserReady
  Return true if a connection to the browser
  has been established. Only necessary if some
  sort of asynchronous communications are used.
*/
int display_primitivePluginBrowserReady(INTERPRETER_ARG)
{
  if (inBrowser)
    {
      pop(INTERPRETER_PARAM_COMMA 1);
      pushBool(INTERPRETER_PARAM_COMMA 1);
    }
  else
    primitiveFail(INTERPRETER_PARAM);
  return 1;
}


/*
  primitivePluginRequestUrlStream: url with: semaIndex
  Request a URL from the browser. Signal semaIndex
  when the result of the request can be queried.
  Returns a handle used in subsequent calls to plugin
  stream functions.
  Note: A request id is the index into requests[].
*/
int display_primitivePluginRequestURLStream(INTERPRETER_ARG)
{
  sqStreamRequest *req;
  int id, url, length, semaIndex;

  if (!inBrowser) return primitiveFail(INTERPRETER_PARAM);

  DPRINT("VM: primitivePluginRequestURLStream()\n");

  for (id=0; id<MAX_REQUESTS; id++) {
    if (!requests[id]) break;
  }
  if (id >= MAX_REQUESTS) return primitiveFail(INTERPRETER_PARAM);

  semaIndex= stackIntegerValue(INTERPRETER_PARAM_COMMA 0);
  url= stackObjectValue(INTERPRETER_PARAM_COMMA 1);
  if (failed(INTERPRETER_PARAM)) return 0;

  if (!isBytes(url)) return primitiveFail(INTERPRETER_PARAM);

  req= calloc(1, sizeof(sqStreamRequest));
  if (!req) return primitiveFail(INTERPRETER_PARAM);
  req->localName= NULL;
  req->semaIndex= semaIndex;
  req->state= -1;
  requests[id]= req;

  length= byteSizeOf(url);
  browserGetURLRequest(id, firstIndexableField(INTERPRETER_PARAM_COMMA url), length, NULL, 0);
  pop(INTERPRETER_PARAM_COMMA 3);
  push(INTERPRETER_PARAM_COMMA positive32BitIntegerFor(INTERPRETER_PARAM_COMMA id));
  DPRINT("VM:   request id: %i\n", id);
  return 1;
}


/*
  primitivePluginRequestURL: url target: target semaIndex: semaIndex
  Request a URL into the given target.
*/
int display_primitivePluginRequestURL(INTERPRETER_ARG)
{
  sqStreamRequest *req;
  int url, urlLength;
  int target, targetLength;
  int id, semaIndex;

  if (!browserWindow) return primitiveFail(INTERPRETER_PARAM);
  for (id=0; id<MAX_REQUESTS; id++) {
    if (!requests[id]) break;
  }

  if (id >= MAX_REQUESTS) return primitiveFail(INTERPRETER_PARAM);

  semaIndex= stackIntegerValue(INTERPRETER_PARAM_COMMA 0);
  target= stackObjectValue(INTERPRETER_PARAM_COMMA 1);
  url= stackObjectValue(INTERPRETER_PARAM_COMMA 2);

  if (failed(INTERPRETER_PARAM)) return 0;
  if (!isBytes(url) || !isBytes(target)) return primitiveFail(INTERPRETER_PARAM);

  urlLength= byteSizeOf(url);
  targetLength= byteSizeOf(target);

  req= calloc(1, sizeof(sqStreamRequest));
  if(!req) return primitiveFail(INTERPRETER_PARAM);
  req->localName= NULL;
  req->semaIndex= semaIndex;
  req->state= -1;
  requests[id]= req;

  browserGetURLRequest(id, firstIndexableField(INTERPRETER_PARAM_COMMA url), urlLength, firstIndexableField(INTERPRETER_PARAM_COMMA target), targetLength);
  pop(INTERPRETER_PARAM_COMMA 4);
  push(INTERPRETER_PARAM_COMMA positive32BitIntegerFor(INTERPRETER_PARAM_COMMA id));
  return 1;
}


/*
  primitivePluginPostURL: url target: target data: data semaIndex: semaIndex
  Post data to a URL into the given target.
*/
int display_primitivePluginPostURL(INTERPRETER_ARG)
{
  sqStreamRequest *req;
  int url, urlLength;
  int target, targetLength;
  int data, dataLength;
  int id, semaIndex;

  if (!browserWindow) return primitiveFail(INTERPRETER_PARAM);
  for (id=0; id<MAX_REQUESTS; id++) {
    if (!requests[id]) break;
  }

  if (id >= MAX_REQUESTS) return primitiveFail(INTERPRETER_PARAM);

  semaIndex= stackIntegerValue(INTERPRETER_PARAM_COMMA 0);
  data= stackObjectValue(INTERPRETER_PARAM_COMMA 1);
  target= stackObjectValue(INTERPRETER_PARAM_COMMA 2);
  url= stackObjectValue(INTERPRETER_PARAM_COMMA 3);

  if (failed(INTERPRETER_PARAM)) return 0;
  if (target == nilObject(INTERPRETER_PARAM)) target= 0;
  if (!isBytes(url) || !isBytes(data) || !(!target || isBytes(target))) return primitiveFail(INTERPRETER_PARAM);

  urlLength= byteSizeOf(url);
  targetLength= target ? byteSizeOf(target) : 0;
  dataLength= byteSizeOf(data);

  req= calloc(1, sizeof(sqStreamRequest));
  if(!req) return primitiveFail(INTERPRETER_PARAM);
  req->localName= NULL;
  req->semaIndex= semaIndex;
  req->state= -1;
  requests[id]= req;

  browserPostURLRequest(id, firstIndexableField(INTERPRETER_PARAM_COMMA url), urlLength, 
			target ? firstIndexableField(INTERPRETER_PARAM_COMMA target) : NULL, targetLength,
			firstIndexableField(INTERPRETER_PARAM_COMMA data), dataLength);
  pop(INTERPRETER_PARAM_COMMA 4);
  push(INTERPRETER_PARAM_COMMA positive32BitIntegerFor(INTERPRETER_PARAM_COMMA id));
  return 1;
}

/* 
  primitivePluginRequestFileHandle: id
  After a URL file request has been successfully
  completed, return a file handle for the received
  data. Note: The file handle must be read-only for
  security reasons.
*/
int display_primitivePluginRequestFileHandle(INTERPRETER_ARG)
{
  sqStreamRequest *req;
  int id, fileOop;
  void *openFn;

  id= stackIntegerValue(INTERPRETER_PARAM_COMMA 0);
  if (failed(INTERPRETER_PARAM)) return 0;
  if (id < 0 || id >= MAX_REQUESTS) return primitiveFail(INTERPRETER_PARAM);

  req= requests[id];
  if (!req || !req->localName) return primitiveFail(INTERPRETER_PARAM);

  fileOop= nilObject(INTERPRETER_PARAM);

  if (req->localName)
    {
      DPRINT("VM: Creating file handle for %s\n", req->localName);
 
      openFn= ioLoadFunctionFrom("fileOpenNamesizewritesecure", "FilePlugin");
      if (!openFn)
      {
	DPRINT("VM:   Couldn't load fileOpenName:size:write:secure: from FilePlugin!\n");
	return primitiveFail(INTERPRETER_PARAM);
      }
  
      fileOop= ((sqInt (*)(char *, sqInt, sqInt, sqInt))openFn)
	(req->localName, strlen(req->localName), 0 /* readonly */, 0 /* insecure */);
 
      /* if file ends in a $, it was a temp link created by the plugin */
      if ('$' == req->localName[strlen(req->localName) - 1])
      {
	DPRINT("VM:   unlink %s\n", req->localName);
	if (-1 == unlink(req->localName))
	  DPRINT("VM:   unlink failed: %s\n", strerror(errno));
      }

      if (failed(INTERPRETER_PARAM)) 
	{
	  DPRINT("VM:   file open failed\n");
	  return 0;
	}
    }
  pop(INTERPRETER_PARAM_COMMA 2);
  push(INTERPRETER_PARAM_COMMA fileOop);
  return 1;
}


/*
  primitivePluginDestroyRequest: id
  Destroy a request that has been issued before.
*/
sqInt display_primitivePluginDestroyRequest(INTERPRETER_ARG)
{
  sqStreamRequest *req;
  int id;

  id= stackIntegerValue(INTERPRETER_PARAM_COMMA 0);
  if (id < 0 || id >= MAX_REQUESTS) return primitiveFail(INTERPRETER_PARAM);
  req= requests[id];
  if (req) {
    if (req->localName) free(req->localName);
    free(req);
  }
  requests[id]= NULL;
  pop(INTERPRETER_PARAM_COMMA 1);
  return 1;
}


/*
  primitivePluginRequestState: id
  Return true if the operation was successfully completed.
  Return false if the operation was aborted.
  Return nil if the operation is still in progress.
*/
sqInt display_primitivePluginRequestState(INTERPRETER_ARG)
{
  sqStreamRequest *req;
  int id;

  id= stackIntegerValue(INTERPRETER_PARAM_COMMA 0);
  if (id < 0 || id >= MAX_REQUESTS) return primitiveFail(INTERPRETER_PARAM);
  req= requests[id];
  if (!req) return primitiveFail(INTERPRETER_PARAM);
  pop(INTERPRETER_PARAM_COMMA 2);
  if (req->state == -1) push(INTERPRETER_PARAM_COMMA nilObject(INTERPRETER_PARAM));
  else pushBool(INTERPRETER_PARAM_COMMA req->state);
  return 1;
}



/* helper functions */

static void browserReceive(void *buf, size_t count)
{
  ssize_t n;
  n= read(browserPipes[SQUEAK_READ], buf, count);
  if (n == -1)
    perror("Squeak read failed:");
  if (n < count)
    fprintf(stderr, "Squeak read too few data from pipe\n");
}


static void browserSend(const void *buf, size_t count)
{
  ssize_t n;
  n= write(browserPipes[SQUEAK_WRITE], buf, count);
  if (n == -1)
    perror("Squeak plugin write failed:");
  if (n < count)
    fprintf(stderr, "Squeak wrote too few data to pipe\n");
}

static void browserSendInt(int value)
{
  browserSend(&value, 4);
}


/*
  browserReceiveData:
  Called in response to a CMD_RECEIVE_DATA message.
  Retrieves the data file name and signals the semaphore.
*/
static void browserReceiveData(INTERPRETER_ARG)
{
  char *localName= NULL;
  int id, ok;

  browserReceive(&id, 4);
  browserReceive(&ok, 4);

  DPRINT("VM:  receiving data id: %i state %i\n", id, ok);

  if (ok == 1) {
    int length= 0;
    browserReceive(&length, 4);
    if (length) {
      localName= malloc(length+1);
      browserReceive(localName, length);
      localName[length]= 0;
      DPRINT("VM:   got filename %s\n", localName);
    }
  }
  if (id >= 0 && id < MAX_REQUESTS) {
    sqStreamRequest *req= requests[id];
    if (req) {
      req->localName= localName;
      req->state= ok;
      DPRINT("VM:  signaling semaphore, state=%i\n", ok);
      /*  synchronizedSignalSemaphoreWithIndex(req->semaIndex);*/
      signalSemaphoreWithIndex(INTERPRETER_PARAM_COMMA req->semaIndex);
    }
  }
}


/*
  browserGetURLRequest:
  Notify plugin to get the specified url into target
*/
static void browserGetURLRequest(int id, char* url, int urlSize, 
				char* target, int targetSize)
{
  if (!inBrowser) {
    fprintf(stderr, "Cannot submit URL request -- "
	    "there is no connection to a browser\n");
    return;
  }

  browserSendInt(CMD_GET_URL);
  browserSendInt(id);

  browserSendInt(urlSize);
  if (urlSize > 0)
    browserSend(url, urlSize);

  browserSendInt(targetSize);
  if (targetSize > 0)
    browserSend(target, targetSize);
}


/*
  browserPostURLRequest:
  Notify plugin to post data to the specified url and get result into target
*/
static void browserPostURLRequest(int id, char* url, int urlSize, 
				 char* target, int targetSize, 
				 char* postData, int postDataSize)
{
  if (!inBrowser) {
    fprintf(stderr, "Cannot submit URL post request -- "
	    "there is no connection to a browser\n");
    return;
  }

  browserSendInt(CMD_POST_URL);
  browserSendInt(id);

  browserSendInt(urlSize);
  if (urlSize > 0)
    browserSend(url, urlSize);

  browserSendInt(targetSize);
  if (targetSize > 0)
    browserSend(target, targetSize);

  browserSendInt(postDataSize);
  if (postDataSize > 0)
    browserSend(postData, postDataSize);
}


/***************************************************************
 * Functions called from sqXWindow.c
 ***************************************************************/

/*
  browserProcessCommand:
  Handle commands sent by the plugin.
*/
void browserProcessCommand(INTERPRETER_ARG)
{
  static int firstTime= 1;
  int cmd, n;

  if (firstTime)
    {
      firstTime= 0;
      /* enable non-blocking reads */
      fcntl(browserPipes[SQUEAK_READ], F_SETFL, O_NONBLOCK);
    }
  DPRINT("VM: browserProcessCommand()\n");

  n= read(browserPipes[SQUEAK_READ], &cmd, 4);
  if (0 == n || (-1 == n && EAGAIN == errno))
    return;

  switch (cmd)
    {
    case CMD_RECEIVE_DATA:
      /* Data is coming in */
      browserReceiveData(INTERPRETER_PARAM);
      break;
    case CMD_BROWSER_WINDOW:
      /* Parent window has changed () */
      browserReceive(&browserWindow, 4);
      stParent= browserWindow;
      DPRINT("VM:  got browser window 0x%X\n", browserWindow);
      break;
    default:
      fprintf(stderr, "Unknown command from Plugin: %i\n", cmd);
    }
}


#else /* !defined(USE_X11) */

sqInt display_primitivePluginBrowserReady()		{ return primitiveFail(INTERPRETER_PARAM); }
sqInt display_primitivePluginRequestURLStream()		{ return primitiveFail(INTERPRETER_PARAM); }
sqInt display_primitivePluginRequestURL()		{ return primitiveFail(INTERPRETER_PARAM); }
sqInt display_primitivePluginPostURL()			{ return primitiveFail(INTERPRETER_PARAM); }
sqInt display_primitivePluginRequestFileHandle()	{ return primitiveFail(INTERPRETER_PARAM); }
sqInt display_primitivePluginDestroyRequest()		{ return primitiveFail(INTERPRETER_PARAM); }
sqInt display_primitivePluginRequestState()		{ return primitiveFail(INTERPRETER_PARAM); }

#endif