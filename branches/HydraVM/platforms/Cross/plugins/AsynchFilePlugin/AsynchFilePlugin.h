/* Header file for AsynchFile plugin */
/* module initialization/shutdown */
int asyncFileInit(void);
int asyncFileShutdown(void);

/*** Experimental Asynchronous File I/O ***/
typedef struct {
	int			sessionID;
	void		*state;
} AsyncFile;

int asyncFileClose(PLUGIN_IARG_COMMA  AsyncFile *f);
int asyncFileOpen(PLUGIN_IARG_COMMA  AsyncFile *f, int fileNamePtr, int fileNameSize, int writeFlag, int semaIndex);
int asyncFileRecordSize();
int asyncFileReadResult(PLUGIN_IARG_COMMA AsyncFile *f, int bufferPtr, int bufferSize);
int asyncFileReadStart(PLUGIN_IARG_COMMA AsyncFile *f, int fPosition, int count);
int asyncFileWriteResult(PLUGIN_IARG_COMMA AsyncFile *f);
int asyncFileWriteStart(PLUGIN_IARG_COMMA AsyncFile *f, int fPosition, int bufferPtr, int bufferSize);
