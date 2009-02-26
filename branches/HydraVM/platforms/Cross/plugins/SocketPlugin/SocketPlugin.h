/* squeak socket support header file */

/* module initialization/shutdown */
sqInt socketInit(void);
sqInt socketShutdown(void);

typedef struct
{
  int	sessionID;
  int	socketType;  /* 0 = TCP, 1 = UDP */
  void	*privateSocketPtr;
}  SQSocket, *SocketPtr;

/* networking primitives */
sqInt sqNetworkInit _iargs(sqInt resolverSemaIndex);
void  sqNetworkShutdown(void);

/* resolver functions */
void  sqResolverAbort _iarg();
void  sqResolverAddrLookupResult _iargs(char *nameForAddress, sqInt nameSize);
sqInt sqResolverAddrLookupResultSize _iarg();
sqInt sqResolverError _iarg();
sqInt sqResolverLocalAddress _iarg();
sqInt sqResolverNameLookupResult _iarg();
void  sqResolverStartAddrLookup _iargs(sqInt address);
void  sqResolverStartNameLookup _iargs(char *hostName, sqInt nameSize);
sqInt sqResolverStatus _iarg();

void sqSocketAbortConnection _iargs(SocketPtr s);
void sqSocketCloseConnection _iargs(SocketPtr s);
sqInt sqSocketConnectionStatus _iargs(SocketPtr s);
void  sqSocketConnectToPort _iargs(SocketPtr s, sqInt addr, sqInt port);
void  sqSocketCreateNetTypeSocketTypeRecvBytesSendBytesSemaID _iargs(SocketPtr s, sqInt netType, sqInt socketType, sqInt recvBufSize, sqInt sendBufSize, sqInt semaIndex);
void  sqSocketCreateNetTypeSocketTypeRecvBytesSendBytesSemaIDReadSemaIDWriteSemaID _iargs(SocketPtr s, sqInt netType, sqInt socketType, sqInt recvBufSize, sqInt sendBufSize, sqInt semaIndex, sqInt readSemaIndex, sqInt writeSemaIndex);
void  sqSocketDestroy _iargs(SocketPtr s);
sqInt sqSocketError _iargs(SocketPtr s);
void  sqSocketListenOnPort _iargs(SocketPtr s, sqInt port);
sqInt sqSocketLocalAddress _iargs(SocketPtr s);
sqInt sqSocketLocalPort _iargs(SocketPtr s);
sqInt sqSocketReceiveDataAvailable _iargs(SocketPtr s);
sqInt sqSocketReceiveDataBufCount _iargs(SocketPtr s, char *buf, sqInt bufSize);
sqInt sqSocketRemoteAddress _iargs(SocketPtr s);
sqInt sqSocketRemotePort _iargs(SocketPtr s);
sqInt sqSocketSendDataBufCount _iargs(SocketPtr s, char *buf, sqInt bufSize);
sqInt sqSocketSendDone _iargs(SocketPtr s);
/* ar 7/16/1999: New primitives for accept().  Note: If accept() calls are not supported simply make the calls fail and the old connection style will be used. */
void  sqSocketListenOnPortBacklogSize _iargs(SocketPtr s, sqInt port, sqInt backlogSize);
void  sqSocketListenOnPortBacklogSizeInterface _iargs(SocketPtr s, sqInt port, sqInt backlogSize, sqInt addr);
void  sqSocketAcceptFromRecvBytesSendBytesSemaID _iargs(SocketPtr s, SocketPtr serverSocket, sqInt recvBufSize, sqInt sendBufSize, sqInt semaIndex);
void  sqSocketAcceptFromRecvBytesSendBytesSemaIDReadSemaIDWriteSemaID _iargs(SocketPtr s, SocketPtr serverSocket, sqInt recvBufSize, sqInt sendBufSize, sqInt semaIndex, sqInt readSemaIndex, sqInt writeSemaIndex);
sqInt sqSocketReceiveUDPDataBufCountaddressportmoreFlag _iargs(SocketPtr s, char *buf, sqInt bufSize,  sqInt *address,  sqInt *port, sqInt *moreFlag);
sqInt sqSockettoHostportSendDataBufCount _iargs(SocketPtr s, sqInt address, sqInt port, char *buf, sqInt bufSize);
sqInt sqSocketSetOptionsoptionNameStartoptionNameSizeoptionValueStartoptionValueSizereturnedValue _iargs(SocketPtr s, char *optionName, sqInt optionNameSize, char *optionValue, sqInt optionValueSize, sqInt *result);
sqInt sqSocketGetOptionsoptionNameStartoptionNameSizereturnedValue _iargs(SocketPtr s, char *optionName, sqInt optionNameSize, sqInt *result);
/* tpr 4/12/06 add declarations for two new socket routines */
void sqSocketBindToPort _iargs(SocketPtr s, int addr, int port);
void sqSocketSetReusable _iargs(SocketPtr s);


