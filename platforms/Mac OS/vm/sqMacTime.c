/****************************************************************************
*   PROJECT: Mac time and millisecond clock logic 
*   FILE:    sqMacTime.c
*   CONTENT: 
*
*   AUTHOR:  John McIntosh.
*   ADDRESS: 
*   EMAIL:   johnmci@smalltalkconsulting.com
*   RCSID:   $Id$
*
*   NOTES: 
*  Feb 22nd, 2002, JMM moved code into 10 other files, see sqMacMain.c for comments
*  Feb 27th, 2002, JMM a bit of cleanup for carbon event usage
*  Apr 17th, 2002, JMM Use accessors for VM variables.
*  Apr 25th, 2002, JMM low res clock is broken after 0x7FFFFFF
*  3.9.1b2 Oct 4th, 2005 Jmm add MillisecondClockMask
 3.8.11b1 Mar 4th, 2006 JMM refactor, cleanup and add headless support
*	3.8.14b1 Oct	,2006 JMM browser rewrite

*****************************************************************************/
#include "sq.h"
#include "sqMacTime.h"
#include "sqMacUIEvents.h"
#include "interp.h"
#define MillisecondClockMask 536870911

#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "sqaio.h"

void ioScheduleTimerSemaphoreSignalAt(INTERPRETER_ARG_COMMA sqInt tick);
sqInt getNextWakeupTick(INTERPRETER_ARG);
static struct timeval	 startUpTime;
static unsigned int	lowResMSecs= 0;
static TMTask    gTMTask;

#define LOW_RES_TICK_MSECS 16
#define HIGH_RES_TICK_MSECS 2
#define COUNTER_LIMIT LOW_RES_TICK_MSECS/HIGH_RES_TICK_MSECS

static pascal void MyTimerProc(QElemPtr time)
{
    lowResMSecs = ioMicroMSecs();
    PrimeTime((QElemPtr)time, LOW_RES_TICK_MSECS);
    return;
}

void SetUpTimers(void)
{
  /* set up the micro/millisecond clock */
    gettimeofday(&startUpTime, 0);
    
    gTMTask.tmAddr = NewTimerUPP((TimerProcPtr) MyTimerProc);
    gTMTask.tmCount = 0;
    gTMTask.tmWakeUp = 0;
    gTMTask.tmReserved = 0;    
     
    InsXTime((QElemPtr)(&gTMTask.qLink));
    PrimeTime((QElemPtr)&gTMTask.qLink,LOW_RES_TICK_MSECS);
}

int ioLowResMSecs(void)
{
  return lowResMSecs;
}

int ioMicroMSecs(void)
{
  struct timeval now;
  gettimeofday(&now, 0);
  if ((now.tv_usec-= startUpTime.tv_usec) < 0) {
    now.tv_usec+= 1000000;
    now.tv_sec-= 1;
  }
  now.tv_sec-= startUpTime.tv_sec;
  return (now.tv_usec / 1000 + now.tv_sec * 1000);
}

int ioSeconds(void) {
    time_t unixTime;
    
    unixTime = time(0);
    unixTime += localtime(&unixTime)->tm_gmtoff;
    /* Squeak epoch is Jan 1, 1901.  Unix epoch is Jan 1, 1970: 17 leap years
        and 52 non-leap years later than Squeak. */
    return unixTime + ((52*365UL + 17*366UL) * 24*60*60UL);
}

int ioRelinquishProcessorForMicroseconds(INTERPRETER_ARG_COMMA int microSeconds) {
	/* This operation is platform dependent. 	 */
	#pragma unused(microSeconds)

    int	   realTimeToWait,now,wake;
	extern int getNextWakeupTick(INTERPRETER_ARG);
        
    now = (ioMSecs() & MillisecondClockMask);
	wake = getNextWakeupTick(INTERPRETER_PARAM);
    if (wake <= now)
        if (wake == 0) {
            realTimeToWait = 16;
			}
        else {
            return 0;
    }
    else
        realTimeToWait = getNextWakeupTick(INTERPRETER_PARAM) - now; 
            
	aioSleep(realTimeToWait*1000);
	return 0;
}
#undef ioMSecs
//Issue with unix aio.c sept 2003

int ioMSecs() {
    return ioMicroMSecs();
}

time_t convertToSqueakTime(time_t unixTime)
{
#ifdef HAVE_TM_GMTOFF
  unixTime+= localtime(&unixTime)->tm_gmtoff;
#else
# ifdef HAVE_TIMEZONE
  unixTime+= ((daylight) * 60*60) - timezone;
# else
#  error: cannot determine timezone correction
# endif
#endif
  /* Squeak epoch is Jan 1, 1901.  Unix epoch is Jan 1, 1970: 17 leap years
     and 52 non-leap years later than Squeak. */
  return unixTime + ((52*365UL + 17*366UL) * 24*60*60UL);
}

int pokeAtTimer(INTERPRETER_ARG);

int pokeAtTimer(INTERPRETER_ARG) {
	sqInt now = (ioMSecs()) & MillisecondClockMask;
	DECL_MAC_STATE();
	
	
	if ((INTERPRETER_PARAM != MAIN_VM) && (MAC_STATE(lowResMSecs) != ioLowResMSecs())) {
		MAC_STATE(lowResMSecs) = ioLowResMSecs();
		postNoOpEvent(INTERPRETER_PARAM);
	}
	
	if (MAC_STATE(wakeUpTick) != 0) {
		if (now < MAC_STATE(lastTick)) {
			MAC_STATE(wakeUpTick) = (MAC_STATE(wakeUpTick) - MillisecondClockMask) - 1;
		}
		if (now >= MAC_STATE(wakeUpTick)) {

			/* set timer interrupt to 0 for 'no timer' */

			MAC_STATE(wakeUpTick) = 0;
			signalTimerSemaphoreAsync(INTERPRETER_PARAM);
		}
	
	} else {
	}
	MAC_STATE(lastTick) = now;
	return true;
}

void ioScheduleTimerSemaphoreSignalAt(INTERPRETER_ARG_COMMA sqInt tick) {
	DECL_MAC_STATE();
	MAC_STATE(wakeUpTick) = tick  & MillisecondClockMask;
}

sqInt getNextWakeupTick(INTERPRETER_ARG) {
	DECL_MAC_STATE();
	return MAC_STATE(wakeUpTick); 
}

