/* Header file for AsynchFile plugin */
/* module initialization/shutdown */
int asyncFileInit(void);
int asyncFileShutdown(void);

/*** Experimental Asynchronous File I/O ***/
typedef struct {
	int			sessionID;
	void		*state;
} AsyncFile;

int asyncFileClose(AsyncFile *f);
int asyncFileOpen(AsyncFile *f, int fileNamePtr, int fileNameSize, int writeFlag, int semaIndex);
int asyncFileRecordSize();
int asyncFileReadResult(AsyncFile *f, int bufferPtr, int bufferSize);
int asyncFileReadStart(AsyncFile *f, int fPosition, int count);
int asyncFileWriteResult(AsyncFile *f);
int asyncFileWriteStart(AsyncFile *f, int fPosition, int bufferPtr, int bufferSize);

/*** security traps ***/
/* following is equivalent ioCanOpenFileOfSize() and should really be handled from there */
int ioCanOpenAsyncFileOfSizeWritable(char* fileNameIndex, int fileNameSize, int writeFlag);

#ifdef DISABLE_SECURITY
#define ioCanOpenAsyncFileOfSizeWritable(index, size, flag) 1
#endif

