/* Header file for database plugin */

typedef struct {
	int		sessionID;
	void		*privateDbPtr;
} Database, *DatabasePtr;

typedef struct {
	int		sessionID;
	void		*privateDbcPtr;
} DbCursor, *DbCursorPtr;

int sqDbInit(void *(*sq_malloc)(size_t size) );
int sqDbShutdown(void);
int sqDbIsValid(Database*);
int sqDbOpen(Database*,char*,int,int,char**);
int sqDbClose(Database*,char**);
int sqDbCommit(Database*,char**);
int sqDbVerify(Database*,char**);
int sqDbSetFlags(Database *db, unsigned int flags, char **errmsg);
int sqDbAt(Database*,void*,int,void**,int*,char**);
int sqDbAtPut(Database*,void*,int,void*,int,char**);
int sqDbRemoveKey(Database*,void*,int,char**);

int sqDbSymbolValue(const char* namePtr, int nameSize, unsigned int *retval);

int sqDbCursor(Database*,DbCursor*,char**);
int sqDbCursorGet(DbCursor*,void**,int*,void**,int*,int,char**);
int sqDbCursorNext(DbCursor*,void**,int*,void**,int*,char**);
int sqDbCursorPut(DbCursor *dbc, void *key, int keySize, void *value, int valueSize, int flags, char **errmsg);
int sqDbCursorDelete(DbCursor *dbc, char **errmsg);
int sqDbCursorClose(DbCursor*,char**);
