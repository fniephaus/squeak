/* sqPlatformSpecific.h -- platform-specific modifications to sq.h
 * 
 *   Copyright (C) 1996-2005 by Ian Piumarta and other authors/contributors
 *                              listed elsewhere in this file.
 *   All rights reserved.
 *   
 *   This file is part of Unix Squeak.
 * 
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 * 
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * 
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 * 
 * Author: ian.piumarta@squeakland.org
 * 
 * Last edited: 2006-10-18 10:06:13 by piumarta on emilia.local
 */

/* undefine clock macros (these are implemented as functions) */

#undef ioMSecs
#undef ioMicroMSecs
#undef ioLowResMSecs

#undef sqAllocateMemory
#undef sqGrowMemoryBy
#undef sqShrinkMemoryBy
#undef sqMemoryExtraBytesLeft

/* Both pthread.h and squeak define a "clone" function (pthread.h via
 * sched.h, and squeak via the generated interp_prototypes.h. Undefining
 * __USE_MISC stops pthread from defining the function, at least on
 * linux
 */
#ifdef __GLIBC__
# ifdef __USE_MISC
#  undef __USE_MISC
# endif /* __USE_MISC */
#endif /* __GLIBC__ */
#include <pthread.h>

#include "sqMemoryAccess.h"

extern sqInt sqAllocateMemory(sqInt minHeapSize, sqInt desiredHeapSize);
extern sqInt sqGrowMemoryBy(sqInt oldLimit, sqInt delta);
extern sqInt sqShrinkMemoryBy(sqInt oldLimit, sqInt delta);
extern sqInt sqMemoryExtraBytesLeft(sqInt includingSwap);

#include <sys/types.h>
#include <sys/param.h>

typedef off_t squeakFileOffsetType;

#define IMAGE_NAME_SIZE MAXPATHLEN

#ifndef sqImageFile
/*XXXXXXXXX EVIL HACK: copy sqImageFile and friends from sq.h XXXXXXX*/

/* Note: The image file save and restore code uses these macros; they
   can be redefined in sqPlatformSpecific.h if desired. These default
   versions are defined in terms of the ANSI Standard C libraries.
*/
#define sqImageFile					   FILE *
/*
#define sqImageFileClose(f)                  		   fclose(f)
#define sqImageFileOpen(fileName, mode)      		   fopen(fileName, mode)
#define sqImageFilePosition(f)               		   ftell(f)
#define sqImageFileRead(ptr, sz, count, f)   		   fread(ptr, sz, count, f)
#define sqImageFileSeek(f, pos)              		   fseek(f, pos, SEEK_SET)
#define sqImageFileWrite(ptr, sz, count, f)  		   fwrite(ptr, sz, count, f)
#define sqImageFileStartLocation(fileRef, fileName, size)  0
*/
#endif /* sqImageFile */

#undef	sqFilenameFromString
#undef	sqFilenameFromStringOpen
#define sqFilenameFromStringOpen sqFilenameFromString

extern void sqFilenameFromString(char *uxName, sqInt stNameIndex, int sqNameLength);

#undef dispatchFunctionPointer
#undef dispatchFunctionPointerOnin
/* we'd like to untypedef fptr too, but such is life */

#include <unistd.h>

#undef	sqFTruncate
#define	sqFTruncate(f,o) ftruncate(fileno(f), o)

#ifndef __GNUC__
# if HAVE_ALLOCA_H
#   include <alloca.h>
# else
#   ifdef _AIX
#     pragma alloca
#   else
#     ifndef alloca /* predefined by HP cc +Olibcalls */
        char *alloca();
#     endif
#   endif
# endif
#endif

/* Per-Interpreter unix vm state */
extern int unixStateId;

typedef struct UnixAttachedState {
	pthread_t timerThread;

	pthread_cond_t sleepEvent;
	pthread_cond_t wakeUpEvent;
	int delayTick;

	vmEvent ioProcessEventsEvt;
	vmEvent ioSignalDelayEvent;

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


	char imagePath[IMAGE_NAME_SIZE+1];	  /* full path to image */
	char windowTitle[IMAGE_NAME_SIZE];      /* window title string */

/* The above is from windows. What follows is from Unix */

  char shortImageName[MAXPATHLEN+1];	  /* image name */
  char      imageName[IMAGE_NAME_SIZE+1]; /* full path and name to image */

} UnixAttachedState;

#define DECL_INTERP_LOCAL() struct UnixAttachedState * interp_local_state = getAttachedStateBuffer(intr, unixStateId)
#define INTERP_LOCAL(name) interp_local_state->name

