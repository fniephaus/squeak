/****************************************************************************
*   PROJECT: Squeak port for Win32 (NT / Win95)
*   FILE:    sqWin32Utils.c
*   CONTENT: Utilities
*
*   AUTHOR:  Andreas Raab (ar)
*   ADDRESS: University of Magdeburg, Germany
*   EMAIL:   raab@isg.cs.uni-magdeburg.de
*   RCSID:   $Id$
*
*   NOTES:
*****************************************************************************/
#define _WIN32_WINDOWS 0x500
#include <windows.h>
#include "sq.h"

/*****************************************************************************
  String conversions: Unicode / Ansi / Squeak
  NOTES: 
    1) The length of strings in inline functions MUST NOT exceed MAX_PATH.
    2) fromSqueak() and fromSqueak2() are inline conversions
       but operate on two different buffers.
    3) toUnicode() and fromUnicode() are inline conversions
       with for at most MAX_PATH sized strings. If the VM
       is not compiled with UNICODE defined they just return
       the input strings. Also, toUnicode operates on the
       same buffer as fromSqueak()
    4) toUnicodeNew and fromUnicodeNew malloc() new strings.
       It is up to the caller to free these!
*****************************************************************************/


#define MAX_BUFFER MAX_PATH
static TCHAR w_buffer1[MAX_BUFFER]; /* wide buffer 1 */
static TCHAR w_buffer2[MAX_BUFFER]; /* wide buffer 2 */
static char  a_buffer1[MAX_BUFFER]; /* ansi buffer 1 */

static TCHAR *fromSqueakInto(const char *sqPtr, int sqSize, TCHAR* buffer)
{
  int i;
  if(sqSize >= MAX_BUFFER) sqSize = MAX_BUFFER-1;
  for(i=0;i<sqSize;i++)
    buffer[i] = (unsigned char) (sqPtr[i]); /* will be extended with zeros */
  buffer[sqSize] = 0;
  return buffer;
}

TCHAR *fromSqueak(const char *sqPtr, int sqSize)
{
  return fromSqueakInto(sqPtr, sqSize, w_buffer1);
}

TCHAR *fromSqueak2(const char *sqPtr, int sqSize)
{
  return fromSqueakInto(sqPtr, sqSize, w_buffer2);
}

TCHAR *toUnicode(const char *ptr)
{
#ifdef UNICODE
  return fromSqueak(ptr, strlen(ptr));
#else
  return (char*) ptr;
#endif
}

char *fromUnicode(const TCHAR *ptr)
{
#ifdef UNICODE
  int i, size;
  size = lstrlen(ptr);
  if(size >= MAX_BUFFER) size = MAX_BUFFER - 1;
  for(i=0;i<size;i++)
    a_buffer1[i] = (unsigned char) ptr[i];
  a_buffer1[size] = 0;
  return a_buffer1;
#else
  return (char*) ptr;
#endif
}

TCHAR *toUnicodeNew(const char *ptr)
{ TCHAR *result;
  int i,size;

  size = strlen(ptr)+1;
  result = (TCHAR*) calloc(size,sizeof(TCHAR));
  for(i=0;i<size;i++)
    result[i] = (unsigned char) ptr[i];
  return result;
}

char *fromUnicodeNew(const TCHAR *ptr)
{ char *result;
  int i,size;

  size = lstrlen(ptr)+1;
  result = (char*) calloc(size,sizeof(char));
  for(i=0;i<size;i++)
    result[i] = (unsigned char) ptr[i];
  return result;
}

TCHAR *lstrrchr(TCHAR *source, TCHAR c)
{ TCHAR *tmp;

  /* point to the last char */
  tmp = source + lstrlen(source)-1;
  while(tmp >= source)
    if(*tmp == c) return tmp;
    else tmp--;
  return NULL;
}


/****************************************************************************/
/* Helper to pop up a message box with a message formatted from the         */
/*   printf() format string and arguments                                   */
/****************************************************************************/
#ifndef sqMessageBox
int __cdecl sqMessageBox(DWORD dwFlags, const TCHAR *titleString, const char* fmt, ...)
{ TCHAR *ptr, *buf;
	va_list args;
  DWORD result;

  ptr = toUnicodeNew(fmt);
  buf = (TCHAR*) calloc(sizeof(TCHAR), 4096);
	va_start(args, fmt);
	wvsprintf(buf, ptr, args);
	va_end(args);

	result = MessageBox(stWindow,buf,titleString,dwFlags|MB_SETFOREGROUND);
  free(ptr);
  free(buf);
  return result;
}
#endif

/****************************************************************************/
/* Print out a message and abort execution                                  */
/****************************************************************************/
#ifndef abortMessage
int __cdecl abortMessage(const TCHAR* fmt, ...)
{ TCHAR *buf;
	va_list args;

  buf = (TCHAR*) calloc(sizeof(TCHAR), 4096);
	va_start(args, fmt);
	wvsprintf(buf, fmt, args);
	va_end(args);

	MessageBox(NULL,buf,TEXT(VM_NAME"!"),MB_OK | MB_TASKMODAL | MB_SETFOREGROUND);
  free(buf);
  exit(-1);
}
#endif

/****************************************************************************/
/* Print out a warning message                                              */
/****************************************************************************/
#ifndef warnPrintf
int __cdecl warnPrintf(const TCHAR *fmt, ...)
{ TCHAR *buf;
	va_list args;

  buf = (TCHAR*) calloc(sizeof(TCHAR), 4096);
	va_start(args, fmt);
	wvsprintf(buf, fmt, args);
	va_end(args);
  MessageBox(stWindow, buf, TEXT(VM_NAME" Warning!"), MB_OK | MB_ICONSTOP | MB_SETFOREGROUND);
  free(buf);
}
#endif

/****************************************************************************/
/*                      Printing of GetLastError's                          */
/****************************************************************************/
#ifndef printLastError
void printLastError(TCHAR *prefix)
{ LPVOID lpMsgBuf;
  DWORD lastError;

  lastError = GetLastError();
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |  FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &lpMsgBuf, 0, NULL );
  warnPrintf(TEXT("%s (%d) -- %s\n"), prefix, lastError, lpMsgBuf);
  LocalFree( lpMsgBuf );
}
#endif

/****************************************************************************/
/*                      Multithreading support                              */
/****************************************************************************/

/* Functions should return 0 if operation failed */

sqInt ioCreateMutex(sqInt initialOwner)
{ sqInt result;
  result = (sqInt) CreateMutex(0,(BOOL)initialOwner, 0);
  dprintf(("ioCreateMutex ^ %x\n", result));
  return result;
}

sqInt ioMutexLock(sqInt mutexHandle)
{ DWORD result;
//  dprintf(("[%x] locking mutex %x ..\n", GetCurrentThreadId(), mutexHandle));
  result = WaitForSingleObject((HANDLE)mutexHandle, INFINITE);
//  dprintf(("[%x] mutex %x lock: %s\n", GetCurrentThreadId(), mutexHandle, (result == WAIT_OBJECT_0) ? "ok" : "failed")); fflush(0);
  return result == WAIT_OBJECT_0;
}

sqInt ioMutexUnlock(sqInt mutexHandle)
{
//  dprintf(("[%x] mutex %x released \n", GetCurrentThreadId(), mutexHandle));
  return ReleaseMutex((HANDLE)mutexHandle);
}

sqInt ioDeleteMutex(sqInt mutexHandle)
{
  return CloseHandle((HANDLE)mutexHandle);
}

sqInt ioMutexWaitmilliseconds(sqInt mutexHandle, sqInt milliseconds)
{ DWORD result;
  
  result = WaitForSingleObject((HANDLE)mutexHandle, (DWORD) milliseconds);
  return result == WAIT_OBJECT_0;
}

sqInt ioCreateThreadForparamsuspended(void * fn, void * param, sqInt suspended)
{
  HANDLE thread;

  thread =
    CreateThread(NULL,                    /* No security descriptor */
                 0,                       /* default stack size     */
                 (ThreadFunction)fn,	  /* what to do */
                 param,					  /* parameter for thread   */
				 (suspended)? CREATE_SUSPENDED : 0,        /* creation parameter -- create suspended so we can check the return value */
                 0);                    /* return value for thread id */

  return (sqInt) thread;
}

sqInt ioGetCurrentThread()
{
	return (sqInt) GetCurrentThreadId();
}

sqInt ioResumeThread(sqInt threadHandle)
{
	DWORD result;
	result = ResumeThread((HANDLE)threadHandle);
	return (result != ((DWORD)-1));
}


/****************************************************************************/
/*                      Atomic event queue functions                        */
/****************************************************************************/
/* 
   Note to non-windows porters: on x86 architecture, atomic CAS is 'lock cmpxchg'.
*/
static HANDLE _hMutex = 0;

#define AtomicCAS(value_ptr, new_value, comparand) InterlockedCompareExchange(value_ptr, new_value, comparand)

void ioInitEventQueue(struct vmEventQueue * queue)
{
//	if (_hMutex == 0 )
//		 _hMutex = CreateMutex(NULL, 0, NULL);

#if 0
	queue->_dummy.next = 0;
	queue->head = &queue->_dummy;
	queue->tail = &queue->_dummy;
#endif

	queue->tail = queue->head = 0;
	queue->osSpecific = (void*) CreateMutex(0,0,0);
}

void ioDestroyEventQueue (struct vmEventQueue * queue)
{
	/* should we check that queue not empty? */
	queue->tail = queue->head = 0;
	CloseHandle((HANDLE) queue->osSpecific);
}

void ioEnqueueEventInto(struct vmEvent * event , struct vmEventQueue * queue)
{
#if 0
	struct vmEvent * tail;
	struct vmEvent * old_tail;
	
	/* add event to tail */
	event->next = 0;
	tail = queue->tail;
	do {
		old_tail = (struct vmEvent *)AtomicCAS(&tail->next, event, 0);
		if (old_tail != 0)
			tail = old_tail;
	} while (old_tail != 0);
	AtomicCAS(&queue->tail, event, tail);
#endif

	WaitForSingleObject((HANDLE)queue->osSpecific,INFINITE);
	event->next = 0;
	if (!queue->head)
	{
		queue->head = queue->tail = event;
		ReleaseMutex((HANDLE)queue->osSpecific);
		return;
	}
	queue->tail->next = event;
	queue->tail = event;
	ReleaseMutex((HANDLE)queue->osSpecific);
}

struct vmEvent * ioDequeueEventFrom(struct vmEventQueue * queue)
{
#if 0
	struct vmEvent * event;
	struct vmEvent * next;
	
loop:
	event = queue->head;
	if (!event->next)
	{
		if (event == &queue->_dummy)
			return 0; // queue is empty
		AtomicCAS(&event->next, &queue->_dummy, 0);
		AtomicCAS(&queue->tail, &queue->_dummy, event);
	}

	while ( event != AtomicCAS(&queue->head, event->next, event)) { event = queue->head; };
	if (event == &queue->_dummy)
	{
		queue->dummy.next = 0;
		goto loop;
	}
	return event;
#endif

	struct vmEvent * event;
	WaitForSingleObject((HANDLE)queue->osSpecific,INFINITE);
	event = queue->head;
	if (event)
	{
		queue->head = event->next;
		if (!queue->head)
			queue->tail = 0;
	}
	ReleaseMutex((HANDLE)queue->osSpecific);
	return event;
}

sqInt ioIsQueueEmpty(struct vmEventQueue * queue)
{
#if 0
	return (queue->head == &queue->_dummy && queue->head->next == 0);
#endif
	return queue->head == 0;
}
