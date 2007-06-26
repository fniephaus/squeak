/* Automatically generated from Squeak on an Array(21 June 2007 12:11:17 pm) */

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
#include "DropPlugin.h"

#include "sqMemoryAccess.h"


/*** Constants ***/

/*** Function Prototypes ***/
static VirtualMachine * getInterpreter(void);
#pragma export on
EXPORT(const char*) getModuleName(void);
#pragma export off
static sqInt halt(void);
#pragma export on
EXPORT(sqInt) initialiseModule(void);
#pragma export off
static sqInt msg(char * s);
#pragma export on
EXPORT(sqInt) primitiveDndOutStart(void);
EXPORT(sqInt) primitiveDropRequestFileHandle(void);
EXPORT(sqInt) primitiveDropRequestFileName(void);
EXPORT(sqInt) setFileAccessCallback(int address);
EXPORT(sqInt) setInterpreter(struct VirtualMachine* anInterpreter);
EXPORT(sqInt) shutdownModule(void);
#pragma export off
static sqInt sqAssert(sqInt aBool);
/*** Variables ***/

#ifdef SQUEAK_BUILTIN_PLUGIN
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"DropPlugin 21 June 2007 (i)"
#else
	"DropPlugin 21 June 2007 (e)"
#endif
;



/*	Note: This is coded so that plugins can be run from Squeak. */

static VirtualMachine * getInterpreter(void) {
	return interpreterProxy;
}


/*	Note: This is hardcoded so it can be run from Squeak.
	The module name is used for validating a module *after*
	it is loaded to check if it does really contain the module
	we're thinking it contains. This is important! */

EXPORT(const char*) getModuleName(void) {
	return moduleName;
}

static sqInt halt(void) {
	;
}

EXPORT(sqInt) initialiseModule(void) {
	return dropInit();
}

static sqInt msg(char * s) {
	fprintf(stderr, "\n%s: %s", moduleName, s);
}

EXPORT(sqInt) primitiveDndOutStart(void) {
	sqInt dataLength;
	sqInt formatLength;
	char *aByteArray;
	char *aFormat;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(1)));
	aByteArray = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(1))));
	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	aFormat = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	if (interpreterProxy->failed()) {
		return null;
	}
	dataLength = interpreterProxy->slotSizeOf(((sqInt)(long)(aByteArray) - 4));
	formatLength = interpreterProxy->slotSizeOf(((sqInt)(long)(aFormat) - 4));
	sqDndOutStart(aByteArray, dataLength, aFormat, formatLength);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(2);
	return null;
}


/*	Note: File handle creation needs to be handled by specific support code explicitly bypassing the plugin file sand box. */

EXPORT(sqInt) primitiveDropRequestFileHandle(void) {
	sqInt handleOop;
	sqInt dropIndex;

	if (!((interpreterProxy->methodArgumentCount()) == 1)) {
		return interpreterProxy->primitiveFail();
	}
	dropIndex = interpreterProxy->stackIntegerValue(0);

	/* dropRequestFileHandle needs to return the actual oop returned */

	handleOop = dropRequestFileHandle(dropIndex);
	if (!(interpreterProxy->failed())) {
		interpreterProxy->pop(2);
		interpreterProxy->push(handleOop);
	}
}


/*	Note: File handle creation needs to be handled by specific support code explicitly bypassing the plugin file sand box. */

EXPORT(sqInt) primitiveDropRequestFileName(void) {
	sqInt nameOop;
	sqInt i;
	char * dropName;
	sqInt nameLength;
	sqInt dropIndex;
	char * namePtr;

	if (!((interpreterProxy->methodArgumentCount()) == 1)) {
		return interpreterProxy->primitiveFail();
	}
	dropIndex = interpreterProxy->stackIntegerValue(0);

	/* dropRequestFileName returns name or NULL on error */

	dropName = dropRequestFileName(dropIndex);
	if (dropName == null) {
		return interpreterProxy->primitiveFail();
	}
	nameLength = strlen(dropName);
	nameOop = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classString(), nameLength);
	namePtr = interpreterProxy->firstIndexableField(nameOop);
	for (i = 0; i <= (nameLength - 1); i += 1) {
		namePtr[i] = (dropName[i]);
	}
	interpreterProxy->pop(2);
	interpreterProxy->push(nameOop);
}

EXPORT(sqInt) setFileAccessCallback(int address) {
	return sqSecFileAccessCallback((void *) address);
}


/*	Note: This is coded so that is can be run from Squeak. */

EXPORT(sqInt) setInterpreter(struct VirtualMachine* anInterpreter) {
	sqInt ok;

	interpreterProxy = anInterpreter;
	ok = interpreterProxy->majorVersion() == VM_PROXY_MAJOR;
	if (ok == 0) {
		return 0;
	}
	ok = interpreterProxy->minorVersion() >= VM_PROXY_MINOR;
	return ok;
}

EXPORT(sqInt) shutdownModule(void) {
	return dropShutdown();
}

static sqInt sqAssert(sqInt aBool) {
	/* missing DebugCode */;
}


#ifdef SQUEAK_BUILTIN_PLUGIN


void* DropPlugin_exports[][3] = {
	{"DropPlugin", "primitiveDropRequestFileName", (void*)primitiveDropRequestFileName},
	{"DropPlugin", "shutdownModule", (void*)shutdownModule},
	{"DropPlugin", "getModuleName", (void*)getModuleName},
	{"DropPlugin", "primitiveDndOutStart", (void*)primitiveDndOutStart},
	{"DropPlugin", "setInterpreter", (void*)setInterpreter},
	{"DropPlugin", "setFileAccessCallback", (void*)setFileAccessCallback},
	{"DropPlugin", "initialiseModule", (void*)initialiseModule},
	{"DropPlugin", "primitiveDropRequestFileHandle", (void*)primitiveDropRequestFileHandle},
	{NULL, NULL, NULL}
};


#endif /* ifdef SQ_BUILTIN_PLUGIN */

