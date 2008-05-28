/* sqUnixThreads.c -- Multithreading support
 *
 * Author: Matthew Fulmer <tapplek@gmail.com>
 *
 *   Copyright (C) 1996-2008 by Ian Piumarta and other authors/contributors
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
 * Last edited: 2008-03-28 10:05:15 by matthewf
 */

#include "sq.h"

/* Squeak Unix thread implementation:
 *
 * For thread id's, assume pthread_t can be cast directly to and from sqInt
 *
 * No threads can be suspended after they start, but sig decided that
 * threads can be created in a suspended state, which pthreads does not
 * directly support (pthreads does not implement any type of
 * suspend/resume). Perhaps the callers should not assume that threads
 * can be suspended, but oh well.
 *
 * To implement suspending a thread on startup, we create a
 * suspendedStartingThread struct and place it on the suspendList. We
 * start it by calling threadMain, which starts by waiting on the
 * startMutex. When it is unlocked, we remove the struct from the linked
 * list, free it, and begin executing the thread.
*/

/****************************************************************************/
/*                       Suspended thread support                           */
/****************************************************************************/

struct suspendedStartingThread
{
  pthread_t thread;
  void *fn;
  void *param;
  pthread_mutex_t startMutex;
  struct suspendedStartingThread *next;
};

static struct suspendedStartingThread
  *suspendListHead = NULL;

struct suspendedStartingThread *
suspendListPrev(struct suspendedStartingThread *item)
{ struct suspendedStartingThread *prev;

  if (item = suspendListHead) return NULL;
  for (prev = suspendListHead; prev != NULL; prev = prev->next)
  {
    if (prev->next == item) return prev;
  }
  return NULL;
}

struct suspendedStartingThread *
suspendListAdd(struct suspendedStartingThread *item)
{
  item->next = suspendListHead;
  suspendListHead = item;
  return item;
}

struct suspendedStartingThread *
suspendListRemove(struct suspendedStartingThread *item)
{ struct suspendedStartingThread *prev;

  prev = suspendListPrev(item);
  if (prev) prev->next = item->next;
  else suspendListHead;
  return item;
}

void threadMain(void *args)
{ struct suspendedStartingThread *call;
  void *fn;
  void *param;
  pthread_mutex_t *startMutex;

  call = (struct suspendedStartingThread *) args;
  fn = call->fn;
  param = call->fn;
  startMutex = &call->startMutex;

  pthread_mutex_lock(startMutex);
  pthread_mutex_unlock(startMutex);

  pthread_mutex_destroy(startMutex);
  free(call);
  ((void (*)(void *)) fn)(param);
}

struct suspendedStartingThread *
newSuspendedThread(void *fn, void *param)
{ struct suspendedStartingThread *call;
  int status = 0;
  call = (struct suspendedStartingThread *)
    malloc(sizeof(struct suspendedStartingThread));
  if (call == NULL) return NULL;

  status |= pthread_mutex_init(&call->startMutex, NULL);
  status |= pthread_mutex_lock(&call->startMutex);

  call->fn = fn;
  call->param = param;

  status |= pthread_create(&call->thread, NULL, (void *)threadMain, (void *)call);

  if (status == 0) return NULL; /* error */
  else return suspendListAdd(call);
}

struct suspendedStartingThread *
suspendListFind(pthread_t thread)
{ struct suspendedStartingThread *item;

  for (item = suspendListHead; item != NULL; item = item->next)
  {
    if (item->thread == thread) return item;
  }
  return NULL;
}

/****************************************************************************/
/*                      Multithreading support                              */
/****************************************************************************/

/* Functions should return 0 if operation failed */

sqInt ioCreateMutex(sqInt initialOwner)
{ sqInt result;
  int status = 0;
  pthread_mutexattr_t attributes;
  result = (sqInt) malloc(sizeof(pthread_mutex_t));
  status |= result == 0;
  status |= pthread_mutexattr_init(&attributes);
  status |= pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_RECURSIVE_NP);
  status |= pthread_mutex_init((pthread_mutex_t *) result, &attributes);
  status |= pthread_mutexattr_destroy(&attributes);
  dprintf(("ioCreateMutex %s\n", (status == 0) ? "ok" : "failed"));
  return (status == 0) ? result : 0;
}

sqInt ioMutexLock(sqInt mutexHandle)
{ int status;

  status = pthread_mutex_lock((pthread_mutex_t *) mutexHandle);
  dprintf(("ioMutexLock %s\n", (status == 0) ? "ok" : "failed"));
  return status == 0;
}

sqInt ioMutexUnlock(sqInt mutexHandle)
{
  return pthread_mutex_unlock((pthread_mutex_t *) mutexHandle) == 0;
}

sqInt ioDeleteMutex(sqInt mutexHandle)
{ int status;

  status = pthread_mutex_destroy((pthread_mutex_t *) mutexHandle);
  free((pthread_mutex_t *) mutexHandle);
  return status == 0;
}

sqInt ioMutexWaitmilliseconds(sqInt mutexHandle, sqInt milliseconds)
{ struct timespec delay;

  delay.tv_sec  = milliseconds / 1000;
  delay.tv_nsec = milliseconds % 1000 * 1000000;

  return pthread_mutex_timedlock((pthread_mutex_t *)mutexHandle, &delay) == 0;
}

sqInt ioCreateThreadForparamsuspended(void * fn, void * param, sqInt suspended)
{
  if (suspended) {
    struct suspendedStartingThread *call = newSuspendedThread(fn, param);
    if (call == NULL) return 0;
    else return (sqInt) call->thread;
  } else {
    pthread_t result;
    int status = 0;
    status = pthread_create(&result, NULL, fn, param);
    dprintf(("ioCreateThread %s\n", (status == 0) ? "ok" : "failed"));
    if (status == 0) return (sqInt)result;
    else return 0;
  }
}

sqInt ioGetCurrentThread()
{
  return (sqInt) pthread_self();
}

sqInt ioResumeThread(sqInt threadHandle)
{ struct suspendedStartingThread * call;
  call = suspendListFind((pthread_t) threadHandle);
  if (call == NULL) return 0;
  suspendListRemove(call);
  return pthread_mutex_unlock(&call->startMutex) == 0;
}

/****************************************************************************/
/*                      Atomic event queue functions                        */
/****************************************************************************/
/*
   Note to non-windows porters: on x86 architecture, atomic CAS is 'lock cmpxchg'.
*/

#define AtomicCAS(value_ptr, new_value, comparand) InterlockedCompareExchange(value_ptr, new_value, comparand)

void ioInitEventQueue(struct vmEventQueue * queue)
{
	queue->head.next = 0;
	queue->tail = &queue->head;
}

void ioEnqueueEventInto(struct vmEvent * event , struct vmEventQueue * queue)
{
	struct vmEvent * tail;
	struct vmEvent * old_next;

	/* add event to tail */
	event->next = 0;
	do {
		tail = queue->tail;
		old_next = (struct vmEvent *)AtomicCAS(&tail->next, event, 0);
		if (old_next != 0)
		{
			AtomicCAS(&queue->tail, old_next, tail);
		}
	} while (old_next != 0);
	AtomicCAS(&queue->tail, event, tail);
}

struct vmEvent * ioDequeueEventFrom(struct vmEventQueue * queue)
{
	struct vmEvent * event;
	struct vmEvent * oldhead;
	do {
		event = queue->head.next;
		if (!event)
			return 0;
		oldhead = (struct vmEvent *)AtomicCAS(&queue->head.next, event->next, event);
	} while (oldhead != event);

	/* To prevent queue damage, when queue tail points to just dequeued event,
		we should replace tail with head */
	if (event->next == 0)
	{
		/* queue->head.next should be 0, put it as tail */
		if ( 0 == AtomicCAS(&event->next, &queue->head, 0))
		{
			AtomicCAS(&queue->tail, &queue->head, event);
		}
	}
	return event;
}

