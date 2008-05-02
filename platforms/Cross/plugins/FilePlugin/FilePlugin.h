/****************************************************************************
*   PROJECT: Common include
*   FILE:    FilePlugin.h
*   CONTENT: 
*
*   AUTHOR:  
*   ADDRESS: 
*   EMAIL:   
*   RCSID:   $Id$
*
*	2005-03-26 IKP fix unaligned accesses to file member
*	2004-06-10 IKP 64-bit cleanliness
*	01/22/2002 JMM change off_t to squeakOffsetFileType
*/
/* File support definitions */

/* squeak file record; see sqFilePrims.c for details */
typedef struct {
	int			 sessionID;	/* ikp: must be first */
	void			*file;
	int			 writable;
	squeakFileOffsetType	 fileSize;
	int			 lastOp;	/* 0 = uncommitted, 1 = read, 2 = write */
} SQFile;

/* file i/o */

sqInt   sqFileAtEnd(PLUGIN_IARG_COMMA SQFile *f);
sqInt   sqFileClose(PLUGIN_IARG_COMMA SQFile *f);
sqInt   sqFileDeleteNameSize(PLUGIN_IARG_COMMA char* sqFileNameIndex, sqInt sqFileNameSize);
squeakFileOffsetType sqFileGetPosition(PLUGIN_IARG_COMMA SQFile *f);
sqInt   sqFileInit(void);
sqInt   sqFileShutdown(void);
sqInt   sqFileOpen(PLUGIN_IARG_COMMA SQFile *f, char* sqFileNameIndex, sqInt sqFileNameSize, sqInt writeFlag);
size_t  sqFileReadIntoAt(PLUGIN_IARG_COMMA SQFile *f, size_t count, char* byteArrayIndex, size_t startIndex);
sqInt   sqFileRenameOldSizeNewSize(PLUGIN_IARG_COMMA char* oldNameIndex, sqInt oldNameSize, char* newNameIndex, sqInt newNameSize);
sqInt   sqFileSetPosition(PLUGIN_IARG_COMMA SQFile *f, squeakFileOffsetType position);
squeakFileOffsetType sqFileSize(PLUGIN_IARG_COMMA SQFile *f);
sqInt   sqFileValid(PLUGIN_IARG_COMMA SQFile *f);
size_t  sqFileWriteFromAt(PLUGIN_IARG_COMMA SQFile *f, size_t count, char* byteArrayIndex, size_t startIndex);
sqInt   sqFileFlush(PLUGIN_IARG_COMMA SQFile *f);
sqInt   sqFileTruncate(PLUGIN_IARG_COMMA SQFile *f,squeakFileOffsetType offset);
sqInt   sqFileThisSession(void);

/* directories */

sqInt dir_Create(char *pathString, sqInt pathStringLength);
sqInt dir_Delete(char *pathString, sqInt pathStringLength);
sqInt dir_Delimitor(void);
sqInt dir_Lookup(char *pathString, sqInt pathStringLength, sqInt index,
		/* outputs: */
		char *name, sqInt *nameLength, sqInt *creationDate, sqInt *modificationDate,
		sqInt *isDirectory, squeakFileOffsetType *sizeIfFile);
sqInt dir_PathToWorkingDir(char *pathName, sqInt pathNameMax);
sqInt dir_SetMacFileTypeAndCreator(char *filename, sqInt filenameSize, char *fType, char *fCreator);
sqInt dir_GetMacFileTypeAndCreator(char *filename, sqInt filenameSize, char *fType, char *fCreator);
