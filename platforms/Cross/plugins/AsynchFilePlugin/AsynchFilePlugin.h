/* Header file for AsynchFile plugin */
/* module initialization/shutdown */
int asyncFileInit(void);
int asyncFileShutdown(void);

/*** Experimental Asynchronous File I/O ***/
typedef struct {
	int			sessionID;
	void		*state;
} AsyncFile;

int asyncFileClose _iargs( AsyncFile *f);
int asyncFileOpen _iargs( AsyncFile *f, int fileNamePtr, int fileNameSize, int writeFlag, int semaIndex);
int asyncFileRecordSize();
int asyncFileReadResult _iargs(AsyncFile *f, int bufferPtr, int bufferSize);
int asyncFileReadStart _iargs(AsyncFile *f, int fPosition, int count);
int asyncFileWriteResult _iargs(AsyncFile *f);
int asyncFileWriteStart _iargs(AsyncFile *f, int fPosition, int bufferPtr, int bufferSize);
