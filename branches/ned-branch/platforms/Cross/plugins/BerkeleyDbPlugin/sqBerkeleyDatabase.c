#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define __time_t

#include "BerkeleyDbPlugin.h"

// Note: this is the main Berkeley DB header, typically found in /usr/include
// in Unix systems.
#include "db.h"

// Well known return codes
//   100 indicates a malloc failure
//    99 indicates invalid Database handle  
//    98 indicates invalid DbCursor handle

#if DB_VERSION_MAJOR > 3
 #define setmalloc(X) dbp->set_alloc(dbp,X,realloc,free)
#else
 #define setmalloc(X) dbp->set_malloc(dbp,X)
#endif

#define MALLOC(X,TYPE,SIZE) \
    X = TYPE malloc(SIZE); if(!X) return 100

#define DBCALL(X) do {                   \
    returnCode = X;                      \
    if(returnCode != 0) {                \
      *errmsg = db_strerror(returnCode); \
      return -1;                         \
    }                                    \
  } while(0)

#define GETDBP(dbp,db)                   \
  if(!((db != 0) &&                      \
     (thisDbSession != 0) &&             \
     (db->privateDbPtr != 0) &&          \
     (db->sessionID == thisDbSession)))  \
       return 99;                        \
  dbp = (DB *) db->privateDbPtr

#define GETDBCP(dbcp,dbc)                \
  if(!((dbc != 0) &&                     \
     (thisDbSession != 0) &&             \
     (dbc->privateDbcPtr != 0) &&        \
     (dbc->sessionID == thisDbSession))) \
       return 98;                        \
  dbcp = (DBC *) dbc->privateDbcPtr

#define SYMDEF(sym) { #sym, sym }

static int thisDbSession = 0;
static void* (*sq_malloc)(size_t size);
static int returnCode;

typedef struct { const char *name; unsigned int value; } symdef;
static symdef dbSymbols[];

//****** Plugin Initialization/Shutdown ******
int sqDbInit(void *(*squeak_malloc_function)(size_t size)) 
{
  int major, minor;
  char *version;

  if (0 != thisDbSession) return 1; /* Already initialized */
  sq_malloc = squeak_malloc_function;
  thisDbSession = clock() + time(0);
  if (0 == thisDbSession) thisDbSession = 1;
  db_version(&major, &minor, NULL);

  // The following check ensures that the version of the db.h 
  // header file that we were compiled against and the version 
  // of the libdb that we are bound with are in agreement 
  if((major != DB_VERSION_MAJOR) || (minor != DB_VERSION_MINOR)) {
    fprintf(stderr,"Error: BerkeleyDB version mismatch\n");
    fprintf(stderr,"       Header file expected %d.%d but got %d.%d\n", 
            DB_VERSION_MAJOR, DB_VERSION_MINOR, major, minor); 
    return 0;
  }
  return 1;
}

int sqDbShutdown() 
{
  return 0;
}

//****** Database methods ******
int sqDbIsValid(Database *db) 
{
  return 
    (db != 0) && 
    (thisDbSession != 0) && 
    (db->privateDbPtr != 0) && 
    (db->sessionID == thisDbSession);
}

int sqDbOpen(Database *db, char *fileNamePtr, int fileNameSize, int createFlag, char **errmsg)
{
  DB *dbp;
  char *fname;
  u_int32_t flags;

  // Create the DB pointer
  DBCALL(db_create(&dbp,NULL,0));

  // Setup our Squeak database structure
  db->sessionID = thisDbSession;
  db->privateDbPtr = (void *) dbp; 

  // Set the malloc function
  DBCALL(setmalloc(sq_malloc));

  // Copy the file name out of the Squeak String
  MALLOC(fname, (char*), fileNameSize + 1);
  if(!fname) return 1;
  memset((void *) (fname + fileNameSize), 0, 1);
  memcpy((void *) fname, fileNamePtr, fileNameSize);

  // Set the flags for the call to db->open
  if(createFlag) {
    flags = DB_CREATE;
  } else {
    flags = 0;
  }

  // Make the call to db->open
#if (DB_VERSION_MAJOR > 3) && (DB_VERSION_MINOR > 0)
  DBCALL(dbp->open(dbp,NULL,fname,NULL,DB_BTREE,flags,0664); free(fname));
#else
  DBCALL(dbp->open(dbp,fname,NULL,DB_BTREE,flags,0664); free(fname));
#endif
  return 0;
}

int sqDbClose(Database *db, char **errmsg)
{
  DB *dbp;
  
  GETDBP(dbp,db);
  db->sessionID = 0; /* mark this db struct invalid */
  db->privateDbPtr = NULL;
  DBCALL(dbp->close(dbp,0));
  return 0;
}

int sqDbCommit(Database *db, char **errmsg)
{
  DB *dbp;

  GETDBP(dbp,db);
  DBCALL(dbp->sync(dbp, 0));
  return 0;
}

int sqDbVerify(Database *db, char **errmsg)
{
  DB *dbp;

  GETDBP(dbp,db);
  DBCALL(dbp->verify(dbp,NULL,NULL,NULL,0));
  return 0;
}

int sqDbSetFlags(Database *db, unsigned int flags, char **errmsg)
{
  DB *dbp;

  GETDBP(dbp,db);
  DBCALL(dbp->set_flags(dbp,flags));
  return 0;
}

int sqDbAt(Database *db, void *key, int keySize, void **value, int *valueSize, char **errmsg)
{
  DBT k, v;
  DB *dbp;
  int ret;

  GETDBP(dbp,db);
  memset(&k, 0, sizeof(k));
  memset(&v, 0, sizeof(v));

  k.data = key;
  k.size = keySize;
  v.flags = DB_DBT_MALLOC;

  ret = dbp->get(dbp, NULL, &k, &v, 0);
  if(ret != 0) {
    *value = NULL;
    *valueSize = 0;
  }
  if(ret == DB_NOTFOUND) return 1;
  DBCALL(ret);
  *value = v.data;
  *valueSize = v.size;
  return 0;
}

int sqDbAtPut(Database *db, void *key, int keySize, void *value, int valueSize, char **errmsg)
{
  DBT k, v;
  DB *dbp;
  int ret;

  GETDBP(dbp,db);
  memset(&k, 0, sizeof(k));
  memset(&v, 0, sizeof(v));

  k.data = key;
  k.size = keySize;
  v.data = value;
  v.size = valueSize;

  DBCALL(dbp->put(dbp, NULL, &k, &v, 0));
  return 0;
}

int sqDbRemoveKey(Database *db, void *key, int keySize, char **errmsg)
{
  DBT k;
  DB *dbp;
  int ret;

  GETDBP(dbp,db);
  memset(&k, 0, sizeof(k));
  k.data = key;
  k.size = keySize;
  ret = dbp->del(dbp, NULL, &k, 0);
  if(ret == DB_NOTFOUND) return 1;
  DBCALL(ret);
  return 0;
}

//****** Database Cursor methods ******
int sqDbCursor(Database *db, DbCursor *dbc, char **errmsg)
{
  DB *dbp;
  DBC *dbcp;

  GETDBP(dbp,db);
  DBCALL(dbp->cursor(dbp, NULL, &dbcp, 0));
  dbc->sessionID = thisDbSession;
  dbc->privateDbcPtr = (void *) dbcp;
  return 0;
}

// This lets us access the DB in special ways (like getting the next dup)
int sqDbCursorGet(DbCursor *dbc, void **key, int *keySize, void **value, int *valueSize, int flags, char **errmsg)
{
  DBC *dbcp;
  DBT k, v;
  int ret;

  GETDBCP(dbcp,dbc);
  memset(&k, 0, sizeof(k));
  memset(&v, 0, sizeof(v));
  ret = dbcp->c_get(dbcp, &k, &v, flags);

  if (ret != 0)
  {
    *value = NULL;
    *valueSize = 0;
  }

  switch (ret)
  {
    case 0:
      *key = k.data;
      *keySize = k.size;
      *value = v.data;
      *valueSize = v.size;
      return 0;
      break;

    case DB_NOTFOUND:
    case DB_KEYEMPTY:
      return 1;
      break;

    default:
      DBCALL(ret);
      break;
  }

  return -1;  /* NOTREACHED */
}

int sqDbCursorNext(DbCursor *dbc, void **key, int *keySize, void **value, int *valueSize,char **errmsg)
{
  DBC *dbcp;
  DBT k, v;
  int ret;

  GETDBCP(dbcp,dbc);
  memset(&k, 0, sizeof(k));
  memset(&v, 0, sizeof(v));
  ret = dbcp->c_get(dbcp, &k, &v, DB_NEXT);

  /* mark cursor as invalid because we've hit the end. */
  if(ret == DB_NOTFOUND) {  
    dbc->sessionID = 0;
    dbc->privateDbcPtr = NULL;
    *keySize = 0;
    return 1;
  }

  DBCALL(ret);
  *key = k.data;
  *keySize = k.size;
  *value = v.data;
  *valueSize = v.size;
  return 0;
}

int sqDbCursorPut(DbCursor *dbc, void *key, int keySize, void *value, int valueSize, int flags, char **errmsg)
{
  DBC *dbcp;
  DBT k, v;
  int ret;

  GETDBCP(dbcp,dbc);
  memset(&k, 0, sizeof(k));
  memset(&v, 0, sizeof(v));

  k.data = key;
  k.size = keySize;
  v.data = value;
  v.size = valueSize;

  ret = dbcp->c_put(dbcp, &k, &v, flags);

  switch (ret) {
    case DB_NOTFOUND: // current key has been deleted
      break;
    case DB_KEYEXIST: // no duplicate keys; already exists.
      break;
  }

  DBCALL(ret);

  return 0;
}

// Delete the record at the cursor.
// If followed by a sqDbCursorGet(...DB_CURRENT...) you will get a DB_KEYEMPTY
int sqDbCursorDelete(DbCursor *dbc, char **errmsg)
{
  DBC *dbcp;

  GETDBCP(dbcp,dbc);
  DBCALL(dbcp->c_del(dbcp, 0));
  return 0;
}

int sqDbCursorClose(DbCursor *dbc, char **errmsg)
{
  DBC *dbcp;

  GETDBCP(dbcp,dbc);
  DBCALL(dbcp->c_close(dbcp));
  return 0;
}

/* This defines a number of symbols from the DB4 header.
 * TODO: There are probably too many here.
 */
static symdef dbSymbols[] =
{
  // c_get() opcodes: pick one
  SYMDEF(DB_CURRENT),
  SYMDEF(DB_FIRST),
  SYMDEF(DB_GET_BOTH),
  SYMDEF(DB_GET_BOTHC),
  SYMDEF(DB_GET_BOTH_RANGE),
  SYMDEF(DB_GET_RECNO),
  SYMDEF(DB_JOIN_ITEM),
  SYMDEF(DB_LAST),
  SYMDEF(DB_NEXT),
  SYMDEF(DB_NEXT_DUP),
  SYMDEF(DB_NEXT_NODUP),
  SYMDEF(DB_PREV),
  SYMDEF(DB_PREV_NODUP),
  SYMDEF(DB_SET),
  SYMDEF(DB_SET_RANGE),
  SYMDEF(DB_SET_RECNO),
  SYMDEF(DB_UPDATE_SECONDARY),

  // c_put() opcodes: pick one.
  SYMDEF(DB_AFTER),
  SYMDEF(DB_BEFORE),
  SYMDEF(DB_KEYFIRST),
  SYMDEF(DB_KEYLAST),
  SYMDEF(DB_NODUPDATA),

  // op flags (or'd with c_get flags)
  SYMDEF(DB_DIRTY_READ),
  SYMDEF(DB_FLUSH),
  SYMDEF(DB_MULTIPLE),
  SYMDEF(DB_MULTIPLE_KEY),
  SYMDEF(DB_RMW),

  // set_flags
  SYMDEF(DB_DUP),
  SYMDEF(DB_DUPSORT),
  SYMDEF(DB_RECNUM),
  SYMDEF(DB_RENUMBER),
  SYMDEF(DB_REVSPLITOFF),
  SYMDEF(DB_SNAPSHOT),
};

#define NUMBER_OF_SYMBOLS (sizeof(dbSymbols)/sizeof(symdef))

/* Symbol lookups */
static int
symbolCompare(const void *a, const void *b)
{
  return strcmp(((const symdef *) a)->name, ((const symdef *) b)->name);
}

/* Return the value of the given #define in *retval.
 * Return value is 1 on success.
 */
int
sqDbSymbolValue(const char *namePtr, int nameSize, unsigned int *retval)
{
  symdef *where;
  char *sname;
  static int sorted = 0;

  /* Copy the file name out of the Squeak String */
  MALLOC(sname, (char *), nameSize + 1);
  if (!sname)
    return -1;
  strncpy(sname, namePtr, nameSize);
  sname[nameSize] = 0;

  if (!sorted)
  {
    qsort(dbSymbols, NUMBER_OF_SYMBOLS, sizeof(symdef), &symbolCompare);
    sorted = 1;
  }

  where = (symdef *)
    bsearch(sname, dbSymbols, NUMBER_OF_SYMBOLS, sizeof(symdef), &symbolCompare);

  if (where != NULL)
  {
    *retval = where->value;
    return 1;
  }
  else
    return 0;
}

/* vim: ts=2 et ff=dos
 */
