/* win32 sqPlatformSpecific.h -- Platform-specific prototypes and definitions */

/* How to use this file:
   This file is for general platform-specific macros and declarations.
   The goal is to keep most of the other header files generic across platforms.
   To override a definition or macro from sq.h, you must first #undef it, then
   provide the new definition.

*/



#ifdef WIN32
/* Override necessary definitions */
#undef putchar
#include "sqWin32Alloc.h"

#include <stddef.h>  /* Need this for use size_t */
#include <windows.h>

#ifdef _MSC_VER
#define squeakFileOffsetType __int64
#else
#define squeakFileOffsetType unsigned long long
#endif

#ifdef WIN32_FILE_SUPPORT

#undef sqImageFile
#undef sqImageFileClose
#undef sqImageFileOpen
#undef sqImageFilePosition
#undef sqImageFileRead
#undef sqImageFileSeek
#undef sqImageFileWrite

#define sqImageFile unsigned long
int sqImageFileClose(sqImageFile h);
sqImageFile sqImageFileOpen(char *fileName, char *mode);
squeakFileOffsetType sqImageFilePosition(sqImageFile h);
size_t sqImageFileRead(void *ptr, size_t sz, size_t count, sqImageFile h);
squeakFileOffsetType sqImageFileSeek(sqImageFile h, squeakFileOffsetType pos);
size_t sqImageFileWrite(void *ptr, size_t sz, size_t count, sqImageFile h);

#endif /* WIN32_FILE_SUPPORT */

/********************************************************/
/* external SYNCHRONIZED signaling of semaphores        */
/********************************************************/
int synchronizedSignalSemaphoreWithIndex (PInterpreter intr, int semaIndex);


/* pluggable primitive support */
#if defined(_MSC_VER) || defined(__MINGW32__)
#  undef EXPORT
#  define EXPORT(returnType) __declspec( dllexport ) returnType
#endif 

/* undefine clock macros that are implemented as functions */
#undef ioMSecs
#undef ioLowResMSecs
#undef ioMicroMSecs

/* sig: ioLowResMSecs no longer supported nor used 

extern int _lowResMSecs;
#define ioLowResMSecs() _lowResMSecs
*/


/* Per-Interpreter win32 vm state */
extern int win32stateId;

typedef struct Win32AttachedState {
	HANDLE wakeUpEvent;
	HANDLE timerThread;
	HANDLE sleepEvent;

	volatile DWORD delayTick;
	vmEvent ioProcessEventsEvt;
	volatile int signalId;

	#define KEYBUF_SIZE 64
	int keyBuf[KEYBUF_SIZE];	/* circular buffer */
	int keyBufGet;				/* index of next item of keyBuf to read */
	int keyBufPut;				/* index of next item of keyBuf to write */
	int keyBufOverflows;		/* number of characters dropped */

	int inputSemaphoreIndex;	/* if non-zero the event semaphore index */

	#define MAX_EVENT_BUFFER 1024
	struct sqInputEvent * eventBuffer;
	int eventBufferGet;
	int eventBufferPut;

#define IMAGE_NAME_SIZE MAX_PATH

	char imageName[MAX_PATH+1];		  /* full path and name to image */
	TCHAR imagePath[MAX_PATH+1];	  /* full path to image */
	TCHAR windowTitle[MAX_PATH];      /* window title string */

} Win32AttachedState;

#define DECL_WIN32_STATE() struct Win32AttachedState * win32state = getAttachedStateBufferof(win32stateId, _iparam())
#define DECL_WIN32_STATE2(i) struct Win32AttachedState * win32state = getAttachedStateBufferof(win32stateId, i)
#define WIN32_STATE(name) win32state->name


#else 
#error "Not Win32!"
#endif /* WIN32 */
