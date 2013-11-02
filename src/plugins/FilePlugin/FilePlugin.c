/* Automatically generated by
	VMPluginCodeGenerator VMMaker.oscog-eem.496 uuid: f0401045-f2f7-470e-9940-3535be9c0334
   from
	FilePlugin VMMaker.oscog-eem.496 uuid: f0401045-f2f7-470e-9940-3535be9c0334
 */
static char __buildInfo[] = "FilePlugin VMMaker.oscog-eem.496 uuid: f0401045-f2f7-470e-9940-3535be9c0334 " __DATE__ ;



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Default EXPORT macro that does nothing (see comment in sq.h): */
#define EXPORT(returnType) returnType

/* Do not include the entire sq.h file but just those parts needed. */
/*  The virtual machine proxy definition */
#include "sqVirtualMachine.h"
/* Configuration options */
#include "sqConfig.h"
/* Platform specific definitions */
#include "sqPlatformSpecific.h"

#define true 1
#define false 0
#define null 0  /* using 'null' because nil is predefined in Think C */
#ifdef SQUEAK_BUILTIN_PLUGIN
#undef EXPORT
// was #undef EXPORT(returnType) but screws NorCroft cc
#define EXPORT(returnType) static returnType
#endif

#include "FilePlugin.h"
#include "sqMemoryAccess.h"


/*** Constants ***/
#define BytesPerWord 4
#define COGMTVM 0
#define DirBadPath 2
#define DirNoMoreEntries 1
#define PrimErrBadArgument 3
#define PrimErrBadIndex 4
#define PrimErrNoMemory 9
#define PrimErrObjectMayMove 14
#define PrimErrUnsupported 7
#define PrimNoErr 0


/*** Function Prototypes ***/
static sqInt asciiDirectoryDelimiter(void);
EXPORT(sqInt) fileOpenNamesizewritesecure(char *nameIndex, sqInt nameSize, sqInt writeFlag, sqInt secureFlag);
sqInt fileRecordSize(void);
SQFile * fileValueOf(sqInt objectPointer);
static VirtualMachine * getInterpreter(void);
EXPORT(const char*) getModuleName(void);
EXPORT(sqInt) getThisSession(void);
static sqInt halt(void);
EXPORT(sqInt) initialiseModule(void);
static sqInt makeDirEntryNamesizecreateDatemodDateisDirfileSize(char *entryName, sqInt entryNameSize, sqInt createDate, sqInt modifiedDate, sqInt dirFlag, squeakFileOffsetType  fileSize);
EXPORT(sqInt) moduleUnloaded(char *aModuleName);
static sqInt msg(char *s);
EXPORT(sqInt) primitiveDirectoryCreate(void);
EXPORT(sqInt) primitiveDirectoryDelete(void);
EXPORT(sqInt) primitiveDirectoryDelimitor(void);
EXPORT(sqInt) primitiveDirectoryEntry(void);
EXPORT(sqInt) primitiveDirectoryGetMacTypeAndCreator(void);
EXPORT(sqInt) primitiveDirectoryLookup(void);
EXPORT(sqInt) primitiveDirectorySetMacTypeAndCreator(void);
EXPORT(sqInt) primitiveDisableFileAccess(void);
EXPORT(sqInt) primitiveFileAtEnd(void);
EXPORT(sqInt) primitiveFileClose(void);
EXPORT(sqInt) primitiveFileDelete(void);
EXPORT(sqInt) primitiveFileFlush(void);
EXPORT(sqInt) primitiveFileGetPosition(void);
EXPORT(sqInt) primitiveFileOpen(void);
EXPORT(sqInt) primitiveFileRead(void);
EXPORT(sqInt) primitiveFileRename(void);
EXPORT(sqInt) primitiveFileSetPosition(void);
EXPORT(sqInt) primitiveFileSize(void);
EXPORT(sqInt) primitiveFileStdioHandles(void);
EXPORT(sqInt) primitiveFileTruncate(void);
EXPORT(sqInt) primitiveFileWrite(void);
EXPORT(sqInt) primitiveHasFileAccess(void);
EXPORT(sqInt) setInterpreter(struct VirtualMachine*anInterpreter);
EXPORT(sqInt) setMacFileTypeAndCreator(char *fileName, char *typeString, char *creatorString);
EXPORT(sqInt) shutdownModule(void);


/*** Variables ***/

#if !defined(SQUEAK_BUILTIN_PLUGIN)
static sqInt (*booleanValueOf)(sqInt obj);
static sqInt (*byteSizeOf)(sqInt oop);
static sqInt (*characterObjectOf)(sqInt characterCode);
static sqInt (*characterTable)(void);
static sqInt (*classArray)(void);
static sqInt (*classByteArray)(void);
static sqInt (*classString)(void);
static sqInt (*failed)(void);
static sqInt (*falseObject)(void);
static sqInt (*fetchPointerofObject)(sqInt index, sqInt oop);
static void * (*firstIndexableField)(sqInt oop);
static sqInt (*fullGC)(void);
static sqInt (*instantiateClassindexableSize)(sqInt classPointer, sqInt size);
static sqInt (*integerObjectOf)(sqInt value);
static void * (*ioLoadFunctionFrom)(char *functionName, char *moduleName);
static sqInt (*isBytes)(sqInt oop);
static sqInt (*isIntegerObject)(sqInt objectPointer);
static sqInt (*isWords)(sqInt oop);
static sqInt (*isWordsOrBytes)(sqInt oop);
static sqInt (*nilObject)(void);
static sqInt (*pop)(sqInt nItems);
static sqInt (*popthenPush)(sqInt nItems, sqInt oop);
static sqInt (*popRemappableOop)(void);
static sqInt (*positive32BitIntegerFor)(sqInt integerValue);
static sqInt (*positive32BitValueOf)(sqInt oop);
static sqLong (*positive64BitIntegerFor)(sqLong integerValue);
static sqLong (*positive64BitValueOf)(sqInt oop);
static sqInt (*primitiveFail)(void);
static sqInt (*primitiveFailFor)(sqInt reasonCode);
static sqInt (*primitiveFailureCode)(void);
static sqInt (*pushBool)(sqInt trueOrFalse);
static sqInt (*pushRemappableOop)(sqInt oop);
static sqInt (*slotSizeOf)(sqInt oop);
static sqInt (*stackIntegerValue)(sqInt offset);
static sqInt (*stackValue)(sqInt offset);
static sqInt (*storePointerofObjectwithValue)(sqInt index, sqInt oop, sqInt valuePointer);
static sqInt (*tenuringIncrementalGC)(void);
static sqInt (*topRemappableOop)(void);
static sqInt (*trueObject)(void);
#else /* !defined(SQUEAK_BUILTIN_PLUGIN) */
extern sqInt booleanValueOf(sqInt obj);
extern sqInt byteSizeOf(sqInt oop);
#if VM_PROXY_MAJOR > 1 || (VM_PROXY_MAJOR == 1 && VM_PROXY_MINOR >= 13)
extern sqInt characterObjectOf(sqInt characterCode);
#else
# define characterObjectOf(characterCode) 0
#endif
extern sqInt characterTable(void);
extern sqInt classArray(void);
extern sqInt classByteArray(void);
extern sqInt classString(void);
extern sqInt failed(void);
extern sqInt falseObject(void);
extern sqInt fetchPointerofObject(sqInt index, sqInt oop);
extern void * firstIndexableField(sqInt oop);
extern sqInt fullGC(void);
extern sqInt instantiateClassindexableSize(sqInt classPointer, sqInt size);
extern sqInt integerObjectOf(sqInt value);
extern void * ioLoadFunctionFrom(char *functionName, char *moduleName);
extern sqInt isBytes(sqInt oop);
extern sqInt isIntegerObject(sqInt objectPointer);
extern sqInt isWords(sqInt oop);
extern sqInt isWordsOrBytes(sqInt oop);
extern sqInt nilObject(void);
extern sqInt pop(sqInt nItems);
extern sqInt popthenPush(sqInt nItems, sqInt oop);
extern sqInt popRemappableOop(void);
extern sqInt positive32BitIntegerFor(sqInt integerValue);
extern sqInt positive32BitValueOf(sqInt oop);
extern sqLong positive64BitIntegerFor(sqLong integerValue);
extern sqLong positive64BitValueOf(sqInt oop);
extern sqInt primitiveFail(void);
extern sqInt primitiveFailFor(sqInt reasonCode);
extern sqInt primitiveFailureCode(void);
extern sqInt pushBool(sqInt trueOrFalse);
extern sqInt pushRemappableOop(sqInt oop);
extern sqInt slotSizeOf(sqInt oop);
extern sqInt stackIntegerValue(sqInt offset);
extern sqInt stackValue(sqInt offset);
extern sqInt storePointerofObjectwithValue(sqInt index, sqInt oop, sqInt valuePointer);
extern sqInt tenuringIncrementalGC(void);
extern sqInt topRemappableOop(void);
extern sqInt trueObject(void);
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"FilePlugin VMMaker.oscog-eem.496 (i)"
#else
	"FilePlugin VMMaker.oscog-eem.496 (e)"
#endif
;
static void * sCCPfn;
static void * sCDFfn;
static void * sCDPfn;
static void * sCGFTfn;
static void * sCLPfn;
static void * sCOFfn;
static void * sCRFfn;
static void * sCSFTfn;
static void * sDFAfn;
static void * sHFAfn;


static sqInt
asciiDirectoryDelimiter(void)
{
	return dir_Delimitor();
}


/*	Open the named file, possibly checking security. Answer the file oop. */

EXPORT(sqInt)
fileOpenNamesizewritesecure(char *nameIndex, sqInt nameSize, sqInt writeFlag, sqInt secureFlag)
{
    SQFile *file;
    sqInt fileOop;
    sqInt okToOpen;

	fileOop = instantiateClassindexableSize(classByteArray(), sizeof(SQFile));
	/* begin fileValueOf: */
	if (!((isBytes(fileOop))
		 && ((byteSizeOf(fileOop)) == (sizeof(SQFile))))) {
		primitiveFail();
		file = null;
		goto l1;
	}
	file = firstIndexableField(fileOop);
l1:	/* end fileValueOf: */;
	if (!(failed())) {
		if (secureFlag) {

			/* If the security plugin can be loaded, use it to check for permission.
			   If not, assume it's ok */

			if (sCOFfn != 0) {
				okToOpen = ((sqInt (*) (char *, sqInt, sqInt)) sCOFfn)(nameIndex, nameSize, writeFlag);
				if (!okToOpen) {
					primitiveFail();
				}
			}
		}
	}
	if (!(failed())) {
		sqFileOpen(file, nameIndex, nameSize, writeFlag);
	}
	return fileOop;
}


/*	Return the size of a Smalltalk file record in bytes. */

sqInt
fileRecordSize(void)
{
	return sizeof(SQFile);
}


/*	Return a pointer to the first byte of of the file record within the given
	Smalltalk object, or nil if objectPointer is not a file record.
 */

SQFile *
fileValueOf(sqInt objectPointer)
{
	if (!((isBytes(objectPointer))
		 && ((byteSizeOf(objectPointer)) == (sizeof(SQFile))))) {
		primitiveFail();
		return null;
	}
	return firstIndexableField(objectPointer);
}


/*	Note: This is coded so that plugins can be run from Squeak. */

static VirtualMachine *
getInterpreter(void)
{
	return interpreterProxy;
}


/*	Note: This is hardcoded so it can be run from Squeak.
	The module name is used for validating a module *after*
	it is loaded to check if it does really contain the module
	we're thinking it contains. This is important! */

EXPORT(const char*)
getModuleName(void)
{
	return moduleName;
}


/*	Exported entry point for the VM. Only used by AsynchFilePlugin and needs
	to be reowrked now we have a VM global session Id capability
 */

EXPORT(sqInt)
getThisSession(void)
{
	return sqFileThisSession();
}

static sqInt
halt(void)
{
	;
	return 0;
}

EXPORT(sqInt)
initialiseModule(void)
{
	sCCPfn = ioLoadFunctionFrom("secCanCreatePathOfSize", "SecurityPlugin");
	sCDPfn = ioLoadFunctionFrom("secCanDeletePathOfSize", "SecurityPlugin");
	sCGFTfn = ioLoadFunctionFrom("secCanGetFileTypeOfSize", "SecurityPlugin");
	sCLPfn = ioLoadFunctionFrom("secCanListPathOfSize", "SecurityPlugin");
	sCSFTfn = ioLoadFunctionFrom("secCanSetFileTypeOfSize", "SecurityPlugin");
	sDFAfn = ioLoadFunctionFrom("secDisableFileAccess", "SecurityPlugin");
	sCDFfn = ioLoadFunctionFrom("secCanDeleteFileOfSize", "SecurityPlugin");
	sCOFfn = ioLoadFunctionFrom("secCanOpenFileOfSizeWritable", "SecurityPlugin");
	sCRFfn = ioLoadFunctionFrom("secCanRenameFileOfSize", "SecurityPlugin");
	sHFAfn = ioLoadFunctionFrom("secHasFileAccess", "SecurityPlugin");
	return sqFileInit();
}

static sqInt
makeDirEntryNamesizecreateDatemodDateisDirfileSize(char *entryName, sqInt entryNameSize, sqInt createDate, sqInt modifiedDate, sqInt dirFlag, squeakFileOffsetType  fileSize)
{
    sqInt createDateOop;
    sqInt fileSizeOop;
    sqInt i;
    sqInt modDateOop;
    sqInt nameString;
    sqInt results;
    char *stringPtr;


	/* allocate storage for results, remapping newly allocated
	   oops in case GC happens during allocation */

	pushRemappableOop(instantiateClassindexableSize(classArray(), 5));
	pushRemappableOop(instantiateClassindexableSize(classString(), entryNameSize));
	pushRemappableOop(positive32BitIntegerFor(createDate));
	pushRemappableOop(positive32BitIntegerFor(modifiedDate));
	pushRemappableOop(positive64BitIntegerFor(fileSize));
	fileSizeOop = popRemappableOop();
	modDateOop = popRemappableOop();
	createDateOop = popRemappableOop();
	nameString = popRemappableOop();

	/* copy name into Smalltalk string */

	results = popRemappableOop();
	stringPtr = firstIndexableField(nameString);
	for (i = 0; i <= (entryNameSize - 1); i += 1) {
		stringPtr[i] = (entryName[i]);
	}
	storePointerofObjectwithValue(0, results, nameString);
	storePointerofObjectwithValue(1, results, createDateOop);
	storePointerofObjectwithValue(2, results, modDateOop);
	if (dirFlag) {
		storePointerofObjectwithValue(3, results, trueObject());
	}
	else {
		storePointerofObjectwithValue(3, results, falseObject());
	}
	storePointerofObjectwithValue(4, results, fileSizeOop);
	return results;
}


/*	The module with the given name was just unloaded.
	Make sure we have no dangling references. */

EXPORT(sqInt)
moduleUnloaded(char *aModuleName)
{
	if ((strcmp(aModuleName, "SecurityPlugin")) == 0) {

		/* The security plugin just shut down. How odd. */

		sCCPfn = (sCDPfn = (sCGFTfn = (sCLPfn = (sCSFTfn = (sDFAfn = (sCDFfn = (sCOFfn = (sCRFfn = (sHFAfn = 0)))))))));
	}
}

static sqInt
msg(char *s)
{
	fprintf(stderr, "\n%s: %s", moduleName, s);
	return 0;
}

EXPORT(sqInt)
primitiveDirectoryCreate(void)
{
    sqInt dirName;
    char *dirNameIndex;
    sqInt dirNameSize;
    sqInt okToCreate;

	dirName = stackValue(0);
	if (!(isBytes(dirName))) {
		return primitiveFail();
	}
	dirNameIndex = firstIndexableField(dirName);

	/* If the security plugin can be loaded, use it to check for permission.
	   If not, assume it's ok */

	dirNameSize = byteSizeOf(dirName);
	if (sCCPfn != 0) {
		okToCreate =  ((sqInt (*)(char *, sqInt))sCCPfn)(dirNameIndex, dirNameSize);
		if (!okToCreate) {
			return primitiveFail();
		}
	}
	if (!(dir_Create(dirNameIndex, dirNameSize))) {
		return primitiveFail();
	}
	pop(1);
}

EXPORT(sqInt)
primitiveDirectoryDelete(void)
{
    sqInt dirName;
    char *dirNameIndex;
    sqInt dirNameSize;
    sqInt okToDelete;

	dirName = stackValue(0);
	if (!(isBytes(dirName))) {
		return primitiveFail();
	}
	dirNameIndex = firstIndexableField(dirName);

	/* If the security plugin can be loaded, use it to check for permission.
	   If not, assume it's ok */

	dirNameSize = byteSizeOf(dirName);
	if (sCDPfn != 0) {
		okToDelete =  ((sqInt (*)(char *, sqInt))sCDPfn)(dirNameIndex, dirNameSize);
		if (!okToDelete) {
			return primitiveFail();
		}
	}
	if (!(dir_Delete(dirNameIndex, dirNameSize))) {
		return primitiveFail();
	}
	pop(1);
}

EXPORT(sqInt)
primitiveDirectoryDelimitor(void)
{
    sqInt ascii;

	ascii = asciiDirectoryDelimiter();
	if ((interpreterProxy->minorVersion()) >= 13) {
		popthenPush(1, characterObjectOf(ascii));
	}
	else {
		if ((ascii >= 0)
		 && (ascii <= 0xFF)) {
			popthenPush(1, fetchPointerofObject(ascii, characterTable()));
		}
		else {
			primitiveFail();
		}
	}
}


/*	Two arguments - directory path, and simple file name;
	returns an array (see primitiveDirectoryLookup) describing the file or
	directory, or nil if it does not exist. 
	Primitive fails if the outer path does not identify a readable directory.
	(This is a lookup-by-name variant of primitiveDirectoryLookup.) */

EXPORT(sqInt)
primitiveDirectoryEntry(void)
{
    sqInt createDate;
    sqInt dirFlag;
    char entryName[256];
    sqInt entryNameSize;
    squeakFileOffsetType fileSize;
    sqInt modifiedDate;
    sqInt okToList;
    sqInt pathName;
    char *pathNameIndex;
    sqInt pathNameSize;
    char *reqNameIndex;
    sqInt reqNameSize;
    sqInt requestedName;
    sqInt status;

	requestedName = stackValue(0);
	pathName = stackValue(1);
	if (!(isBytes(pathName))) {
		return primitiveFail();
	}
	pathNameIndex = firstIndexableField(pathName);
	pathNameSize = byteSizeOf(pathName);
	reqNameIndex = firstIndexableField(requestedName);

	/* If the security plugin can be loaded, use it to check for permission.
	   If not, assume it's ok */

	reqNameSize = byteSizeOf(requestedName);
	if (sCLPfn != 0) {
		okToList = ((sqInt (*)(char *, sqInt))sCLPfn)(pathNameIndex, pathNameSize);
	}
	else {
		okToList = 1;
	}
	if (okToList) {
		status = dir_EntryLookup(pathNameIndex, pathNameSize, reqNameIndex, reqNameSize,
													  entryName, &entryNameSize, &createDate,
													  &modifiedDate, &dirFlag, &fileSize);
	}
	else {
		status = DirNoMoreEntries;
	}
	if (failed()) {
		return null;
	}
	if (status == DirNoMoreEntries) {

		/* no entry; return nil */

		popthenPush(3, nilObject());
		return null;
	}
	if (status == DirBadPath) {
		return primitiveFail();
	}
	popthenPush(3, makeDirEntryNamesizecreateDatemodDateisDirfileSize(entryName, entryNameSize, createDate, modifiedDate, dirFlag, fileSize));
}

EXPORT(sqInt)
primitiveDirectoryGetMacTypeAndCreator(void)
{
    sqInt creatorString;
    char *creatorStringIndex;
    sqInt fileName;
    char *fileNameIndex;
    sqInt fileNameSize;
    sqInt okToGet;
    sqInt typeString;
    char *typeStringIndex;

	creatorString = stackValue(0);
	typeString = stackValue(1);
	fileName = stackValue(2);
	if (!((isBytes(creatorString))
		 && ((byteSizeOf(creatorString)) == 4))) {
		return primitiveFail();
	}
	if (!((isBytes(typeString))
		 && ((byteSizeOf(typeString)) == 4))) {
		return primitiveFail();
	}
	if (!(isBytes(fileName))) {
		return primitiveFail();
	}
	creatorStringIndex = firstIndexableField(creatorString);
	typeStringIndex = firstIndexableField(typeString);
	fileNameIndex = firstIndexableField(fileName);

	/* If the security plugin can be loaded, use it to check for permission.
	   If not, assume it's ok */

	fileNameSize = byteSizeOf(fileName);
	if (sCGFTfn != 0) {
		okToGet =  ((sqInt (*)(char *, sqInt))sCGFTfn)(fileNameIndex, fileNameSize);
		if (!okToGet) {
			return primitiveFail();
		}
	}
	if (!(dir_GetMacFileTypeAndCreator(fileNameIndex, fileNameSize, typeStringIndex, creatorStringIndex))) {
		return primitiveFail();
	}
	pop(3);
}

EXPORT(sqInt)
primitiveDirectoryLookup(void)
{
    sqInt createDate;
    sqInt dirFlag;
    char entryName[256];
    sqInt entryNameSize;
    squeakFileOffsetType fileSize;
    sqInt index;
    sqInt modifiedDate;
    sqInt okToList;
    sqInt pathName;
    char *pathNameIndex;
    sqInt pathNameSize;
    sqInt status;

	index = stackIntegerValue(0);
	pathName = stackValue(1);
	if (!(isBytes(pathName))) {
		return primitiveFail();
	}
	pathNameIndex = firstIndexableField(pathName);

	/* If the security plugin can be loaded, use it to check for permission.
	   If not, assume it's ok */

	pathNameSize = byteSizeOf(pathName);
	if (sCLPfn != 0) {
		okToList = ((sqInt (*)(char *, sqInt))sCLPfn)(pathNameIndex, pathNameSize);
	}
	else {
		okToList = 1;
	}
	if (okToList) {
		status = dir_Lookup(pathNameIndex, pathNameSize, index,
												entryName, &entryNameSize, &createDate,
												&modifiedDate, &dirFlag, &fileSize);
	}
	else {
		status = DirNoMoreEntries;
	}
	if (failed()) {
		return null;
	}
	if (status == DirNoMoreEntries) {

		/* no more entries; return nil */

		popthenPush(3, nilObject());
		return null;
	}
	if (status == DirBadPath) {
		return primitiveFail();
	}
	popthenPush(3, makeDirEntryNamesizecreateDatemodDateisDirfileSize(entryName, entryNameSize, createDate, modifiedDate, dirFlag, fileSize));
}

EXPORT(sqInt)
primitiveDirectorySetMacTypeAndCreator(void)
{
    sqInt creatorString;
    char *creatorStringIndex;
    sqInt fileName;
    char *fileNameIndex;
    sqInt fileNameSize;
    sqInt okToSet;
    sqInt typeString;
    char *typeStringIndex;

	creatorString = stackValue(0);
	typeString = stackValue(1);
	fileName = stackValue(2);
	if (!((isBytes(creatorString))
		 && ((byteSizeOf(creatorString)) == 4))) {
		return primitiveFail();
	}
	if (!((isBytes(typeString))
		 && ((byteSizeOf(typeString)) == 4))) {
		return primitiveFail();
	}
	if (!(isBytes(fileName))) {
		return primitiveFail();
	}
	creatorStringIndex = firstIndexableField(creatorString);
	typeStringIndex = firstIndexableField(typeString);
	fileNameIndex = firstIndexableField(fileName);

	/* If the security plugin can be loaded, use it to check for permission.
	   If not, assume it's ok */

	fileNameSize = byteSizeOf(fileName);
	if (sCSFTfn != 0) {
		okToSet =  ((sqInt (*)(char *, sqInt))sCSFTfn)(fileNameIndex, fileNameSize);
		if (!okToSet) {
			return primitiveFail();
		}
	}
	if (!(dir_SetMacFileTypeAndCreator(fileNameIndex, fileNameSize,typeStringIndex, creatorStringIndex))) {
		return primitiveFail();
	}
	pop(3);
}


/*	If the security plugin can be loaded, use it to turn off file access
	If not, assume it's ok */

EXPORT(sqInt)
primitiveDisableFileAccess(void)
{
	if (sDFAfn != 0) {
		((sqInt (*)(void))sDFAfn)();
	}
}

EXPORT(sqInt)
primitiveFileAtEnd(void)
{
    sqInt atEnd;
    SQFile *file;
    sqInt objectPointer;

	/* begin fileValueOf: */
	objectPointer = stackValue(0);
	if (!((isBytes(objectPointer))
		 && ((byteSizeOf(objectPointer)) == (sizeof(SQFile))))) {
		primitiveFail();
		file = null;
		goto l1;
	}
	file = firstIndexableField(objectPointer);
l1:	/* end fileValueOf: */;
	if (!(failed())) {
		atEnd = sqFileAtEnd(file);
	}
	if (!(failed())) {
		pop(2);
		pushBool(atEnd);
	}
}

EXPORT(sqInt)
primitiveFileClose(void)
{
    SQFile *file;
    sqInt objectPointer;

	/* begin fileValueOf: */
	objectPointer = stackValue(0);
	if (!((isBytes(objectPointer))
		 && ((byteSizeOf(objectPointer)) == (sizeof(SQFile))))) {
		primitiveFail();
		file = null;
		goto l1;
	}
	file = firstIndexableField(objectPointer);
l1:	/* end fileValueOf: */;
	if (!(failed())) {
		sqFileClose(file);
	}
	if (!(failed())) {
		pop(1);
	}
}

EXPORT(sqInt)
primitiveFileDelete(void)
{
    char *nameIndex;
    sqInt namePointer;
    sqInt nameSize;
    sqInt okToDelete;

	namePointer = stackValue(0);
	if (!(isBytes(namePointer))) {
		return primitiveFail();
	}
	nameIndex = firstIndexableField(namePointer);

	/* If the security plugin can be loaded, use it to check for permission.
	   If not, assume it's ok */

	nameSize = byteSizeOf(namePointer);
	if (sCDFfn != 0) {
		okToDelete =  ((sqInt (*)(char *, sqInt))sCDFfn)(nameIndex, nameSize);
		if (!okToDelete) {
			return primitiveFail();
		}
	}
	sqFileDeleteNameSize(nameIndex, nameSize);
	if (!(failed())) {
		pop(1);
	}
}

EXPORT(sqInt)
primitiveFileFlush(void)
{
    SQFile *file;
    sqInt objectPointer;

	/* begin fileValueOf: */
	objectPointer = stackValue(0);
	if (!((isBytes(objectPointer))
		 && ((byteSizeOf(objectPointer)) == (sizeof(SQFile))))) {
		primitiveFail();
		file = null;
		goto l1;
	}
	file = firstIndexableField(objectPointer);
l1:	/* end fileValueOf: */;
	if (!(failed())) {
		sqFileFlush(file);
	}
	if (!(failed())) {
		pop(1);
	}
}

EXPORT(sqInt)
primitiveFileGetPosition(void)
{
    SQFile *file;
    sqInt objectPointer;
    squeakFileOffsetType position;

	/* begin fileValueOf: */
	objectPointer = stackValue(0);
	if (!((isBytes(objectPointer))
		 && ((byteSizeOf(objectPointer)) == (sizeof(SQFile))))) {
		primitiveFail();
		file = null;
		goto l1;
	}
	file = firstIndexableField(objectPointer);
l1:	/* end fileValueOf: */;
	if (!(failed())) {
		position = sqFileGetPosition(file);
	}
	if (!(failed())) {
		popthenPush(2, positive64BitIntegerFor(position));
	}
}

EXPORT(sqInt)
primitiveFileOpen(void)
{
    sqInt filePointer;
    char *nameIndex;
    sqInt namePointer;
    sqInt nameSize;
    sqInt writeFlag;

	writeFlag = booleanValueOf(stackValue(0));
	namePointer = stackValue(1);
	if (!(isBytes(namePointer))) {
		return primitiveFail();
	}
	nameIndex = firstIndexableField(namePointer);
	nameSize = byteSizeOf(namePointer);
	filePointer = fileOpenNamesizewritesecure(nameIndex, nameSize, writeFlag, 1);
	if (!(failed())) {
		popthenPush(3, filePointer);
	}
}

EXPORT(sqInt)
primitiveFileRead(void)
{
    sqInt array;
    sqInt bytesRead;
    size_t count;
    size_t elementSize;
    SQFile *file;
    sqInt objectPointer;
    sqInt retryCount;
    size_t startIndex;

	retryCount = 0;
	count = positive32BitValueOf(stackValue(0));
	startIndex = positive32BitValueOf(stackValue(1));
	while (1) {
		array = stackValue(2);
		/* begin fileValueOf: */
		objectPointer = stackValue(3);
		if (!((isBytes(objectPointer))
			 && ((byteSizeOf(objectPointer)) == (sizeof(SQFile))))) {
			primitiveFail();
			file = null;
			goto l1;
		}
		file = firstIndexableField(objectPointer);
	l1:	/* end fileValueOf: */;
		if ((failed())
		 || (!(isWordsOrBytes(array)))) {
			return primitiveFailFor(PrimErrBadArgument);
		}
		elementSize = (isWords(array)
			? 4
			: 1);
		if (!((startIndex >= 1)
			 && (((startIndex + count) - 1) <= (slotSizeOf(array))))) {
			return primitiveFailFor(PrimErrBadIndex);
		}
		bytesRead = sqFileReadIntoAt(file, count * elementSize, ((char *) (firstIndexableField(array))), (startIndex - 1) * elementSize);
		if (!(((primitiveFailureCode()) == PrimErrObjectMayMove)
 && (((retryCount += 1)) <= 2))) break;
		tenuringIncrementalGC();
		primitiveFailFor(PrimNoErr);
	}
	if (!(failed())) {
		popthenPush(5, integerObjectOf(bytesRead / elementSize));
	}
}

EXPORT(sqInt)
primitiveFileRename(void)
{
    char *newNameIndex;
    sqInt newNamePointer;
    sqInt newNameSize;
    sqInt okToRename;
    char *oldNameIndex;
    sqInt oldNamePointer;
    sqInt oldNameSize;

	newNamePointer = stackValue(0);
	oldNamePointer = stackValue(1);
	if (!((isBytes(newNamePointer))
		 && (isBytes(oldNamePointer)))) {
		return primitiveFail();
	}
	newNameIndex = firstIndexableField(newNamePointer);
	newNameSize = byteSizeOf(newNamePointer);
	oldNameIndex = firstIndexableField(oldNamePointer);

	/* If the security plugin can be loaded, use it to check for rename permission.
	   If not, assume it's ok */

	oldNameSize = byteSizeOf(oldNamePointer);
	if (sCRFfn != 0) {
		okToRename =  ((sqInt (*)(char *, sqInt))sCRFfn)(oldNameIndex, oldNameSize);
		if (!okToRename) {
			return primitiveFail();
		}
	}
	sqFileRenameOldSizeNewSize(oldNameIndex, oldNameSize, newNameIndex, newNameSize);
	if (!(failed())) {
		pop(2);
	}
}

EXPORT(sqInt)
primitiveFileSetPosition(void)
{
    SQFile *file;
    squeakFileOffsetType newPosition;
    sqInt objectPointer;

	if (!(isIntegerObject(stackValue(0)))) {
		if ((byteSizeOf(stackValue(0))) != (sizeof(squeakFileOffsetType))) {
			return primitiveFail();
		}
	}
	newPosition = positive64BitValueOf(stackValue(0));
	/* begin fileValueOf: */
	objectPointer = stackValue(1);
	if (!((isBytes(objectPointer))
		 && ((byteSizeOf(objectPointer)) == (sizeof(SQFile))))) {
		primitiveFail();
		file = null;
		goto l1;
	}
	file = firstIndexableField(objectPointer);
l1:	/* end fileValueOf: */;
	if (!(failed())) {
		sqFileSetPosition(file, newPosition);
	}
	if (!(failed())) {
		pop(2);
	}
}

EXPORT(sqInt)
primitiveFileSize(void)
{
    SQFile *file;
    sqInt objectPointer;
    squeakFileOffsetType size;

	/* begin fileValueOf: */
	objectPointer = stackValue(0);
	if (!((isBytes(objectPointer))
		 && ((byteSizeOf(objectPointer)) == (sizeof(SQFile))))) {
		primitiveFail();
		file = null;
		goto l1;
	}
	file = firstIndexableField(objectPointer);
l1:	/* end fileValueOf: */;
	if (!(failed())) {
		size = sqFileSize(file);
	}
	if (!(failed())) {
		popthenPush(2, positive64BitIntegerFor(size));
	}
}


/*	Answer an Array of file handles for standard in, standard out and standard
	error, with nil in entries that are unvailable, e.g. because the platform
	does not provide
	standard error, etc. Fail if there are no standard i/o facilities on the
	platform or
	if the security plugin denies access or if memory runs out. */

EXPORT(sqInt)
primitiveFileStdioHandles(void)
{
    SQFile fileRecords[3];
    sqInt index;
    sqInt result;
    sqInt validMask;

	if (sHFAfn != 0) {
		if (!( ((sqInt (*)(void))sHFAfn)())) {
			return primitiveFailFor(PrimErrUnsupported);
		}
	}
	
	validMask = sqFileStdioHandlesInto(fileRecords);
	if (validMask == 0) {
		return primitiveFailFor(PrimErrUnsupported);
	}
	result = instantiateClassindexableSize(classArray(), 3);
	if (result == null) {
		return primitiveFailFor(PrimErrNoMemory);
	}
	pushRemappableOop(result);
	for (index = 0; index <= 2; index += 1) {
		if ((validMask & (1 << index)) != 0) {
			result = instantiateClassindexableSize(classByteArray(), sizeof(SQFile));
			if (result == null) {
				popRemappableOop();
				return primitiveFailFor(PrimErrNoMemory);
			}
			storePointerofObjectwithValue(index, topRemappableOop(), result);
			memcpy(firstIndexableField(result), (&(fileRecords[index])), sizeof(SQFile));
		}
	}
	
#  if COGMTVM
	fullGC();

#  endif /* COGMTVM */

	result = popRemappableOop();
	popthenPush(1, result);
}


/*	ftruncate is not an ansi function so we have a macro to point to a
	suitable platform implementation
 */

EXPORT(sqInt)
primitiveFileTruncate(void)
{
    SQFile *file;
    sqInt objectPointer;
    sqInt sz;
    squeakFileOffsetType truncatePosition;

	if (!(isIntegerObject(stackValue(0)))) {
		sz = sizeof(squeakFileOffsetType);
		if ((byteSizeOf(stackValue(0))) > sz) {
			return primitiveFail();
		}
	}
	truncatePosition = positive64BitValueOf(stackValue(0));
	/* begin fileValueOf: */
	objectPointer = stackValue(1);
	if (!((isBytes(objectPointer))
		 && ((byteSizeOf(objectPointer)) == (sizeof(SQFile))))) {
		primitiveFail();
		file = null;
		goto l1;
	}
	file = firstIndexableField(objectPointer);
l1:	/* end fileValueOf: */;
	if (!(failed())) {
		sqFileTruncate(file, truncatePosition);
	}
	if (!(failed())) {
		pop(2);
	}
}

EXPORT(sqInt)
primitiveFileWrite(void)
{
    sqInt array;
    sqInt bytesWritten;
    size_t count;
    size_t elementSize;
    SQFile *file;
    sqInt objectPointer;
    size_t startIndex;

	count = positive32BitValueOf(stackValue(0));
	startIndex = positive32BitValueOf(stackValue(1));
	array = stackValue(2);
	/* begin fileValueOf: */
	objectPointer = stackValue(3);
	if (!((isBytes(objectPointer))
		 && ((byteSizeOf(objectPointer)) == (sizeof(SQFile))))) {
		primitiveFail();
		file = null;
		goto l1;
	}
	file = firstIndexableField(objectPointer);
l1:	/* end fileValueOf: */;
	if ((failed())
	 || (!(isWordsOrBytes(array)))) {
		return primitiveFailFor(PrimErrBadArgument);
	}
	elementSize = (isWords(array)
		? 4
		: 1);
	if (!((startIndex >= 1)
		 && (((startIndex + count) - 1) <= (slotSizeOf(array))))) {
		return primitiveFailFor(PrimErrBadIndex);
	}
	bytesWritten = sqFileWriteFromAt(file, count * elementSize, ((char *) (firstIndexableField(array))), (startIndex - 1) * elementSize);
	if (!(failed())) {
		popthenPush(5, integerObjectOf(bytesWritten / elementSize));
	}
}

EXPORT(sqInt)
primitiveHasFileAccess(void)
{
    sqInt hasAccess;


	/* If the security plugin can be loaded, use it to check .
	   If not, assume it's ok */

	if (sHFAfn != 0) {
		hasAccess =  ((sqInt (*)(void))sHFAfn)();
	}
	else {
		hasAccess = 1;
	}
	pop(1);
	pushBool(hasAccess);
}


/*	Note: This is coded so that it can be run in Squeak. */

EXPORT(sqInt)
setInterpreter(struct VirtualMachine*anInterpreter)
{
    sqInt ok;

	interpreterProxy = anInterpreter;
	ok = ((interpreterProxy->majorVersion()) == (VM_PROXY_MAJOR))
	 && ((interpreterProxy->minorVersion()) >= (VM_PROXY_MINOR));
	if (ok) {
		
#if !defined(SQUEAK_BUILTIN_PLUGIN)
		booleanValueOf = interpreterProxy->booleanValueOf;
		byteSizeOf = interpreterProxy->byteSizeOf;
#if VM_PROXY_MAJOR > 1 || (VM_PROXY_MAJOR == 1 && VM_PROXY_MINOR >= 13)
		characterObjectOf = interpreterProxy->characterObjectOf;
#else
		characterObjectOf = 0;
#endif
		characterTable = interpreterProxy->characterTable;
		classArray = interpreterProxy->classArray;
		classByteArray = interpreterProxy->classByteArray;
		classString = interpreterProxy->classString;
		failed = interpreterProxy->failed;
		falseObject = interpreterProxy->falseObject;
		fetchPointerofObject = interpreterProxy->fetchPointerofObject;
		firstIndexableField = interpreterProxy->firstIndexableField;
		fullGC = interpreterProxy->fullGC;
		instantiateClassindexableSize = interpreterProxy->instantiateClassindexableSize;
		integerObjectOf = interpreterProxy->integerObjectOf;
		ioLoadFunctionFrom = interpreterProxy->ioLoadFunctionFrom;
		isBytes = interpreterProxy->isBytes;
		isIntegerObject = interpreterProxy->isIntegerObject;
		isWords = interpreterProxy->isWords;
		isWordsOrBytes = interpreterProxy->isWordsOrBytes;
		nilObject = interpreterProxy->nilObject;
		pop = interpreterProxy->pop;
		popthenPush = interpreterProxy->popthenPush;
		popRemappableOop = interpreterProxy->popRemappableOop;
		positive32BitIntegerFor = interpreterProxy->positive32BitIntegerFor;
		positive32BitValueOf = interpreterProxy->positive32BitValueOf;
		positive64BitIntegerFor = interpreterProxy->positive64BitIntegerFor;
		positive64BitValueOf = interpreterProxy->positive64BitValueOf;
		primitiveFail = interpreterProxy->primitiveFail;
		primitiveFailFor = interpreterProxy->primitiveFailFor;
		primitiveFailureCode = interpreterProxy->primitiveFailureCode;
		pushBool = interpreterProxy->pushBool;
		pushRemappableOop = interpreterProxy->pushRemappableOop;
		slotSizeOf = interpreterProxy->slotSizeOf;
		stackIntegerValue = interpreterProxy->stackIntegerValue;
		stackValue = interpreterProxy->stackValue;
		storePointerofObjectwithValue = interpreterProxy->storePointerofObjectwithValue;
		tenuringIncrementalGC = interpreterProxy->tenuringIncrementalGC;
		topRemappableOop = interpreterProxy->topRemappableOop;
		trueObject = interpreterProxy->trueObject;
#endif /* !defined(SQUEAK_BUILTIN_PLUGIN) */
	}
	return ok;
}


/*	Exported entry point for the VM. Needed for image saving only and no-op on
	anything but Macs.
 */

EXPORT(sqInt)
setMacFileTypeAndCreator(char *fileName, char *typeString, char *creatorString)
{
	return dir_SetMacFileTypeAndCreator(fileName, strlen(fileName), typeString, creatorString);
}

EXPORT(sqInt)
shutdownModule(void)
{
	return sqFileShutdown();
}


#ifdef SQUEAK_BUILTIN_PLUGIN

void* FilePlugin_exports[][3] = {
	{"FilePlugin", "fileOpenNamesizewritesecure", (void*)fileOpenNamesizewritesecure},
	{"FilePlugin", "getModuleName", (void*)getModuleName},
	{"FilePlugin", "getThisSession", (void*)getThisSession},
	{"FilePlugin", "initialiseModule", (void*)initialiseModule},
	{"FilePlugin", "moduleUnloaded", (void*)moduleUnloaded},
	{"FilePlugin", "primitiveDirectoryCreate", (void*)primitiveDirectoryCreate},
	{"FilePlugin", "primitiveDirectoryDelete", (void*)primitiveDirectoryDelete},
	{"FilePlugin", "primitiveDirectoryDelimitor", (void*)primitiveDirectoryDelimitor},
	{"FilePlugin", "primitiveDirectoryEntry", (void*)primitiveDirectoryEntry},
	{"FilePlugin", "primitiveDirectoryGetMacTypeAndCreator", (void*)primitiveDirectoryGetMacTypeAndCreator},
	{"FilePlugin", "primitiveDirectoryLookup", (void*)primitiveDirectoryLookup},
	{"FilePlugin", "primitiveDirectorySetMacTypeAndCreator", (void*)primitiveDirectorySetMacTypeAndCreator},
	{"FilePlugin", "primitiveDisableFileAccess", (void*)primitiveDisableFileAccess},
	{"FilePlugin", "primitiveFileAtEnd", (void*)primitiveFileAtEnd},
	{"FilePlugin", "primitiveFileClose", (void*)primitiveFileClose},
	{"FilePlugin", "primitiveFileDelete", (void*)primitiveFileDelete},
	{"FilePlugin", "primitiveFileFlush", (void*)primitiveFileFlush},
	{"FilePlugin", "primitiveFileGetPosition", (void*)primitiveFileGetPosition},
	{"FilePlugin", "primitiveFileOpen", (void*)primitiveFileOpen},
	{"FilePlugin", "primitiveFileRead", (void*)primitiveFileRead},
	{"FilePlugin", "primitiveFileRename", (void*)primitiveFileRename},
	{"FilePlugin", "primitiveFileSetPosition", (void*)primitiveFileSetPosition},
	{"FilePlugin", "primitiveFileSize", (void*)primitiveFileSize},
	{"FilePlugin", "primitiveFileStdioHandles", (void*)primitiveFileStdioHandles},
	{"FilePlugin", "primitiveFileTruncate", (void*)primitiveFileTruncate},
	{"FilePlugin", "primitiveFileWrite", (void*)primitiveFileWrite},
	{"FilePlugin", "primitiveHasFileAccess", (void*)primitiveHasFileAccess},
	{"FilePlugin", "setInterpreter", (void*)setInterpreter},
	{"FilePlugin", "setMacFileTypeAndCreator", (void*)setMacFileTypeAndCreator},
	{"FilePlugin", "shutdownModule", (void*)shutdownModule},
	{NULL, NULL, NULL}
};

#endif /* ifdef SQ_BUILTIN_PLUGIN */
