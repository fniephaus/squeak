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

int ioDisablePowerManager(int disableIfNonZero) { return success(MAIN_VM_COMMA false); }

#ifdef NO_SOUND
int snd_AvailableSpace(void) { return success(MAIN_VM_COMMA false); }
int snd_InsertSamplesFromLeadTime(int frameCount, int srcBufPtr, int samplesOfLeadTime) { return success(MAIN_VM_COMMA false); }
int snd_PlaySamplesFromAtLength(int frameCount, int arrayIndex, int startIndex) { return success(MAIN_VM_COMMA false); }
int snd_PlaySilence(void) { return success(MAIN_VM_COMMA false); }
int snd_Start(int frameCount, int samplesPerSec, int stereo, int semaIndex) { return success(MAIN_VM_COMMA false); }
int snd_Stop(void) { return 1; }
int snd_SetRecordLevel(int level) { return success(MAIN_VM_COMMA false); }
int snd_StartRecording(int desiredSamplesPerSec, int stereo, int semaIndex) { return success(MAIN_VM_COMMA false); }
int snd_StopRecording(void) { return success(MAIN_VM_COMMA false); }
double snd_GetRecordingSampleRate(void) { return success(MAIN_VM_COMMA false); }
int snd_RecordSamplesIntoAtLength(int buf, int startSliceIndex, int bufferSizeInBytes) { return success(MAIN_VM_COMMA false); }
#endif

#ifdef NO_JOYSTICK
int joystickInit(void) { return 1; }
int joystickRead(int stickIndex) { return success(MAIN_VM_COMMA false); }
#endif

#ifdef NO_NETWORK
#ifndef SocketPtr 
typedef sqInt SocketPtr;
#endif
int		sqNetworkInit(PLUGIN_IARG_COMMA int resolverSemaIndex) { return success(MAIN_VM_COMMA false); }
void	sqNetworkShutdown(void) { }
void	sqResolverAbort(void) { success(MAIN_VM_COMMA false); }
void	sqResolverAddrLookupResult(char *nameForAddress, int nameSize) { success(MAIN_VM_COMMA false); }
int		sqResolverAddrLookupResultSize(void) { return success(MAIN_VM_COMMA false); }
int		sqResolverError(void) { return success(MAIN_VM_COMMA false); }
int		sqResolverLocalAddress(void) { return success(MAIN_VM_COMMA false); }
int		sqResolverNameLookupResult(void) { return success(MAIN_VM_COMMA false); }
void	sqResolverStartAddrLookup(int address) { success(MAIN_VM_COMMA false); }
void	sqResolverStartNameLookup(char *hostName, int nameSize) { success(MAIN_VM_COMMA false); }
int		sqResolverStatus(void) { return success(MAIN_VM_COMMA false); }
void	sqSocketAbortConnection(SocketPtr s) { success(MAIN_VM_COMMA false); }
void	sqSocketCloseConnection(SocketPtr s) { success(MAIN_VM_COMMA false); }
int		sqSocketConnectionStatus(SocketPtr s) { return success(MAIN_VM_COMMA false); }
void	sqSocketConnectToPort(SocketPtr s, int addr, int port) { success(MAIN_VM_COMMA false); }
void	sqSocketCreateNetTypeSocketTypeRecvBytesSendBytesSemaID(
			SocketPtr s, int netType, int socketType,
			int recvBufSize, int sendBufSize, int semaIndex) { success(MAIN_VM_COMMA false); }
void	sqSocketDestroy(SocketPtr s) { success(MAIN_VM_COMMA false); }
int		sqSocketError(SocketPtr s) { return success(MAIN_VM_COMMA false); }
void	sqSocketListenOnPort(SocketPtr s, int port) { success(MAIN_VM_COMMA false); }
int		sqSocketLocalAddress(SocketPtr s) { return success(MAIN_VM_COMMA false); }
int		sqSocketLocalPort(SocketPtr s) { return success(MAIN_VM_COMMA false); }
int		sqSocketReceiveDataAvailable(SocketPtr s) { return success(MAIN_VM_COMMA false); }
int		sqSocketReceiveDataBufCount(SocketPtr s, int buf, int bufSize) { return success(MAIN_VM_COMMA false); }
int		sqSocketRemoteAddress(SocketPtr s) { return success(MAIN_VM_COMMA false); }
int		sqSocketRemotePort(SocketPtr s) { return success(MAIN_VM_COMMA false); }
int		sqSocketSendDataBufCount(SocketPtr s, int buf, int bufSize) { return success(MAIN_VM_COMMA false); }
int		sqSocketSendDone(SocketPtr s) { return success(MAIN_VM_COMMA false); }
void	sqSocketListenOnPortBacklogSize(SocketPtr s, int port, int backlogSize) { success(MAIN_VM_COMMA false); }
void	sqSocketAcceptFromRecvBytesSendBytesSemaID(
            SocketPtr s, SocketPtr serverSocket,
            int recvBufSize, int sendBufSize, int semaIndex) { success(MAIN_VM_COMMA false); }
int win32DebugPrintSocketState(void) {}
#endif

#ifdef NO_SERIAL_PORT
int serialPortClose(int portNum) { return 1; }
int serialPortOpen(
  int portNum, int baudRate, int stopBitsType, int parityType, int dataBits,
  int inFlowCtrl, int outFlowCtrl, int xOnChar, int xOffChar) { return success(MAIN_VM_COMMA false); }
int serialPortReadInto(int portNum, int count, int bufferPtr) { return success(MAIN_VM_COMMA false); }
int serialPortWriteFrom(int portNum, int count, int bufferPtr) { return success(MAIN_VM_COMMA false); }
#endif

#ifdef NO_MIDI
void SetupMIDI(void) {}
int sqMIDIGetClock(void) { return success(MAIN_VM_COMMA false); }
int sqMIDIGetPortCount(void) { return success(MAIN_VM_COMMA false); }
int sqMIDIGetPortDirectionality(int portNum) { return success(MAIN_VM_COMMA false); }
int sqMIDIGetPortName(int portNum, int namePtr, int length) { return success(MAIN_VM_COMMA false); }
int sqMIDIClosePort(int portNum) { return 1; }
int sqMIDIOpenPort(int portNum, int readSemaIndex, int interfaceClockRate) { return success(MAIN_VM_COMMA false); }
int sqMIDIParameter(int whichParameter, int modify, int newValue) { return success(MAIN_VM_COMMA false); }
int sqMIDIPortReadInto(int portNum, int count, int bufferPtr) { return success(MAIN_VM_COMMA false); }
int sqMIDIPortWriteFromAt(int portNum, int count, int bufferPtr, int time) { return success(MAIN_VM_COMMA false); }
#endif


#ifdef NO_ASYNC_FILES
int asyncFileClose(AsyncFile *f) { return success(MAIN_VM_COMMA false); }
int asyncFileOpen(AsyncFile *f, int fileNamePtr, int fileNameSize, int writeFlag, int semaIndex) { return success(MAIN_VM_COMMA false); }
int asyncFileRecordSize() { return success(MAIN_VM_COMMA false); }
int asyncFileReadResult(AsyncFile *f, int bufferPtr, int bufferSize) { return success(MAIN_VM_COMMA false); }
int asyncFileReadStart(AsyncFile *f, int fPosition, int count) { return success(MAIN_VM_COMMA false); }
int asyncFileWriteResult(AsyncFile *f) { return success(MAIN_VM_COMMA false); }
int asyncFileWriteStart(AsyncFile *f, int fPosition, int bufferPtr, int bufferSize) { return success(MAIN_VM_COMMA false); }
#endif

#ifdef NO_TABLET
int tabletGetParameters(int cursorIndex, int result[]) { return success(MAIN_VM_COMMA false); }
int tabletRead(int cursorIndex, int result[]) { return success(MAIN_VM_COMMA false); }
int tabletResultSize(void) { return success(MAIN_VM_COMMA false); }
#endif
