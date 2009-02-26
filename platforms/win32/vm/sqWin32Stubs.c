/****************************************************************************
*   PROJECT: Squeak port for Win32 (NT / Win95)
*   FILE:    sqWin32Stubs.c
*   CONTENT: Stubs for most optional primitives
*
*   AUTHOR:  Andreas Raab (ar)
*   ADDRESS: University of Magdeburg, Germany
*   EMAIL:   raab@isg.cs.uni-magdeburg.de
*   RCSID:   $Id$
*
*   NOTES:
*****************************************************************************/
#include "sq.h"

#define FAIL_STUB { PInterpreter intr = interpreterOfCurrentThread(); return success _iparams(false); }

int ioDisablePowerManager(int disableIfNonZero) FAIL_STUB

#ifdef NO_SOUND
int snd_AvailableSpace(void) FAIL_STUB 
int snd_InsertSamplesFromLeadTime(int frameCount, int srcBufPtr, int samplesOfLeadTime) FAIL_STUB 
int snd_PlaySamplesFromAtLength(int frameCount, int arrayIndex, int startIndex) FAIL_STUB 
int snd_PlaySilence(void) FAIL_STUB
int snd_Start(int frameCount, int samplesPerSec, int stereo, int semaIndex) FAIL_STUB
int snd_Stop(void) { return 1; }
int snd_SetRecordLevel(int level) FAIL_STUB
int snd_StartRecording(int desiredSamplesPerSec, int stereo, int semaIndex) FAIL_STUB
int snd_StopRecording(void) FAIL_STUB
double snd_GetRecordingSampleRate(void) FAIL_STUB
int snd_RecordSamplesIntoAtLength(int buf, int startSliceIndex, int bufferSizeInBytes) FAIL_STUB
#endif

#ifdef NO_JOYSTICK
int joystickInit(void) { return 1; }
int joystickRead(int stickIndex) FAIL_STUB
#endif

#ifdef NO_NETWORK
#ifndef SocketPtr 
typedef sqInt SocketPtr;
#endif
int		sqNetworkInit _iargs(int resolverSemaIndex) FAIL_STUB 
void	sqNetworkShutdown(void) { }
void	sqResolverAbort(void) FAIL_STUB 
void	sqResolverAddrLookupResult(char *nameForAddress, int nameSize) FAIL_STUB 
int		sqResolverAddrLookupResultSize(void) FAIL_STUB 
int		sqResolverError(void) FAIL_STUB 
int		sqResolverLocalAddress(void) FAIL_STUB 
int		sqResolverNameLookupResult(void) FAIL_STUB 
void	sqResolverStartAddrLookup(int address) FAIL_STUB 
void	sqResolverStartNameLookup(char *hostName, int nameSize) FAIL_STUB 
int		sqResolverStatus(void) FAIL_STUB 
void	sqSocketAbortConnection(SocketPtr s) FAIL_STUB 
void	sqSocketCloseConnection(SocketPtr s) FAIL_STUB 
int		sqSocketConnectionStatus(SocketPtr s) FAIL_STUB 
void	sqSocketConnectToPort(SocketPtr s, int addr, int port) FAIL_STUB 
void	sqSocketCreateNetTypeSocketTypeRecvBytesSendBytesSemaID(
			SocketPtr s, int netType, int socketType,
			int recvBufSize, int sendBufSize, int semaIndex) FAIL_STUB 
void	sqSocketDestroy(SocketPtr s) FAIL_STUB 
int		sqSocketError(SocketPtr s) FAIL_STUB 
void	sqSocketListenOnPort(SocketPtr s, int port) FAIL_STUB 
int		sqSocketLocalAddress(SocketPtr s) FAIL_STUB 
int		sqSocketLocalPort(SocketPtr s) FAIL_STUB 
int		sqSocketReceiveDataAvailable(SocketPtr s) FAIL_STUB 
int		sqSocketReceiveDataBufCount(SocketPtr s, int buf, int bufSize) FAIL_STUB 
int		sqSocketRemoteAddress(SocketPtr s) FAIL_STUB 
int		sqSocketRemotePort(SocketPtr s) FAIL_STUB 
int		sqSocketSendDataBufCount(SocketPtr s, int buf, int bufSize) FAIL_STUB 
int		sqSocketSendDone(SocketPtr s) FAIL_STUB 
void	sqSocketListenOnPortBacklogSize(SocketPtr s, int port, int backlogSize) FAIL_STUB 
void	sqSocketAcceptFromRecvBytesSendBytesSemaID(
            SocketPtr s, SocketPtr serverSocket,
            int recvBufSize, int sendBufSize, int semaIndex) FAIL_STUB 
int win32DebugPrintSocketState(void) {}
#endif

#ifdef NO_SERIAL_PORT
int serialPortClose(int portNum) { return 1; }
int serialPortOpen(
  int portNum, int baudRate, int stopBitsType, int parityType, int dataBits,
  int inFlowCtrl, int outFlowCtrl, int xOnChar, int xOffChar) FAIL_STUB 
int serialPortReadInto(int portNum, int count, int bufferPtr) FAIL_STUB 
int serialPortWriteFrom(int portNum, int count, int bufferPtr) FAIL_STUB 
#endif

#ifdef NO_MIDI
void SetupMIDI(void) {}
int sqMIDIGetClock(void) FAIL_STUB 
int sqMIDIGetPortCount(void) FAIL_STUB 
int sqMIDIGetPortDirectionality(int portNum) FAIL_STUB 
int sqMIDIGetPortName(int portNum, int namePtr, int length) FAIL_STUB 
int sqMIDIClosePort(int portNum) { return 1; }
int sqMIDIOpenPort(int portNum, int readSemaIndex, int interfaceClockRate) FAIL_STUB 
int sqMIDIParameter(int whichParameter, int modify, int newValue) FAIL_STUB 
int sqMIDIPortReadInto(int portNum, int count, int bufferPtr) FAIL_STUB 
int sqMIDIPortWriteFromAt(int portNum, int count, int bufferPtr, int time) FAIL_STUB 
#endif


#ifdef NO_ASYNC_FILES
int asyncFileClose(AsyncFile *f) FAIL_STUB 
int asyncFileOpen(AsyncFile *f, int fileNamePtr, int fileNameSize, int writeFlag, int semaIndex) FAIL_STUB 
int asyncFileRecordSize() FAIL_STUB 
int asyncFileReadResult(AsyncFile *f, int bufferPtr, int bufferSize) FAIL_STUB 
int asyncFileReadStart(AsyncFile *f, int fPosition, int count) FAIL_STUB 
int asyncFileWriteResult(AsyncFile *f) FAIL_STUB 
int asyncFileWriteStart(AsyncFile *f, int fPosition, int bufferPtr, int bufferSize) FAIL_STUB 
#endif

#ifdef NO_TABLET
int tabletGetParameters(int cursorIndex, int result[]) FAIL_STUB 
int tabletRead(int cursorIndex, int result[]) FAIL_STUB 
int tabletResultSize(void) FAIL_STUB 
#endif
