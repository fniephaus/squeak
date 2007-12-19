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
sqInt sqNetworkInit(PLUGIN_IARG_COMMA sqInt resolverSemaIndex);
void  sqNetworkShutdown(void);

/* resolver functions */
void  sqResolverAbort(PLUGIN_IARG);
void  sqResolverAddrLookupResult(PLUGIN_IARG_COMMA char *nameForAddress, sqInt nameSize);
sqInt sqResolverAddrLookupResultSize(PLUGIN_IARG);
sqInt sqResolverError(PLUGIN_IARG);
sqInt sqResolverLocalAddress(PLUGIN_IARG);
sqInt sqResolverNameLookupResult(PLUGIN_IARG);
void  sqResolverStartAddrLookup(PLUGIN_IARG_COMMA sqInt address);
void  sqResolverStartNameLookup(PLUGIN_IARG_COMMA char *hostName, sqInt nameSize);
sqInt sqResolverStatus(PLUGIN_IARG);

void sqSocketAbortConnection(PLUGIN_IARG_COMMA SocketPtr s);
void sqSocketCloseConnection(PLUGIN_IARG_COMMA SocketPtr s);
sqInt sqSocketConnectionStatus(PLUGIN_IARG_COMMA SocketPtr s);
void  sqSocketConnectToPort(PLUGIN_IARG_COMMA SocketPtr s, sqInt addr, sqInt port);
void  sqSocketCreateNetTypeSocketTypeRecvBytesSendBytesSemaID(PLUGIN_IARG_COMMA SocketPtr s, sqInt netType, sqInt socketType, sqInt recvBufSize, sqInt sendBufSize, sqInt semaIndex);
void  sqSocketCreateNetTypeSocketTypeRecvBytesSendBytesSemaIDReadSemaIDWriteSemaID(PLUGIN_IARG_COMMA SocketPtr s, sqInt netType, sqInt socketType, sqInt recvBufSize, sqInt sendBufSize, sqInt semaIndex, sqInt readSemaIndex, sqInt writeSemaIndex);
void  sqSocketDestroy(PLUGIN_IARG_COMMA SocketPtr s);
sqInt sqSocketError(PLUGIN_IARG_COMMA SocketPtr s);
void  sqSocketListenOnPort(PLUGIN_IARG_COMMA SocketPtr s, sqInt port);
sqInt sqSocketLocalAddress(PLUGIN_IARG_COMMA SocketPtr s);
sqInt sqSocketLocalPort(PLUGIN_IARG_COMMA SocketPtr s);
sqInt sqSocketReceiveDataAvailable(PLUGIN_IARG_COMMA SocketPtr s);
sqInt sqSocketReceiveDataBufCount(PLUGIN_IARG_COMMA SocketPtr s, char *buf, sqInt bufSize);
sqInt sqSocketRemoteAddress(PLUGIN_IARG_COMMA SocketPtr s);
sqInt sqSocketRemotePort(PLUGIN_IARG_COMMA SocketPtr s);
sqInt sqSocketSendDataBufCount(PLUGIN_IARG_COMMA SocketPtr s, char *buf, sqInt bufSize);
sqInt sqSocketSendDone(PLUGIN_IARG_COMMA SocketPtr s);
/* ar 7/16/1999: New primitives for accept().  Note: If accept() calls are not supported simply make the calls fail and the old connection style will be used. */
void  sqSocketListenOnPortBacklogSize(PLUGIN_IARG_COMMA SocketPtr s, sqInt port, sqInt backlogSize);
void  sqSocketListenOnPortBacklogSizeInterface(PLUGIN_IARG_COMMA SocketPtr s, sqInt port, sqInt backlogSize, sqInt addr);
void  sqSocketAcceptFromRecvBytesSendBytesSemaID(PLUGIN_IARG_COMMA SocketPtr s, SocketPtr serverSocket, sqInt recvBufSize, sqInt sendBufSize, sqInt semaIndex);
void  sqSocketAcceptFromRecvBytesSendBytesSemaIDReadSemaIDWriteSemaID(PLUGIN_IARG_COMMA SocketPtr s, SocketPtr serverSocket, sqInt recvBufSize, sqInt sendBufSize, sqInt semaIndex, sqInt readSemaIndex, sqInt writeSemaIndex);
sqInt sqSocketReceiveUDPDataBufCountaddressportmoreFlag(PLUGIN_IARG_COMMA SocketPtr s, char *buf, sqInt bufSize,  sqInt *address,  sqInt *port, sqInt *moreFlag);
sqInt sqSockettoHostportSendDataBufCount(PLUGIN_IARG_COMMA SocketPtr s, sqInt address, sqInt port, char *buf, sqInt bufSize);
sqInt sqSocketSetOptionsoptionNameStartoptionNameSizeoptionValueStartoptionValueSizereturnedValue(PLUGIN_IARG_COMMA SocketPtr s, char *optionName, sqInt optionNameSize, char *optionValue, sqInt optionValueSize, sqInt *result);
sqInt sqSocketGetOptionsoptionNameStartoptionNameSizereturnedValue(PLUGIN_IARG_COMMA SocketPtr s, char *optionName, sqInt optionNameSize, sqInt *result);
/* tpr 4/12/06 add declarations for two new socket routines */
void sqSocketBindToPort(PLUGIN_IARG_COMMA SocketPtr s, int addr, int port);
void sqSocketSetReusable(PLUGIN_IARG_COMMA SocketPtr s);


