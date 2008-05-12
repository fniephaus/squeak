/*
 *  hydramacintoshextra.c
 *  SqueakHydraForCarbon
 *
 *  Created by John M McIntosh on 2/23/08.
 *  Copyright 2008 Corporate Smalltalk Consulting Ltd. All rights reserved.
 *
 */
#include "sq.h" 
#include "hydramacintoshextra.h"
#include "sqMacTime.h"
#include "sqMacUIConstants.h"
#include <pthread.h>

int threadedInterpretFn(void * param);
static sqInt eventQueueLock=0;

void * ioGetThreadedInterpretFunctionPointer(void)
{
	return (void*)threadedInterpretFn;
}

sqInt ioCreateThreadForparamsuspended(void * fn, void * param, sqInt suspended) {
	pthread_t * thread = malloc(sizeof(pthread_t));
	int rc;
	
	if (suspended) 
		rc = pthread_create_suspended_np(thread, NULL, fn,param);
	else
		rc = pthread_create(thread, NULL, fn,param);
	if (rc) return 0;
	return (sqInt) thread;
}

sqInt ioGetCurrentThread(void){
	return (sqInt) pthread_self();
}

sqInt ioCreateMutex(sqInt initialOwner) { 
	return ioCreateMutexWithRecursion(initialOwner,true);
}
		
sqInt ioCreateMutexWithRecursion(sqInt initialOwner,bool recursive) { 
	/*  apparently mutex has to be recursive and locked or not locked */

	static pthread_mutexattr_t recursiveThing;
	static int recursiveThingInit=0;
	pthread_mutex_t* mutex = malloc(sizeof(pthread_mutex_t));
	int rc;
	
	if (recursiveThingInit == 0) {
		rc = pthread_mutexattr_init(&recursiveThing);
		if (rc) {
			return 0;
		}
		pthread_mutexattr_settype(&recursiveThing,PTHREAD_MUTEX_RECURSIVE);
		recursiveThingInit = 1;
	}
	if (recursive) 
		rc = pthread_mutex_init(mutex, &recursiveThing);
	else 
		rc = pthread_mutex_init(mutex, NULL);
	if (rc) {
		free(mutex);
		return 0;
	}
	if (initialOwner) 
		ioMutexLock((sqInt) mutex);
		
	return (sqInt) mutex;
}

sqInt ioMutexLock(sqInt mutexHandle) {
	int rc; 
	if (mutexHandle == 0)
		return 0;
	rc = pthread_mutex_lock((pthread_mutex_t *)mutexHandle);
	return (rc == 0);
}

sqInt ioMutexUnlock(sqInt mutexHandle){
	int rc;
	if (mutexHandle == 0)
		return 0;
	rc = pthread_mutex_unlock((pthread_mutex_t *)mutexHandle);
	return (rc == 0);
}

sqInt ioDeleteMutex(sqInt mutexHandle){
	int rc;
	if (mutexHandle == 0)
		return 0;
	rc = pthread_mutex_destroy((pthread_mutex_t *)mutexHandle);
	if (rc) return 0;
	free((void *)mutexHandle);
	return 1;
}

sqInt ioMutexWaitmilliseconds(sqInt mutexHandle, sqInt milliseconds){
	mach_timespec_t tspec;
	int		err;
	
	if (mutexHandle == 0)
		return 0;

	pthread_cond_t  gSleepLockCondition;
	err = pthread_cond_init(&gSleepLockCondition,NULL);
	if(err) return 0;
	
	tspec.tv_sec=  milliseconds / 1000;
    tspec.tv_nsec= (milliseconds % 1000)*1000000;
    
    err = pthread_mutex_lock((pthread_mutex_t*) mutexHandle);
	if(err) goto fail;
    err = pthread_cond_timedwait_relative_np(&gSleepLockCondition,(pthread_mutex_t *)mutexHandle,(const struct timespec *)&tspec);	
	if(err) goto fail;
    err = pthread_mutex_unlock((pthread_mutex_t *)mutexHandle);
	if(err) goto fail;
	pthread_cond_destroy(&gSleepLockCondition);
	return 1;
	fail: 
		pthread_cond_destroy(&gSleepLockCondition);
		return 0;
}

int threadedInterpretFn(void * param)
{
	struct Interpreter * intr = (struct Interpreter *) param;
	printf("New interpreter instance %x is about to start!\n", param); fflush(0);

	if (eventQueueLock == 0) {
		eventQueueLock = ioCreateMutexWithRecursion(0,false);
	}
	
	setInterpreterThread(intr, ioGetCurrentThread());

	printf("Initializing attached states...\n"); fflush(0);
	/* if this is first interpreter instance we should call initializeAllNewAttachedStates() */
	if (intr == MAIN_VM)
	{
		/* THIS IS CALLED IN sqMacMain.c */
	}
	else
	{
		initializeAttachedStates(intr);
	}
	printf("...done\n"); fflush(0);

	ioGetSecureUserDirectory();
	browserPluginInitialiseIfNeeded(); /* don't really think this belongs here */

	lockInterpreter(intr); /* lock interpreter mutex before entering loop */
	while (1)
	{
		interpret(intr);
		// give a chance other threads to do something
		unlockInterpreter(intr);
		lockInterpreter(intr);
		pokeAtTimer(intr);
		handleEvents(intr);
	}

	printf("Interpreter %x thread just exited!\n", param); fflush(0);
}


sqInt ioSqueakImageSize(char* filename)
{
    char cFileName[IMAGE_NAME_SIZE+1];  
	FILE *file;  
	sqInt length;
	
	sqFilenameFromStringOpen(cFileName,(sqInt) filename, strlen(filename));
	file = fopen(cFileName, "r+b");
	if (file == NULL) 
		return 0;
	fseek(file, 0, SEEK_END);
	length = ftello(file);
	fclose(file);
	return length;
}

sqInt ioWakeUp(INTERPRETER_ARG) {};



//   The OSAtomicCompareAndSwap() operations compare oldValue to *theValue, and set *theValue to newValue if
//   the comparison is equal.  The comparison and assignment occur as one atomic operation.				  

bool    OSAtomicCompareAndSwap32Barrier( int32_t __oldValue, int32_t __newValue, volatile int32_t *__theValue );
int32_t AtomicCAS(volatile int32_t *__theValue, int32_t __newValue,int32_t __oldValue);

//int32_t AtomicCAS(volatile int32_t *__theValue, volatile int32_t __newValue,volatile int32_t __oldValue) {
//	volatile int32_t old = *__theValue;
//	OSAtomicCompareAndSwap32Barrier(__oldValue, __newValue,__theValue );
//	return old;
//}

/* These functions are thread and multiprocessor safe.  For each function, there is a version that does
     and another that does not incorporate a memory barrier.  Barriers strictly order memory access on a
     weakly-ordered architecture such as PPC.  All loads and stores executed in sequential program order
     before the barrier will complete before any load or store executed after the barrier.  On a uniproces-sor, uniprocessor,
     sor, the barrier operation is typically a nop.  On a multiprocessor, the barrier can be quite expen-sive. expensive.
     sive.

     Most code will want to use the barrier functions to insure that memory shared between threads is prop-erly properly
     erly synchronized.  For example, if you want to initialize a shared data structure and then atomically
     increment a variable to indicate that the initialization is complete, then you must use OSAtomicIncre-ment32Barrier() OSAtomicIncrement32Barrier()
     ment32Barrier() to ensure that the stores to your data structure complete before the atomic add.  Like-wise, Likewise,
     wise, the consumer of that data structure must use OSAtomicDecrement32Barrier(), in order to ensure
     that their loads of the structure are not executed before the atomic decrement.  On the other hand, if
     you are simply incrementing a global counter, then it is safe and potentially much faster to use
     OSAtomicIncrement32().  If you are unsure which version to use, prefer the barrier variants as they are
     safer.
*/

/*
on windows this is 
#define AtomicCAS(value_ptr, new_value, comparand) InterlockedCompareExchange(value_ptr Destination , new_value Exchange, comparand Comparand) 
Destination
A pointer to the destination value. The sign is ignored.
Exchange
The exchange value. The sign is ignored.
Comparand
The value to compare to Destination. The sign is ignored.
Return Value
The function returns the initial value of the Destination parameter.
Remarks
The function compares the Destination value with the Comparand value. If the Destination value is equal to the Comparand value, the Exchange value is stored in the address specified by Destination. Otherwise, no operation is performed.
The parameters for this function must be aligned on a 32-bit boundary; otherwise, the function will behave unpredictably on multiprocessor x86 systems and any non-x86 systems.
The interlocked functions provide a simple mechanism for synchronizing access to a variable that is shared by multiple threads. This function is atomic with respect to calls to other interlocked functions.
This function is implemented using a compiler intrinsic where possible. For more information, see the header file and _InterlockedCompareExchange.
This function generates a full memory barrier (or fence) to ensure that memory operations are completed in order.
*/

long InterlockedCompareExchange(long *dest, long exch, long comp);

long InterlockedCompareExchange(long *dest, long exch, long comp) {
	long old = *dest;
	if(old==comp) {
		*dest=exch;
	}
	return old;
}


#define AtomicCAS(value_ptr, new_value, comparand) InterlockedCompareExchange(value_ptr, new_value, comparand)


void ioInitEventQueue(struct vmEventQueue * queue)
{
	ioMutexLock(eventQueueLock);
	queue->head.next = 0;
	queue->tail = &queue->head;
	ioMutexUnlock(eventQueueLock);
}

void ioEnqueueEventInto(struct vmEvent * event , struct vmEventQueue * queue)
{
	struct vmEvent * tail;
	struct vmEvent * old_next;
	
	/* add event to tail */
	ioMutexLock(eventQueueLock);
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
	ioMutexUnlock(eventQueueLock);

}

struct vmEvent * ioDequeueEventFrom(struct vmEventQueue * queue)
{
	struct vmEvent * event;
	struct vmEvent * oldhead;

	ioMutexLock(eventQueueLock);
	do {
		event = queue->head.next;
		if (!event) {
			ioMutexUnlock(eventQueueLock);
			return 0;
		}
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
	ioMutexUnlock(eventQueueLock);
	return event;
}
