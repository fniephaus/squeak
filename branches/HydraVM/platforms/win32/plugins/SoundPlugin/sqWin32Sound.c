/****************************************************************************
*   PROJECT: Squeak port for Win32 (NT / Win95)
*   FILE:    sqWin32Sound.c
*   CONTENT: Sound management
*
*   AUTHOR:  Andreas Raab (ar)
*   ADDRESS: University of Magdeburg, Germany
*   EMAIL:   raab@isg.cs.uni-magdeburg.de
*   RCSID:   $Id$
*
*   NOTES:   For now we're supporting both, the DirectSound and the win32
*            based interface. In the future we'll switch to DSound exclusively.
*
*****************************************************************************/
#include <windows.h>
#include <mmsystem.h>
#include "SoundPlugin_imports.h"
extern BOOL  fUseDirectSound;/* Do we use DirectSound?! */

#ifdef DEBUG
#define DPRINTF(x) warnPrintf x
#else
#define DPRINTF(x)
#endif

#ifndef NO_SOUND

#ifndef NO_RCSID
  static char RCSID[]="$Id$";
#endif

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
#ifndef NO_DIRECT_SOUND
/* The new DirectSound based implementation of the sound primitives */
#define _LPCWAVEFORMATEX_DEFINED
#undef WINNT
#define DIRECTSOUND_VERSION 0x0600	/* use DirectSound 6.0 */
#ifdef __MINGW32__
#define HMONITOR_DECLARED
#endif
#include <dsound.h>

 /* HACK! Define LPWAVEFORMAT as LPWAVEFORMATEX for WinVER >= 4 */
#if WINVER >= 0x0400
#define LPWAVEFORMAT LPWAVEFORMATEX
#endif

#endif

typedef struct SndPluginState {

	BOOL  fUseDirectSound; /* Do we use DirectSound?! */
#ifndef NO_DIRECT_SOUND
	int fUsingDirectSound; /* when we're in direct sound */

	LPDIRECTSOUND lpdSound;
	LPDIRECTSOUNDBUFFER lpdPrimaryBuffer;
	LPDIRECTSOUNDBUFFER lpdPlayBuffer;
	WAVEFORMATEX waveOutFormat;
	HANDLE hPlayEvent;
	HANDLE hPlayThread;
	int playBufferSize;
	int playBufferIndex;
	int playBufferAvailable;
	int playTerminate;
	int playSemaphore;

	LPDIRECTSOUNDCAPTURE lpdCapture;
	LPDIRECTSOUNDCAPTUREBUFFER lpdRecBuffer;
	WAVEFORMATEX waveInFormat;
	HANDLE hRecEvent;
	HANDLE hRecThread;
	int recBufferSize;
	int recBufferFrameCount;
	int recBufferIndex;
	int recBufferPosition;
	int recBufferAvailable;
	int recTerminate;
	int recSemaphore;

#endif 
	/* number of buffers used */
	#define NUM_BUFFERS 4
	/* initial buffer size */
	#define BUFFER_SIZE 4096

	/* Note: NUM_BUFFERS and BUFFER_SIZE should be carefully chosen.
	   You better not touch it unless you know what you're doing, but
	   ALWAYS use a multiple of 2 for NUM_BUFFERS. BUFFER_SIZE affects 
	   only sound input. The output buffer size is determined by Squeak.
	*/

	HWAVEOUT hWaveOut;
	HWAVEIN hWaveIn;

	HANDLE win32SyncMutex;

	WAVEOUTCAPS outCaps;
	int outBufferSize;
	int outSemaIndex;

	WAVEHDR * playBuffers[NUM_BUFFERS];
	WAVEHDR * recordBuffers[NUM_BUFFERS];

	int currentRecordBuffer; /* Index of the buffer currently recorded */
	int currentPlayBuffer;   /* Index of the buffer currently played */
	int playingStarted;
	int lastPlayBuffer;

} SndPluginState;

static int sndStateId = 0;

#define DECL_PLUGIN_STATE() SndPluginState * pstate = vmFunction(getAttachedStateBufferof)(sndStateId, _iparam())
#define SND_STATE(name) (pstate->name)

void sndAttachedStateInit (PInterpreter intr)
{
	SndPluginState * pstate = vmFunction(getAttachedStateBufferof)(sndStateId, intr);
	SND_STATE(fUseDirectSound) = fUseDirectSound;
#ifndef NO_DIRECT_SOUND
	SND_STATE(fUsingDirectSound) = 0;
	SND_STATE(lpdSound) = NULL;
	SND_STATE(lpdPrimaryBuffer) = NULL;
	SND_STATE(lpdPlayBuffer) = NULL;
	SND_STATE(hRecEvent) = CreateEvent( NULL, FALSE, FALSE, NULL );
	SND_STATE(hPlayEvent) = CreateEvent( NULL, FALSE, FALSE, NULL );
	SND_STATE(hPlayThread) = NULL;
	SND_STATE(playBufferSize) = 0;
	SND_STATE(playBufferIndex) = 0;
	SND_STATE(playBufferAvailable) = 0;
	SND_STATE(playTerminate) = 0;
	SND_STATE(playSemaphore) = -1;
	SND_STATE(lpdCapture) = NULL;
	SND_STATE(lpdRecBuffer) = NULL;
	SND_STATE(hRecThread) = NULL;
	SND_STATE(recBufferSize) = 0;
	SND_STATE(recBufferFrameCount) = 1024;
	SND_STATE(recBufferIndex) = 0;
	SND_STATE(recBufferPosition) = 0;
	SND_STATE(recBufferAvailable) = 0;
	SND_STATE(recTerminate) = 0;
	SND_STATE(recSemaphore) = -1;

#endif
	SND_STATE(win32SyncMutex) = CreateMutex(NULL, 0, NULL);

	SND_STATE(hWaveOut) = 0;
	SND_STATE(hWaveIn) = 0;
	SND_STATE(outBufferSize) = 0;
	SND_STATE(outSemaIndex) = -1;
	SND_STATE(currentRecordBuffer) = 0;
	SND_STATE(currentPlayBuffer) = 0;
	SND_STATE(playingStarted) = 0;
	SND_STATE(lastPlayBuffer) = -1;
}

void sndAttachedStateShutdown (PInterpreter intr) 
{
	SndPluginState * pstate = vmFunction(getAttachedStateBufferof)(sndStateId, intr);

#ifndef NO_DIRECT_SOUND
	DPRINTF(("dx_soundShutDown\n"));
	dx_snd_StopPlaying _iparam();
	dx_snd_StopRecording _iparam();
	CloseHandle(SND_STATE(hPlayEvent));
	CloseHandle(SND_STATE(hRecEvent));
#endif
	win32_snd_StopPlaying _iparam();
	win32_snd_StopRecording _iparam();
	CloseHandle(SND_STATE(win32SyncMutex));

}
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/* module initialization/shutdown */
int soundInit(void) {
	sndStateId = vmFunction(attachStateBufferinitializeFnfinalizeFn)(sizeof(SndPluginState),sndAttachedStateInit,sndAttachedStateShutdown);
}

int soundShutdown(void) {
	if (sndStateId)
	{
		vmFunction(releaseAttachedState)(sndStateId);
		sndStateId = 0;
	}
}

#ifndef NO_DIRECT_SOUND

int dx_snd_StopPlaying _iarg() {
  DECL_PLUGIN_STATE();
  SND_STATE(playTerminate) = 0;
  if(SND_STATE(lpdPlayBuffer)) {
    DPRINTF(("Shutting down DSound\n"));
    IDirectSoundBuffer_Stop(SND_STATE(lpdPlayBuffer));
    IDirectSoundBuffer_Release(SND_STATE(lpdPlayBuffer));
    SND_STATE(lpdPlayBuffer) = NULL;
  }
  if(SND_STATE(lpdPrimaryBuffer)) {
    IDirectSoundBuffer_Release(SND_STATE(lpdPrimaryBuffer));
    SND_STATE(lpdPrimaryBuffer) = NULL;
  }
  if(SND_STATE(lpdSound)) {
    IDirectSound_Release(SND_STATE(lpdSound));
    SND_STATE(lpdSound) = NULL;
  }
  if(SND_STATE(hPlayThread)) {
    ResetEvent(SND_STATE(hPlayEvent));
    SND_STATE(playTerminate) = 1;
    SetEvent(SND_STATE(hPlayEvent));
    WaitForSingleObject(SND_STATE(hPlayThread), 100); /* wait until terminated */
    SND_STATE(hPlayThread) = NULL;
    SND_STATE(playTerminate) = 0;
  }
  ResetEvent(SND_STATE(hPlayEvent));
  if(!SND_STATE(lpdCapture)) {
    SND_STATE(fUsingDirectSound) = 0; /* no longer */
  }
  return 1;
}

DWORD WINAPI playCallback( LPVOID interp ) {
  struct Interpreter * intr = (struct Interpreter *) interp;
  DECL_PLUGIN_STATE();
  while(1) {
    if(WaitForSingleObject(SND_STATE(hPlayEvent), INFINITE) == WAIT_OBJECT_0) {
      if(SND_STATE(playTerminate)) {
	SND_STATE(hPlayThread) = NULL;
	DPRINTF(("playCallback shutdown\n"));
	dx_snd_StopPlaying _iparam();
	return 0; /* done playing */
      }
      SND_STATE(playBufferAvailable) = 1;
      SND_STATE(playBufferIndex) = ++SND_STATE(playBufferIndex) & 1;
      synchronizedSignalSemaphoreWithIndex(intr, SND_STATE(playSemaphore));
    }
  }
}

DWORD WINAPI recCallback( LPVOID interp ) {
  struct Interpreter * intr = (struct Interpreter *) interp;
  DECL_PLUGIN_STATE();
  while(1) {
    if(WaitForSingleObject(SND_STATE(hRecEvent), INFINITE) == WAIT_OBJECT_0) {
      if(SND_STATE(recTerminate)) return 0; /* done playing */
      SND_STATE(recBufferAvailable) = 1;
      SND_STATE(recBufferIndex) = ++SND_STATE(recBufferIndex) & 1;
      synchronizedSignalSemaphoreWithIndex(intr, SND_STATE(recSemaphore));
    }
  }
}

/* sound output */
int dx_snd_AvailableSpace _iarg() {
  DECL_PLUGIN_STATE();
  if(SND_STATE(playBufferAvailable)) {
    return SND_STATE(playBufferSize);
  }
  return 0;
}

int dx_snd_InsertSamplesFromLeadTime _iargs(int frameCount, int srcBufPtr, int samplesOfLeadTime)
{
  /* currently not supported */
  return 0;
}

int dx_snd_PlaySamplesFromAtLength _iargs(int frameCount, int arrayIndex, int startIndex)
{
  DECL_PLUGIN_STATE();
  HRESULT hRes;
  int bytesWritten;
  DWORD dstLen;
  void *dstPtr;

  bytesWritten = SND_STATE(waveOutFormat).nBlockAlign  * frameCount;

  if(bytesWritten > SND_STATE(playBufferSize))
    bytesWritten = SND_STATE(playBufferSize);

  if(bytesWritten < SND_STATE(playBufferSize))
    return 0;

  DPRINTF(("[%d", frameCount));

  hRes = IDirectSoundBuffer_Lock(SND_STATE(lpdPlayBuffer), 
				 SND_STATE(playBufferSize) * SND_STATE(playBufferIndex),
				 SND_STATE(playBufferSize), 
				 &dstPtr, &dstLen, 
				 NULL, NULL, 
				 0);
  if(FAILED(hRes)) {
    DPRINTF(("snd_Play: IDirectSoundBuffer_Lock failed (errCode: %x)\n", hRes));
    return 0;
  }
  /* mix in stuff */
  { 
    int i;
    short *shortSrc = (short*)(arrayIndex+startIndex);
    short *shortDst = (short*)dstPtr;
    dstLen /=2;
    DPRINTF(("|%d", dstLen));
    for(i=0;i<dstLen;i++) {
      *shortDst++ = *(shortSrc++);
    }
  }
  IDirectSoundBuffer_Unlock(SND_STATE(lpdPlayBuffer), dstPtr, dstLen, NULL, 0);
  DPRINTF(("]"));
  SND_STATE(playBufferAvailable) = 0;
  return bytesWritten / SND_STATE(waveOutFormat).nBlockAlign;
}

int dx_snd_PlaySilence(void) {
  /* no longer supported */
  return -1;
}

int dx_snd_Start _iargs(int frameCount, int samplesPerSec, int stereo, int semaIndex) {
  DECL_PLUGIN_STATE();
  DSBUFFERDESC dsbd;
  DSBPOSITIONNOTIFY  posNotify[2];
  LPDIRECTSOUNDNOTIFY lpdNotify = NULL;
  HRESULT hRes;
  DWORD threadID;
  int bytesPerFrame;
  int bufferSize;

  /* round up the size of the play buffers to multiple of 16 bytes*/
  bytesPerFrame = stereo ? 4 : 2;
  bufferSize = ((bytesPerFrame * frameCount) / 16) * 16;
  if((bufferSize != SND_STATE(playBufferSize)) || 
     (samplesPerSec != SND_STATE(waveOutFormat).nSamplesPerSec) || 
     ((stereo == 0) != (SND_STATE(waveOutFormat).nChannels == 1))) {
    /* format change */
    DPRINTF(("DXSound format change (%d, %d, %s)\n", frameCount, samplesPerSec, (stereo ? "stereo" : "mono")));
    dx_snd_StopPlaying _iparam();
  }

  if(SND_STATE(lpdPlayBuffer)) {
    /* keep playing */
    SND_STATE(playTerminate) = 0;
    SND_STATE(playSemaphore) = semaIndex; /* might have changed */
    DPRINTF(("Continuing DSound\n"));
    return 1;
  }

  DPRINTF(("Starting DSound\n"));
  if(!SND_STATE(lpdSound)) {
    /* Initialize DirectSound */
    DPRINTF(("# Creating SND_STATE(lpdSound)\n"));
    hRes = CoCreateInstance(&CLSID_DirectSound,
			    NULL, 
			    CLSCTX_INPROC_SERVER,
			    &IID_IDirectSound,
			    (void**)&SND_STATE(lpdSound));
    if(FAILED(hRes)) {
      /* Well, well. If we can't create a DSound then we're
	 very likely to be on a non-DSound system (like NT3). */
      DPRINTF(("sndStart: CoCreateInstance() failed (errCode: %x)\n", hRes));
      DPRINTF(("Turning off DirectSound\n"));
      SND_STATE(fUseDirectSound) = 0;
      /* and try again */
      return snd_Start _iparams(frameCount, samplesPerSec, stereo, semaIndex);
    }
    DPRINTF(("# Initializing SND_STATE(lpdSound)\n"));
    hRes = IDirectSound_Initialize(SND_STATE(lpdSound), NULL);
    if(FAILED(hRes)) {
      DPRINTF(("sndStart: IDirectSound_Initialize() failed (errCode: %x)\n", hRes));
      IDirectSound_Release(SND_STATE(lpdSound));
      SND_STATE(lpdSound) = NULL;
      DPRINTF(("Turning off DirectSound\n"));
      SND_STATE(fUseDirectSound) = 0;
      /* and try again */
      return snd_Start _iparams(frameCount, samplesPerSec, stereo, semaIndex);
    }
    /* set the cooperative level (DSSCL_PRIORITY is recommended) */
    hRes = IDirectSound_SetCooperativeLevel(SND_STATE(lpdSound), stWindow, DSSCL_PRIORITY);
    if(FAILED(hRes)) {
      DPRINTF(("sndStart: IDirectSound_SetCooperativeLevel failed (errCode: %x)\n", hRes));
      /* for now don't fail because of lack in cooperation */
    }
    /* grab the primary sound buffer for handling format changes */
    ZeroMemory(&dsbd, sizeof(dsbd));
    dsbd.dwSize = sizeof(dsbd);
    dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
    DPRINTF(("# Creating primary buffer\n"));
    hRes = IDirectSound_CreateSoundBuffer(SND_STATE(lpdSound), &dsbd, &SND_STATE(lpdPrimaryBuffer), NULL);
    if(FAILED(hRes)) {
      DPRINTF(("sndStart: Failed to create primary buffer (errCode: %x)\n", hRes));
    }
  }

  SND_STATE(fUsingDirectSound) = 1; /* we are */
  SND_STATE(playSemaphore) = semaIndex;

  if(!SND_STATE(hPlayThread)) {
    /* create the playback notification thread */
    DPRINTF(("# Creating playback thread\n"));
    SND_STATE(hPlayThread) = CreateThread(NULL, 0, playCallback, _iparam(), 0, &threadID);
    if(SND_STATE(hPlayThread) == 0) {
      printLastError("sndStart: CreateThread failed");
      dx_snd_StopPlaying _iparam();
      DPRINTF(("Turning off DirectSound\n"));
      SND_STATE(fUseDirectSound) = 0;
      /* and try again */
      return snd_Start _iparams(frameCount, samplesPerSec, stereo, semaIndex);
    }
    if(!SetThreadPriority(SND_STATE(hPlayThread), THREAD_PRIORITY_HIGHEST))
      printLastError(TEXT("SetThreadPriority() failed"));
  }

  /* since we start from buffer 0 the first play index is one */
  SND_STATE(playBufferIndex) = 1; 

  /* round up the size of the play buffers to multiple of 16 bytes*/
  bytesPerFrame = stereo ? 4 : 2;
  SND_STATE(playBufferSize) = ((bytesPerFrame * frameCount) / 16) * 16;

  /* create the secondary sound buffer */
  dsbd.dwSize = sizeof(dsbd);
  dsbd.dwFlags = 
    DSBCAPS_CTRLPOSITIONNOTIFY |   /* position notification */
    DSBCAPS_GETCURRENTPOSITION2 |  /* accurate positioning */
    DSBCAPS_LOCSOFTWARE |          /* software buffers please */
    DSBCAPS_GLOBALFOCUS;           /* continue playing */
  dsbd.dwBufferBytes = 2 * SND_STATE(playBufferSize);
  dsbd.dwReserved = 0;
    SND_STATE(waveOutFormat).wFormatTag = WAVE_FORMAT_PCM;
    SND_STATE(waveOutFormat).nChannels = stereo ? 2 : 1;
    SND_STATE(waveOutFormat).nSamplesPerSec = samplesPerSec;
    SND_STATE(waveOutFormat).nAvgBytesPerSec = samplesPerSec * bytesPerFrame;
    SND_STATE(waveOutFormat).nBlockAlign = bytesPerFrame;
    SND_STATE(waveOutFormat).wBitsPerSample = 16;
  dsbd.lpwfxFormat = &SND_STATE(waveOutFormat);

  /* set the primary buffer format */
  if(SND_STATE(lpdPrimaryBuffer)) {
    hRes = IDirectSoundBuffer_SetFormat(SND_STATE(lpdPrimaryBuffer), &SND_STATE(waveOutFormat));
    if(FAILED(hRes)) {
      DPRINTF(("sndStart: Failed to set primary buffer format (errCode: %x)\n", hRes));
    }
  }

  DPRINTF(("# Creating play buffer\n"));
  hRes = IDirectSound_CreateSoundBuffer(SND_STATE(lpdSound), &dsbd, &SND_STATE(lpdPlayBuffer), NULL);
  if(FAILED(hRes)) {
    DPRINTF(("sndStart: IDirectSound_CreateSoundBuffer() failed (errCode: %x)\n", hRes));
    dx_snd_StopPlaying _iparam();
    DPRINTF(("Turning off DirectSound\n"));
    SND_STATE(fUseDirectSound) = 0;
    /* and try again */
    return snd_Start _iparams(frameCount, samplesPerSec, stereo, semaIndex);
  }

  /* setup notifications */
  hRes = IDirectSoundBuffer_QueryInterface(SND_STATE(lpdPlayBuffer),
					   &IID_IDirectSoundNotify, 
					   (void**)&lpdNotify );
  if(FAILED(hRes)) {
    DPRINTF(("sndStart: QueryInterface(IDirectSoundNotify) failed (errCode: %x)\n"));
    dx_snd_StopPlaying _iparam();
    DPRINTF(("Turning off DirectSound\n"));
    SND_STATE(fUseDirectSound) = 0;
    /* and try again */
    return snd_Start _iparams(frameCount, samplesPerSec, stereo, semaIndex);
  }
  posNotify[0].dwOffset = 1 * SND_STATE(playBufferSize) - 1;
  posNotify[1].dwOffset = 2 * SND_STATE(playBufferSize) - 1;
  posNotify[0].hEventNotify = SND_STATE(hPlayEvent);
  posNotify[1].hEventNotify = SND_STATE(hPlayEvent);
  DPRINTF(("# Setting notifications\n"));
  hRes = IDirectSoundNotify_SetNotificationPositions(lpdNotify, 2, posNotify);
  IDirectSoundNotify_Release(lpdNotify);
  if(FAILED(hRes)) {
    DPRINTF(("sndStart: IDirectSoundNotify_SetNotificationPositions() failed (errCode: %x)\n", hRes));
    dx_snd_StopPlaying _iparam();
    DPRINTF(("Turning off DirectSound\n"));
    SND_STATE(fUseDirectSound) = 0;
    /* and try again */
    return snd_Start _iparams(frameCount, samplesPerSec, stereo, semaIndex);
  }

  DPRINTF(("# Starting to play buffer\n"));
  hRes = IDirectSoundBuffer_Play(SND_STATE(lpdPlayBuffer), 0, 0, DSBPLAY_LOOPING);
  if(FAILED(hRes)) {
    DPRINTF(("sndStart: IDirectSoundBuffer_Play() failed (errCode: %x)\n", hRes));
    dx_snd_StopPlaying _iparam();
    DPRINTF(("Turning off DirectSound\n"));
    SND_STATE(fUseDirectSound) = 0;
    /* and try again */
    return snd_Start _iparams(frameCount, samplesPerSec, stereo, semaIndex);
  }
  return 1;
}

int dx_snd_Stop _iarg() {
  dx_snd_StopPlaying _iparam();
  return 1;
}

/* sound input */
int dx_snd_SetRecordLevel(int level) {
  /* not supported */
  return 0;
}

int dx_snd_StartRecording _iargs(int samplesPerSec, int stereo, int semaIndex) {
  DECL_PLUGIN_STATE();
  DSCBUFFERDESC dscb;
  DSBPOSITIONNOTIFY  posNotify[2];
  LPDIRECTSOUNDNOTIFY lpdNotify = NULL;
  HRESULT hRes;
  DWORD threadID;
  int bytesPerFrame;

  if(SND_STATE(lpdRecBuffer)) {
    snd_StopRecording _iparam();
  }

  if(!SND_STATE(lpdCapture)) {
    hRes = CoCreateInstance(&CLSID_DirectSoundCapture,
			    NULL, 
			    CLSCTX_INPROC_SERVER,
			    &IID_IDirectSoundCapture,
			    (void**)&SND_STATE(lpdCapture));
    if(FAILED(hRes)) {
      DPRINTF(("snd_StartRec: CoCreateInstance() failed (errCode: %x)\n", hRes));
      return 0;
    }
    hRes = IDirectSoundCapture_Initialize(SND_STATE(lpdCapture), NULL);
    if(FAILED(hRes)) {
      DPRINTF(("snd_StartRec: IDirectSoundCapture_Initialize() failed (errCode: %x)\n", hRes));
      SND_STATE(lpdCapture) = NULL;
      return 0;
    }
  }

  SND_STATE(fUsingDirectSound) = 1; /* we are */

  /* create the recording notification thread */
  SND_STATE(recSemaphore) = semaIndex;
  SND_STATE(hRecThread) = CreateThread(NULL, 0, recCallback, _iparam(), 0, &threadID);
  if(SND_STATE(hRecThread) == 0) {
    printLastError("snd_StartRec: CreateThread failed");
    snd_StopRecording _iparam();
    return 0;
  }
  if(!SetThreadPriority(SND_STATE(hRecThread), THREAD_PRIORITY_HIGHEST))
    printLastError(TEXT("SetThreadPriority() failed"));

  /* since we start from buffer 0 the first recording index is one */
  SND_STATE(recBufferIndex) = 1;

  /* round up the size of the record buffers to multiple of 16 bytes*/
  bytesPerFrame = stereo ? 4 : 2;
  SND_STATE(recBufferSize) = ((bytesPerFrame * SND_STATE(recBufferFrameCount)+ 15) / 16) * 16;

  /* create the secondary sound buffer */
  dscb.dwSize = sizeof(dscb);
  dscb.dwFlags = DSCBCAPS_WAVEMAPPED;
  dscb.dwBufferBytes = 2 * SND_STATE(recBufferSize);
  dscb.dwReserved = 0;
    SND_STATE(waveInFormat).wFormatTag = WAVE_FORMAT_PCM;
    SND_STATE(waveInFormat).nChannels = stereo ? 2 : 1;
    SND_STATE(waveInFormat).nSamplesPerSec = samplesPerSec;
    SND_STATE(waveInFormat).nAvgBytesPerSec = samplesPerSec * bytesPerFrame;
    SND_STATE(waveInFormat).nBlockAlign = bytesPerFrame;
    SND_STATE(waveInFormat).wBitsPerSample = 16;
  dscb.lpwfxFormat = &SND_STATE(waveInFormat);
  hRes = IDirectSoundCapture_CreateCaptureBuffer(SND_STATE(lpdCapture), &dscb, &SND_STATE(lpdRecBuffer), NULL);
  if(FAILED(hRes)) {
    DPRINTF(("snd_StartRec: IDirectSoundCapture_CreateCaptureBuffer() failed (errCode: %x)\n", hRes));
    return 0;
  }
  hRes = IDirectSoundCaptureBuffer_QueryInterface(SND_STATE(lpdRecBuffer),
						  &IID_IDirectSoundNotify, 
						  (void**)&lpdNotify );
  if(FAILED(hRes)) {
    DPRINTF(("snd_StartRec: QueryInterface(IDirectSoundNotify) failed (errCode: %x)\n"));
    snd_StopRecording _iparam();
    return 0;
  }
  posNotify[0].dwOffset = 1 * SND_STATE(recBufferSize) - 1;
  posNotify[1].dwOffset = 2 * SND_STATE(recBufferSize) - 1;
  posNotify[0].hEventNotify = SND_STATE(hRecEvent);
  posNotify[1].hEventNotify = SND_STATE(hRecEvent);
  hRes = IDirectSoundNotify_SetNotificationPositions(lpdNotify, 2, posNotify);
  IDirectSoundNotify_Release(lpdNotify);
  if(FAILED(hRes)) {
    DPRINTF(("snd_StartRec: IDirectSoundNotify_SetNotificationPositions() failed (errCode: %x)\n", hRes));
    snd_StopRecording _iparam();
    return 0;
  }
  hRes = IDirectSoundCaptureBuffer_Start(SND_STATE(lpdRecBuffer), DSCBSTART_LOOPING);
  if(FAILED(hRes)) {
    DPRINTF(("snd_StartRec: IDirectSoundCaptureBuffer_Start() failed (errCode: %x)\n", hRes));
    snd_StopRecording _iparam();
    return 0;
  }

  return 0;
}

int dx_snd_StopRecording _iarg() {
  DECL_PLUGIN_STATE();
  if(SND_STATE(lpdRecBuffer)) {
    IDirectSoundCaptureBuffer_Stop(SND_STATE(lpdRecBuffer));
    IDirectSoundCaptureBuffer_Release(SND_STATE(lpdRecBuffer));
    SND_STATE(lpdRecBuffer) = NULL;
  }
  if(SND_STATE(lpdCapture)) {
    IDirectSoundCapture_Release(SND_STATE(lpdCapture));
    SND_STATE(lpdCapture) = NULL;
  }
  if(SND_STATE(hRecThread)) {
    ResetEvent(SND_STATE(hRecEvent));
    SND_STATE(recTerminate) = 1;
    SetEvent(SND_STATE(hRecEvent));
    WaitForSingleObject(SND_STATE(hRecThread), 100); /* wait until terminated */
    SND_STATE(hRecThread) = NULL;
    SND_STATE(recTerminate) = 0;
  }
  ResetEvent(SND_STATE(hRecEvent));
  if(!SND_STATE(lpdSound)) {
    SND_STATE(fUsingDirectSound) = 0; /* no longer */
  }
  return 0;
}

double dx_snd_GetRecordingSampleRate _iarg() {
  DECL_PLUGIN_STATE();
  if(!SND_STATE(lpdRecBuffer)) return 0.0;
  return (double) SND_STATE(waveInFormat).nSamplesPerSec;
}

int dx_snd_RecordSamplesIntoAtLength _iargs(int buf, int startSliceIndex, int bufferSizeInBytes) {
  /* if data is available, copy as many sample slices as possible into the
     given buffer starting at the given slice index. do not write past the
     end of the buffer, which is buf + bufferSizeInBytes. return the number
     of slices (not bytes) copied. a slice is one 16-bit sample in mono
     or two 16-bit samples in stereo. */
  DECL_PLUGIN_STATE();
  int bytesPerSlice = (SND_STATE(waveInFormat).nChannels * 2);
  int bytesCopied;
  char *srcPtr, *dstPtr;
  HRESULT hRes;
  DWORD srcLen;

  if(!SND_STATE(lpdRecBuffer)) {
    /* not recording */
    return 0;
  }
  if(!SND_STATE(recBufferAvailable)) {
    /* no data available */
    return 0;
  }
  bytesCopied = bufferSizeInBytes - (startSliceIndex * bytesPerSlice);
  if(bytesCopied > (SND_STATE(recBufferSize) - SND_STATE(recBufferPosition))) 
    bytesCopied = SND_STATE(recBufferSize) - SND_STATE(recBufferPosition);

  hRes = IDirectSoundCaptureBuffer_Lock(SND_STATE(lpdRecBuffer), 
					(SND_STATE(recBufferSize) * SND_STATE(recBufferIndex)),
					SND_STATE(recBufferSize),
					(void*)&srcPtr, &srcLen,
					NULL, NULL,
					0);
  if(FAILED(hRes)) {
    DPRINTF(("snd_Rec: IDirectSoundCaptureBuffer_Lock() failed (errCode: %x)\n", hRes));
    return 0;
  }

  dstPtr = (char*)(buf + startSliceIndex * bytesPerSlice);
  memcpy(dstPtr, srcPtr+SND_STATE(recBufferPosition), bytesCopied);
  SND_STATE(recBufferPosition) = (SND_STATE(recBufferPosition) + bytesCopied) % SND_STATE(recBufferSize);
  if(SND_STATE(recBufferPosition) == 0) {
    SND_STATE(recBufferAvailable) = 0;
  }
  hRes = IDirectSoundCaptureBuffer_Unlock(SND_STATE(lpdRecBuffer),
					  srcPtr, srcLen,
					  NULL, 0);
  if(FAILED(hRes)) {
    DPRINTF(("snd_Rec: IDirectSoundCaptureBuffer_Unlock() failed (errCode: %x)\n", hRes));
  }

  return bytesCopied / bytesPerSlice;
}


/* NOTE: Both of the below functions use the default wave out device */
void dx_snd_Volume(double *left, double *right)
{
  DWORD volume = (DWORD)-1;

  waveOutGetVolume((HWAVEOUT)0, &volume);
  *left = (volume & 0xFFFF) / 65535.0;
  *right = (volume >> 16) / 65535.0;
}

void dx_snd_SetVolume(double left, double right)
{
  DWORD volume;

  if(left < 0.0) left = 0.0;
  if(left > 1.0) left = 1.0;
  volume = (int)(left * 0xFFFF);
  if(right < 0.0) right = 0.0;
  if(right > 1.0) right = 1.0;
  volume |= ((int)(right *0xFFFF)) << 16;

  waveOutSetVolume((HWAVEOUT) 0, volume);
}

#endif /* NO_DIRECT_SOUND */

/***************************************************************************/
/***************************************************************************/


/* Initialize wave headers */
static void
initWaveHeaders(WAVEHDR **buffers, DWORD bufferSize)
{ int i;

 for(i=0;i<NUM_BUFFERS;i++)
   {
     (buffers[i]) = GlobalAlloc(GMEM_ZEROINIT | GMEM_FIXED ,sizeof(WAVEHDR));
     (buffers[i])->dwBufferLength = bufferSize;
     (buffers[i])->lpData = GlobalAlloc(GMEM_ZEROINIT | GMEM_FIXED, bufferSize);
     (buffers[i])->dwFlags |= WHDR_DONE;
   }
}

/* Release wave headers */
static void
freeWaveHeaders(WAVEHDR **buffers)
{ int i;

  for(i=0;i<NUM_BUFFERS;i++)
    {
      /* Free data buffer */
      GlobalFree((buffers[i])->lpData);
      /* Free wave header */
      GlobalFree(buffers[i]);
      buffers[i] = 0;
    }
}

/* find the next empty header which can be used for sound output */
static WAVEHDR *
findEmptyHeader _iargs(WAVEHDR **buffers)
{ 
  DECL_PLUGIN_STATE();
  int i;
  int bufIndex = SND_STATE(currentPlayBuffer);

  for(i=0;i<NUM_BUFFERS;i++)
    {
      if(((buffers[bufIndex])->dwFlags & WHDR_DONE) != 0) {
        return buffers[bufIndex];
      }
      bufIndex = (bufIndex+1) % NUM_BUFFERS;
    }
  return NULL;
}

/* provide a buffer for sound input */
static int 
provideRecordBuffer _iargs(WAVEHDR *header)
{
  DECL_PLUGIN_STATE();
  header->dwUser = 0; /* reset read index */
  if(waveInPrepareHeader(SND_STATE(hWaveIn),header,sizeof(WAVEHDR)) != 0)
    return 0;
  if(waveInAddBuffer(SND_STATE(hWaveIn),header,sizeof(WAVEHDR)) != 0)
    return 0;
  return 1;
}

static int
providePlayBuffer _iargs(WAVEHDR *header)
{
  DECL_PLUGIN_STATE();
  /* give us a time stamp for the mix-in of samples */
  if(!SND_STATE(playingStarted)) 
    {
      (SND_STATE(playBuffers)[SND_STATE(currentPlayBuffer)])->dwUser = (DWORD) ioMicroMSecs();
      SND_STATE(playingStarted) = 1;
    }
  if(waveOutPrepareHeader(SND_STATE(hWaveOut),header,sizeof(WAVEHDR)) != 0)
    return 0;
  if(waveOutWrite(SND_STATE(hWaveOut),header,sizeof(WAVEHDR)) != 0)
    return 0;
  return 1;
}

int win32_snd_StopPlaying _iarg()
{
  DECL_PLUGIN_STATE();
  HWAVEOUT h;
  SND_STATE(lastPlayBuffer) = -1;

  WaitForSingleObject(SND_STATE(win32SyncMutex), INFINITE);
  h = SND_STATE(hWaveOut);
  SND_STATE(hWaveOut) = 0;
  ReleaseMutex(SND_STATE(win32SyncMutex));
  if(!h) return 0;
  DPRINTF(("Shutting down W32Sound\n"));
  waveOutReset(h);
  waveOutClose(h);
  return 1;
}


/* The wave callback: Signal the associated semaphore that a buffer has completed.
   This means either that the buffer has been played and can be filled again (playing)
   or that the buffer is filled and waits for being read (recording) */
void CALLBACK
waveCallback(HWAVEOUT hWave, UINT uMsg, DWORD interp, DWORD dwParam1, DWORD dwParam2)
{
  struct Interpreter * intr = (struct Interpreter *) interp;
  DECL_PLUGIN_STATE();
  if(uMsg == WOM_DONE)
    { /* a buffer has been played */
      SND_STATE(currentPlayBuffer) = (SND_STATE(currentPlayBuffer) + 1) % NUM_BUFFERS;
      (SND_STATE(playBuffers)[SND_STATE(currentPlayBuffer)])->dwUser = (DWORD) ioMicroMSecs();
      if(SND_STATE(currentPlayBuffer) == SND_STATE(lastPlayBuffer)) {
	DPRINTF(("W32Sound: stopping from waveCallback\n"));
	win32_snd_StopPlaying _iparam();
	DPRINTF(("W32Sound: stopped from waveCallback\n"));
      }
    }
  if(uMsg == WOM_DONE || uMsg == WIM_DATA)
    { /* a buffer has been completed */
      synchronizedSignalSemaphoreWithIndex(intr, SND_STATE(outSemaIndex));
    }
}

/*******************************************************************/
/*  Sound output functions                                         */
/*******************************************************************/

int win32_snd_AvailableSpace _iarg()
{ 
  DECL_PLUGIN_STATE();
  int i;
  for(i=0;i<NUM_BUFFERS;i++)
    if(((SND_STATE(playBuffers)[i])->dwFlags & WHDR_DONE) != 0)
      return SND_STATE(outBufferSize);
  return 0;
}

int win32_snd_PlaySamplesFromAtLength _iargs(int frameCount, int arrayIndex, int startIndex)
{ 
  DECL_PLUGIN_STATE();
  WAVEHDR *header;
  int bytesWritten;
  char *src,*dst,*end;
  short *shortSrc;

  header = findEmptyHeader _iparams(SND_STATE(playBuffers));
  if(!header) return 0; /* no buffer */

  DPRINTF(("%d ", frameCount));

  bytesWritten = SND_STATE(waveOutFormat).nBlockAlign  * frameCount;
  if(bytesWritten > SND_STATE(outBufferSize))
    bytesWritten = SND_STATE(outBufferSize);

  src = (char *) (arrayIndex + startIndex);
  dst = (char *) header->lpData;
  end = (char *) dst + bytesWritten;
  shortSrc = (short*) src;

  if (SND_STATE(waveOutFormat).wBitsPerSample == 8) 
    {  /* 8-bit samples */
      if (SND_STATE(waveOutFormat).nChannels == 2) /* stereo */
        while (dst < end)
          { *dst++ = (*shortSrc++ + 0x8000) >> 8;
            *dst++ = (*shortSrc++ + 0x8000) >> 8;
          }
      else
        while (dst < end)
          *dst++ = ( ((*shortSrc++ + 0x8000) >> 8) + ((*shortSrc++ + 0x8000) >> 8) ) / 2;
          
    } 
  else 
    if (SND_STATE(waveOutFormat).nChannels == 2) /* stereo */
      while (dst < end)
        *((long*)dst)++ = *((long*)src)++;
    else /* if mono, skip every other frame of the source */
      while (dst < end) 
        {
          *((short*)dst)++ = (*shortSrc++ + *shortSrc++) / 2;
        }

  header->dwBufferLength = bytesWritten;
  if(!providePlayBuffer _iparams(header)) return 0;
  return bytesWritten / SND_STATE(waveOutFormat).nBlockAlign;
}

void MixInSamples _iargs(int count, char *srcBufPtr, int srcStartIndex, char *dstBufPtr, int dstStartIndex) 
{ 
  DECL_PLUGIN_STATE();
  unsigned char *src, *dst, *end;
  short *shortSrc, *shortDst;
  int sample;

  src = (unsigned char *) srcBufPtr + (srcStartIndex * 4);
  dst = (unsigned char *) dstBufPtr + (dstStartIndex * SND_STATE(waveOutFormat).nBlockAlign);
  end = (unsigned char *) dstBufPtr + ((dstStartIndex + count) * 4);
  shortSrc = (short*) src;
  shortDst = (short*) dst;

  if (SND_STATE(waveOutFormat).wBitsPerSample == 8) 
    {  /* 8-bit samples */
      if (SND_STATE(waveOutFormat).nChannels == 2) /* stereo */
        while (dst < end)
          { 
            sample = *dst + ((*shortSrc++ + 0x8000) >> 8);
            /* @@: working on unsigned - no underflow */
            if(sample > 255) sample = 255;
            *dst++ = sample;
          }
      else /* Mono */
        while (dst < end)
          {
            sample = *dst + ( ((*shortSrc++ + 0x8000) >> 8) + ((*shortSrc++ + 0x8000) >> 8) ) / 2;
            /* @@: working on unsigned - no underflow */
            if(sample > 255) sample = 255;
            *dst++ = sample;
          }
          
    } 
  else /* 16-bit samples */
    if (SND_STATE(waveOutFormat).nChannels == 2) /* stereo */
      while (shortDst < (short*)end)
        {
          sample = (*shortDst + *shortSrc++) / 2;
          if (sample > 32767) sample = 32767;
          else if (sample < -32767) sample = -32767;
          *shortDst++ = sample;
        }
    else /* if mono, skip every other frame of the source */
      while (shortDst < (short*)end) 
        {
          sample = (*dst + *shortSrc++ + *shortSrc++) / 4;
          if (sample > 32767) sample = 32767;
          else if (sample < -32767) sample = -32767;
          *shortDst++ = sample;
        }
}


int win32_snd_PlaySilence _iarg()
{ 
  DECL_PLUGIN_STATE();
  WAVEHDR *header;

  header = findEmptyHeader _iparams(SND_STATE(playBuffers));
  if(!header) return 0; /* no buffer */
  ZeroMemory(header->lpData, header->dwBufferLength);
  if(!providePlayBuffer _iparams(header)) return 0;
  return header->dwBufferLength / SND_STATE(waveOutFormat).nBlockAlign;
}

static int computeplayBufferSize(int frameCount, int bytesPerFrame) {
  int bufferBytes;

  bufferBytes = (bytesPerFrame * frameCount) / (NUM_BUFFERS / 2);
  bufferBytes = (bufferBytes / 16) * 16;
  /* Make sure our buffers are powers of two or multiple of 4k */
  if(bufferBytes <= 2048) {
    if(bufferBytes <= 1024) {
      if(bufferBytes <= 512) {
	return 512;
      } else {
	return 1024;
      }
    } else {
      return 2048;
    }
  }
  return (bufferBytes + 4095) & ~4095;
}

static int startDevicePlaying _iargs(int frameCount, int bytesPerFrame)
{
  DECL_PLUGIN_STATE();
  int bufferBytes;

  static int initFlag = 0;

  /* Initialize output buffers */
  bufferBytes = computeplayBufferSize(frameCount, bytesPerFrame);
  if(bufferBytes != SND_STATE(outBufferSize) || !initFlag) {
#ifndef NDEBUG
    warnPrintf(TEXT("Initializing %d output buffers of size %d\n"), NUM_BUFFERS, bufferBytes);
#endif
    SND_STATE(outBufferSize) = bufferBytes;
    if(initFlag) freeWaveHeaders(SND_STATE(playBuffers));
    initWaveHeaders(SND_STATE(playBuffers), SND_STATE(outBufferSize));
    initFlag = 1;
  }
  SND_STATE(playingStarted) = 0;
  SND_STATE(currentPlayBuffer) = 0;
  SND_STATE(lastPlayBuffer) = -1;
  return 1;
}

int win32_snd_Start _iargs(int frameCount, int samplesPerSec, int stereo, int semaphoreIndex)
{
  DECL_PLUGIN_STATE();
  int bytesPerFrame;
  int bytesPerSample;
  int bufferSize;
  int maxDevs = 0,devId = 0;
  MMRESULT result;

  bytesPerFrame = stereo ? 2 * bytesPerSample : bytesPerSample;
  bufferSize = computeplayBufferSize(frameCount, bytesPerFrame);
  if((bufferSize != SND_STATE(outBufferSize)) || 
     (samplesPerSec != SND_STATE(waveOutFormat).nSamplesPerSec) || 
     ((stereo == 0) != (SND_STATE(waveOutFormat).nChannels == 1))) {
    /* format change */
    DPRINTF(("W32Sound: stopping from win32_snd_Start\n"));
    DPRINTF(("W32Sound format change (%d, %d, %s)\n", frameCount, samplesPerSec, (stereo ? "stereo" : "mono")));
    win32_snd_StopPlaying _iparam();
    DPRINTF(("W32Sound: stopped from win32_snd_Start\n"));
  }

  SND_STATE(outSemaIndex) = semaphoreIndex; /* might have changed */

  if (SND_STATE(hWaveOut)) {
    /* still open from last time; keep playing */
    SND_STATE(lastPlayBuffer) = -1;
    DPRINTF(("Continuing W32Sound\n"));
    return 1;
  }

  DPRINTF(("Starting W32Sound\n"));

  /* perform an initial attempt to open exactly the specified values */
  for(bytesPerSample = 2; bytesPerSample >= 1; bytesPerSample--) {
    bytesPerFrame = stereo ? 2 * bytesPerSample : bytesPerSample;
    SND_STATE(waveOutFormat).wFormatTag = WAVE_FORMAT_PCM;
    SND_STATE(waveOutFormat).nChannels = stereo ? 2 : 1;
    SND_STATE(waveOutFormat).nSamplesPerSec = samplesPerSec;
    SND_STATE(waveOutFormat).nAvgBytesPerSec = samplesPerSec * bytesPerFrame;
    SND_STATE(waveOutFormat).nBlockAlign = bytesPerFrame;
    SND_STATE(waveOutFormat).wBitsPerSample = 8 * bytesPerSample;
    result = waveOutOpen(&SND_STATE(hWaveOut), WAVE_MAPPER, (LPWAVEFORMAT)&SND_STATE(waveOutFormat), 
			 (DWORD)waveCallback, (DWORD)_iparam(), CALLBACK_FUNCTION);
    if(result == 0) { /* success */
      /* get device caps */
      waveOutGetDevCaps(devId,&SND_STATE(outCaps),sizeof(WAVEOUTCAPS));
      return startDevicePlaying _iparams(frameCount, bytesPerFrame);
    }
  }

  /* find a device supporting the requested output rate */
  maxDevs = waveOutGetNumDevs();
  /* try 16bit devices first */
  bytesPerSample = 2;
  for(devId=0;devId<maxDevs;devId++){
    WAVEOUTCAPS caps;
    int flag;
    waveOutGetDevCaps(devId,&caps,sizeof(WAVEOUTCAPS));
    switch (samplesPerSec) {
    case 11025: flag = stereo ? WAVE_FORMAT_1S16 : WAVE_FORMAT_1M16; break;
    case 22050: flag = stereo ? WAVE_FORMAT_2S16 : WAVE_FORMAT_2M16; break;
    case 44100: flag = stereo ? WAVE_FORMAT_4S16 : WAVE_FORMAT_4M16; break;
    default: return 0;
    }
    if(caps.dwFormats & flag) break;
  }
  
  if(devId >= maxDevs) {
    /* try 8bit devices */
    bytesPerSample = 1;
    for(devId=0;devId<maxDevs;devId++) {
      WAVEOUTCAPS caps;
      int flag;
      waveOutGetDevCaps(devId,&caps,sizeof(WAVEOUTCAPS));
      switch (samplesPerSec) {
      case 11025: flag = stereo ? WAVE_FORMAT_1S08 : WAVE_FORMAT_1M08; break;
      case 22050: flag = stereo ? WAVE_FORMAT_2S08 : WAVE_FORMAT_2M08; break;
      case 44100: flag = stereo ? WAVE_FORMAT_4S08 : WAVE_FORMAT_4M08; break;
      default: return 0;
      }
      if(caps.dwFormats & flag) break;
    }
  }
  if(devId >= maxDevs) return 0;

  bytesPerFrame = stereo ? 2 * bytesPerSample : bytesPerSample;
  SND_STATE(waveOutFormat).wFormatTag = WAVE_FORMAT_PCM;
  SND_STATE(waveOutFormat).nChannels = stereo ? 2 : 1;
  SND_STATE(waveOutFormat).nSamplesPerSec = samplesPerSec;
  SND_STATE(waveOutFormat).nAvgBytesPerSec = samplesPerSec * bytesPerFrame;
  SND_STATE(waveOutFormat).nBlockAlign = bytesPerFrame;
  SND_STATE(waveOutFormat).wBitsPerSample = 8 * bytesPerSample;


  result = waveOutOpen(&SND_STATE(hWaveOut),devId, (LPWAVEFORMAT)&SND_STATE(waveOutFormat), 
		       (DWORD)waveCallback, (DWORD)_iparam(), CALLBACK_FUNCTION);

  if(result != 0) {
#ifndef NO_WARNINGS
    TCHAR errorText[256];
    waveOutGetErrorText(result,errorText,255);
    warnPrintf(TEXT("%s\n"),errorText);
#endif
    return 0;
  } else {
    /* get device caps */
    waveOutGetDevCaps(devId,&SND_STATE(outCaps),sizeof(WAVEOUTCAPS));
  }
  return startDevicePlaying _iparams(frameCount, bytesPerFrame);
}

int win32_snd_Stop _iarg()
{
  DECL_PLUGIN_STATE();
  WAVEHDR *header;
  int i, idx;
  if(!SND_STATE(hWaveOut)) return 0;
  if(SND_STATE(playBuffers)[SND_STATE(currentPlayBuffer)]->dwFlags & WHDR_DONE) {
    /* no buffers playing */
    DPRINTF(("W32Sound: stopping from win32_snd_Stop\n"));
    win32_snd_StopPlaying _iparam();
    DPRINTF(("W32Sound: stopped from win32_snd_Stop\n"));
  }
  for(i = 0; i < NUM_BUFFERS; i++) {
    idx = (SND_STATE(currentPlayBuffer) + i + 1) % NUM_BUFFERS;
    if(SND_STATE(playBuffers)[idx]->dwFlags & WHDR_DONE) {
      SND_STATE(lastPlayBuffer) = idx;
      DPRINTF(("W32Sound: last play buffer = %d\n", SND_STATE(lastPlayBuffer)));
      DPRINTF(("W32Sound: current play buffer = %d\n", SND_STATE(currentPlayBuffer)));
      return 1;
    }
  }
  /* all buffers are being played */
  SND_STATE(lastPlayBuffer) = SND_STATE(currentPlayBuffer);
  DPRINTF(("W32Sound: last = current play buffer = %d\n", SND_STATE(lastPlayBuffer)));
  return 1;
}

int win32_snd_InsertSamplesFromLeadTime _iargs(int frameCount, int srcBufPtr, int samplesOfLeadTime) 
{ 
  DECL_PLUGIN_STATE();
  WAVEHDR *header;
  int currentBuffer;
  int samplesInserted;
  int startSample;
  int i, count;
  DWORD nowTime;

  if(!SND_STATE(hWaveOut)) return -1;
  samplesInserted = 0;
  currentBuffer = SND_STATE(currentPlayBuffer);
  /* mix as many samples as can fit into the remainder of the currently playing buffer */
  header = SND_STATE(playBuffers)[currentBuffer];
  nowTime = (DWORD) ioMicroMSecs();
  startSample = ((SND_STATE(waveOutFormat).nSamplesPerSec * (nowTime - header->dwUser)) / 1000) + samplesOfLeadTime;
  if((DWORD) startSample * SND_STATE(waveOutFormat).nBlockAlign < header->dwBufferLength)
    {
      count = (header->dwBufferLength / SND_STATE(waveOutFormat).nBlockAlign) - startSample;
      if(count > frameCount) count = frameCount;
      MixInSamples _iparams(count, (char*) srcBufPtr, 0, header->lpData, startSample);
      samplesInserted = count;
    }
  /* mix remaining samples into the inactive buffers */
  for(i=1;i<NUM_BUFFERS;i++)
    {
      currentBuffer = (currentBuffer+1) % NUM_BUFFERS;
      header = SND_STATE(playBuffers)[currentBuffer];
      count = (header->dwBufferLength / SND_STATE(waveOutFormat).nBlockAlign);
      if((samplesInserted + count) > frameCount) count = frameCount - samplesInserted;
      MixInSamples _iparams(count, (char*) srcBufPtr, samplesInserted, header->lpData, 0);
      samplesInserted += count;
    }
  return samplesInserted;
}

/*******************************************************************/
/*  Sound input functions                                          */
/*******************************************************************/

int win32_snd_SetRecordLevel(int level)
{
  /* Note: I don't see a simple way of accomplishing the change in recording level.
     One way (such as on my SB16) would be to modify the output level of the source
     signal. This works fine on on SoundBlaster cards but I don't know of other cards.
     Also we would have to know what we're recording (it's no good idea to change
     the output level of _all_ available drivers such as CD, MIDI, Wave, PC-Speaker,
     Line-In, or Microphone).
     Another way could be using the mixer functions, but my help files and books
     lack this topic completely. If somebody has an idea how to do it, let me know.
     AR
  */
  return 1;
}


static int startDeviceRecording _iarg()
{
  DECL_PLUGIN_STATE();
  int i;
  /* The device was successfully opened. Provide the input buffers. */
  for(i=0; i<NUM_BUFFERS; i++)
    if(!provideRecordBuffer _iparams(SND_STATE(recordBuffers)[i])) return 0; /* something went wrong */
  /* first buffer has index 0 */
  SND_STATE(currentRecordBuffer) = 0;
  /* And away we go ... */
  if(waveInStart(SND_STATE(hWaveIn)) != 0) return 0;
  return 1;
}

int win32_snd_StartRecording _iargs(int desiredSamplesPerSec, int stereo, int semaphoreIndex) 
{
  DECL_PLUGIN_STATE();
  int bytesPerSample;
  int bytesPerFrame, samplesPerSec;
  int maxDevs = 0,devId = 0, i;
  MMRESULT result;

  static int initFlag = 0;

  if(!initFlag) {
    /* first time -- initialize wave headers */
    initWaveHeaders(SND_STATE(recordBuffers), BUFFER_SIZE);
    initFlag = 1;
  }

  if (SND_STATE(hWaveIn))  {
    /* still open from last time; clean up before continuing */
    snd_StopRecording _iparam();
  }

  /* perform an initial attempt to open exactly the specified values */
  for(i=0;i<2;i++) {
    for(bytesPerSample = 2; bytesPerSample >= 1; bytesPerSample--) {
      bytesPerFrame = stereo ? 2 * bytesPerSample : bytesPerSample;
      SND_STATE(waveInFormat).wFormatTag = WAVE_FORMAT_PCM;
      SND_STATE(waveInFormat).nChannels = stereo ? 2 : 1;
      SND_STATE(waveInFormat).nSamplesPerSec = desiredSamplesPerSec;
      SND_STATE(waveInFormat).nAvgBytesPerSec = desiredSamplesPerSec * bytesPerFrame;
      SND_STATE(waveInFormat).nBlockAlign = bytesPerFrame;
      SND_STATE(waveInFormat).wBitsPerSample = 8 * bytesPerSample;
/*      
sig: this strange code seem never worked because semaphoreIndex is completely ignored in callback 

	result = waveInOpen(&SND_STATE(hWaveIn),WAVE_MAPPER, (LPWAVEFORMAT)&SND_STATE(waveInFormat), 
			  (DWORD)waveCallback, semaphoreIndex, CALLBACK_FUNCTION); 
*/
      result = waveInOpen(&SND_STATE(hWaveIn),WAVE_MAPPER, (LPWAVEFORMAT)&SND_STATE(waveInFormat), 
			  (DWORD)waveCallback, (DWORD)_iparam(), CALLBACK_FUNCTION);

	  if(result == 0) {/* success */
	return startDeviceRecording _iparam();
      }
    }
    /* round desiredSamplesPerSec to valid values for the second round */
    if (desiredSamplesPerSec <= 16538)
      samplesPerSec = 11025;
    else if(desiredSamplesPerSec <= 33075)
      samplesPerSec = 22050;
    else
      samplesPerSec = 44100;
  }
  /* finally, try finding a device supporting the requested input rate */
  maxDevs = waveInGetNumDevs();
  /* try 16bit devices first */
  bytesPerSample = 2;
  for(devId=0;devId<maxDevs;devId++) {
    WAVEINCAPS caps;
    int flag;
    waveInGetDevCaps(devId,&caps,sizeof(WAVEINCAPS));
    switch (samplesPerSec) {
    case 11025: flag = stereo ? WAVE_FORMAT_1S16 : WAVE_FORMAT_1M16; break;
    case 22050: flag = stereo ? WAVE_FORMAT_2S16 : WAVE_FORMAT_2M16; break;
    case 44100: flag = stereo ? WAVE_FORMAT_4S16 : WAVE_FORMAT_4M16; break;
    default: return 0;
    }
    if(caps.dwFormats & flag) break;
  }
  
  if(devId >= maxDevs) {
    /* try 8bit devices */
    bytesPerSample = 1;
    for(devId=0;devId<maxDevs;devId++){
      WAVEINCAPS caps;
      int flag;
      waveInGetDevCaps(devId,&caps,sizeof(WAVEINCAPS));
      switch (samplesPerSec) {
      case 11025: flag = stereo ? WAVE_FORMAT_1S08 : WAVE_FORMAT_1M08; break;
      case 22050: flag = stereo ? WAVE_FORMAT_2S08 : WAVE_FORMAT_2M08; break;
      case 44100: flag = stereo ? WAVE_FORMAT_4S08 : WAVE_FORMAT_4M08; break;
      default: return 0;
      }
      if(caps.dwFormats & flag) break;
    }
  }
  if(devId >= maxDevs) return 0;

  bytesPerFrame = stereo ? 2 * bytesPerSample : bytesPerSample;
  SND_STATE(waveInFormat).wFormatTag = WAVE_FORMAT_PCM;
  SND_STATE(waveInFormat).nChannels = stereo ? 2 : 1;
  SND_STATE(waveInFormat).nSamplesPerSec = samplesPerSec;
  SND_STATE(waveInFormat).nAvgBytesPerSec = samplesPerSec * bytesPerFrame;
  SND_STATE(waveInFormat).nBlockAlign = bytesPerFrame;
  SND_STATE(waveInFormat).wBitsPerSample = 8 * bytesPerSample;

/*  result = waveInOpen(&SND_STATE(hWaveIn),devId, (LPWAVEFORMAT)&SND_STATE(waveInFormat), 
		      (DWORD)waveCallback, semaphoreIndex, CALLBACK_FUNCTION);
*/
  result = waveInOpen(&SND_STATE(hWaveIn),devId, (LPWAVEFORMAT)&SND_STATE(waveInFormat), 
		      (DWORD)waveCallback, (DWORD) _iparam(), CALLBACK_FUNCTION);

  if(result != 0) {
#ifndef NO_WARNINGS
    TCHAR errorText[256];
    waveInGetErrorText(result,errorText,255);
    warnPrintf(TEXT("%s\n"),errorText);
#endif
    return 0;
  }
  return startDeviceRecording _iparam();
}

int win32_snd_StopRecording _iarg()
{
  DECL_PLUGIN_STATE();
  if(!SND_STATE(hWaveIn)) return 0;
  waveInReset(SND_STATE(hWaveIn));
  waveInClose(SND_STATE(hWaveIn));
  SND_STATE(hWaveIn) = 0;
  return 1;
}

double win32_snd_GetRecordingSampleRate _iarg()
{
  DECL_PLUGIN_STATE();
  if(!SND_STATE(hWaveIn)) return 0.0;
  return (double) SND_STATE(waveInFormat).nSamplesPerSec;
}

int win32_snd_RecordSamplesIntoAtLength _iargs(int buf, int startSliceIndex, int bufferSizeInBytes)
{
  DECL_PLUGIN_STATE();
  /* if data is available, copy as many sample slices as possible into the
     given buffer starting at the given slice index. do not write past the
     end of the buffer, which is buf + bufferSizeInBytes. return the number
     of slices (not bytes) copied. a slice is one 16-bit sample in mono
     or two 16-bit samples in stereo. */
  int bytesPerSlice = (SND_STATE(waveInFormat).nChannels * 2);
  int recordBytesPerSlice = (SND_STATE(waveInFormat).wBitsPerSample / 8);
  char *nextBuf = (char *) buf + (startSliceIndex * bytesPerSlice);
  char *bufEnd = (char *) buf + bufferSizeInBytes;
  char *srcStart, *src, *srcEnd;
  int bytesCopied;
  WAVEHDR *recBuf;

  if (!SND_STATE(hWaveIn)) {
    success _iparams(false);
    return 0;
  }

  /* Copy all recorded samples into the buffer provided.
     We use an endless loop here which is exited if
       a) there is no room left in the provided buffer
       b) there is no data available from the wave input device
     The WAVEHDR's dwUser member is used to mark the read position
     in case a previous call has been exited because of a)
  */
  bytesCopied = 0;
  while(true)
    {
      /* select the buffer */
      recBuf = SND_STATE(recordBuffers)[SND_STATE(currentRecordBuffer)];
      /* We do _NOT_ copy partial buffer contents, so wait 
         until the buffer is marked */
      if((recBuf->dwFlags & WHDR_DONE) == 0) break;
      /* copy samples into the client's buffer */
      srcStart = recBuf->lpData + recBuf->dwUser;
      src = srcStart;
      srcEnd = recBuf->lpData + recBuf->dwBytesRecorded;
      if (SND_STATE(waveInFormat).wBitsPerSample == 8) 
        {
          while ((src < srcEnd) && (nextBuf < bufEnd)) 
            {
              /* convert 8-bit sample to 16-bit sample */
              *nextBuf++ = 0;  /* low-order byte is zero */
              *nextBuf++ = (*src++) - 127;  /* convert from [0-255] to [-128-127] */
            }
        }
      else 
        {
          while ((src < srcEnd) && (nextBuf < bufEnd)) 
            *nextBuf++ = *src++;
        }
      bytesCopied += src - srcStart;
      /* are we at the end of the provided buffer? */
      if(src < srcEnd)
        { /* Yes, update read index */
          recBuf->dwUser = src - recBuf->lpData;
          /* and exit */
          break;
        }
      /* we have completed a buffer, send it back to the device for further use */
      if(!provideRecordBuffer _iparams(recBuf)) break; /* something went wrong */
      /* step on to the next buffer */
      SND_STATE(currentRecordBuffer) = (SND_STATE(currentRecordBuffer) + 1) % NUM_BUFFERS;
    }
  /* return the number of slices copied */
  return bytesCopied / recordBytesPerSlice;
}

void win32_snd_Volume(double *left, double *right)
{
  DWORD volume = (DWORD)-1;

  waveOutGetVolume((HWAVEOUT)0, &volume);
  *left = (volume & 0xFFFF) / 65535.0;
  *right = (volume >> 16) / 65535.0;
}

void win32_snd_SetVolume(double left, double right)
{
  DWORD volume;

  if(left < 0.0) left = 0.0;
  if(left > 1.0) left = 1.0;
  volume = (int)(left * 0xFFFF);
  if(right < 0.0) right = 0.0;
  if(right > 1.0) right = 1.0;
  volume |= ((int)(right *0xFFFF)) << 16;

  waveOutSetVolume((HWAVEOUT) 0, volume);
}


/* sound output */
int snd_AvailableSpace _iarg() {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound))
    return dx_snd_AvailableSpace _iparam();
#endif
  return win32_snd_AvailableSpace _iparam();
}

int snd_InsertSamplesFromLeadTime _iargs(int frameCount, int srcBufPtr, int samplesOfLeadTime) {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound))
    return dx_snd_InsertSamplesFromLeadTime _iparams(frameCount, srcBufPtr, samplesOfLeadTime);
#endif
  return win32_snd_InsertSamplesFromLeadTime _iparams(frameCount, srcBufPtr, samplesOfLeadTime);
}

int snd_PlaySamplesFromAtLength _iargs(int frameCount, int arrayIndex, int startIndex) {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound))
    return dx_snd_PlaySamplesFromAtLength _iparams(frameCount, arrayIndex, startIndex);
#endif
  return win32_snd_PlaySamplesFromAtLength _iparams(frameCount, arrayIndex, startIndex);
}

int snd_PlaySilence _iarg() {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound))
    return dx_snd_PlaySilence();
#endif
  return win32_snd_PlaySilence _iparam();
}

int snd_Start _iargs(int frameCount, int samplesPerSec, int stereo, int semaIndex) {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound) || SND_STATE(fUseDirectSound))
    return dx_snd_Start _iparams(frameCount, samplesPerSec, stereo, semaIndex);
#endif
  return win32_snd_Start _iparams(frameCount, samplesPerSec, stereo, semaIndex);
}

int snd_Stop _iarg() {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound))
    return dx_snd_Stop _iparam();
#endif
  return win32_snd_Stop _iparam();
}

/* sound input */
int snd_SetRecordLevel _iargs(int level) {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound))
    return dx_snd_SetRecordLevel(level);
#endif
  return win32_snd_SetRecordLevel(level);
}

int snd_StartRecording _iargs(int desiredSamplesPerSec, int stereo, int semaIndex) {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound) || SND_STATE(fUseDirectSound))
    return dx_snd_StartRecording _iparams(desiredSamplesPerSec, stereo, semaIndex);
#endif
  return win32_snd_StartRecording _iparams(desiredSamplesPerSec, stereo, semaIndex);
}

int snd_StopRecording _iarg() {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound))
    return dx_snd_StopRecording _iparam();
#endif
  return win32_snd_StopRecording _iparam();
}

double snd_GetRecordingSampleRate _iarg() {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound))
    return dx_snd_GetRecordingSampleRate _iparam();
#endif
  return win32_snd_GetRecordingSampleRate _iparam();
}

int snd_RecordSamplesIntoAtLength _iargs(int buf, int startSliceIndex, int bufferSizeInBytes) {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound))
    return dx_snd_RecordSamplesIntoAtLength _iparams(buf, startSliceIndex, bufferSizeInBytes);
#endif
    return win32_snd_RecordSamplesIntoAtLength _iparams(buf, startSliceIndex, bufferSizeInBytes);
}
  
void snd_Volume _iargs(double *left, double *right) {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound) || SND_STATE(fUseDirectSound))
    return dx_snd_Volume(left, right);
#endif
  return win32_snd_Volume(left, right);
}

void snd_SetVolume _iargs(double left, double right) {
#ifndef NO_DIRECT_SOUND
  DECL_PLUGIN_STATE();
  if(SND_STATE(fUsingDirectSound) || fUseDirectSound)
    return dx_snd_SetVolume(left, right);
#endif
  return win32_snd_SetVolume(left, right);
}

int snd_SetRecordBufferFrameCount _iargs(int frameCount)
{
    DECL_PLUGIN_STATE();
	int alreadyRecording = 0;
	if (SND_STATE(recBufferFrameCount) == frameCount) return 0; /* no change */

	/* Check if we're already recording... if so, fail. */
	if (SND_STATE(lpdRecBuffer) != 0 || SND_STATE(hWaveIn) != 0) return 1;

	SND_STATE(recBufferFrameCount) = frameCount;
	return 0;
}


#endif /* NO_SOUND */
