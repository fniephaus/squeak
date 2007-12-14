/* Automatically generated from Squeak on #(13 December 2007 10:06:59 pm) */

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
#include "OggPlugin.h"

/* memory access macros */
#define byteAt(i) (*((unsigned char *) (i)))
#define byteAtput(i, val) (*((unsigned char *) (i)) = val)
#define longAt(i) (*((int *) (i)))
#define longAtput(i, val) (*((int *) (i)) = val)


/*** Constants ***/

/*** Function Prototypes ***/
#pragma export on
EXPORT(const char*) getModuleName(void);
#pragma export off
static int halt(void);
static int msg(char *s);
static SqOggPtr * oggInstanceOf(int oggOop);
#pragma export on
EXPORT(int) primitiveClose(void);
EXPORT(int) primitiveExtractMono(void);
EXPORT(int) primitiveGetChannels(void);
EXPORT(int) primitiveGetComment(void);
EXPORT(int) primitiveGetCommentSize(void);
EXPORT(int) primitiveGetRate(void);
EXPORT(int) primitiveGetState(void);
EXPORT(int) primitiveGetVendor(void);
EXPORT(int) primitiveOpen(void);
EXPORT(int) primitivePacketFlush(void);
EXPORT(int) primitiveRead(void);
EXPORT(int) primitiveReadSize(void);
EXPORT(int) primitiveSetChannels(void);
EXPORT(int) primitiveSetQuality(void);
EXPORT(int) primitiveSetRate(void);
EXPORT(int) primitiveVersion(void);
EXPORT(int) primitiveWrite(void);
EXPORT(int) primitiveWriteEOS(void);
EXPORT(int) setInterpreter(struct VirtualMachine* anInterpreter);
#pragma export off
static int sqAssert(int aBool);
/*** Variables ***/

#ifdef SQUEAK_BUILTIN_PLUGIN
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"OggPlugin 13 December 2007 (i)"
#else
	"OggPlugin 13 December 2007 (e)"
#endif
;



/*	Note: This is hardcoded so it can be run from Squeak.
	The module name is used for validating a module *after*
	it is loaded to check if it does really contain the module
	we're thinking it contains. This is important! */

EXPORT(const char*) getModuleName(void) {
	return moduleName;
}

static int halt(void) {
	;
}

static int msg(char *s) {
	fprintf(stderr, "\n%s: %s", moduleName, s);
}


/*	Convert from Squeak pointer to SqOggPtr */

static SqOggPtr * oggInstanceOf(int oggOop) {
	SqOggPtr * oggp;

	interpreterProxy->success((interpreterProxy->isBytes(oggOop)) && ((interpreterProxy->byteSizeOf(oggOop)) == (sizeof(SqOggPtr))));
	oggp = ((SqOggPtr *) (interpreterProxy->firstIndexableField(oggOop)));
	return oggp;
}

EXPORT(int) primitiveClose(void) {
	char *oggPtr;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	if (interpreterProxy->failed()) {
		return null;
	}
	SqOggClose((SqOggPtr *) oggPtr);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(1);
	return null;
}

EXPORT(int) primitiveExtractMono(void) {
	int channel;
	short * dest;
	int size;
	int i;
	short * src;
	int srcOop;
	int destOop;

	if (!((interpreterProxy->methodArgumentCount()) == 4)) {
		return interpreterProxy->primitiveFail();
	}

	/* output buffer */

	destOop = interpreterProxy->stackObjectValue(3);

	/* source buffer */

	srcOop = interpreterProxy->stackObjectValue(2);

	/* frame size (word size of dest) */

	size = interpreterProxy->stackIntegerValue(1);

	/* channel number */

	channel = interpreterProxy->stackIntegerValue(0);
	dest = ((short *) (interpreterProxy->firstIndexableField(destOop)));
	src = ((short *) (interpreterProxy->firstIndexableField(srcOop)));
	for (i = 0; i <= (size - 1); i += 1) {
		dest[i] = (src[(i * 2) + channel]);
	}
	return interpreterProxy->pop(4);
}

EXPORT(int) primitiveGetChannels(void) {
	char *oggPtr;
	int _return_value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	if (interpreterProxy->failed()) {
		return null;
	}
	_return_value = interpreterProxy->integerObjectOf((SqOggGetChannels((SqOggPtr *) oggPtr)));
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(2, _return_value);
	return null;
}

EXPORT(int) primitiveGetComment(void) {
	char *oggPtr;
	char *buffer;
	int size;
	int _return_value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(2)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(2))));
	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(1)));
	buffer = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(1))));
	size = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	_return_value = interpreterProxy->integerObjectOf((SqOggGetComment((SqOggPtr *) oggPtr, buffer, size)));
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(4, _return_value);
	return null;
}

EXPORT(int) primitiveGetCommentSize(void) {
	char *oggPtr;
	int _return_value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	if (interpreterProxy->failed()) {
		return null;
	}
	_return_value = interpreterProxy->integerObjectOf((SqOggGetCommentSize((SqOggPtr *) oggPtr)));
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(2, _return_value);
	return null;
}

EXPORT(int) primitiveGetRate(void) {
	char *oggPtr;
	int _return_value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	if (interpreterProxy->failed()) {
		return null;
	}
	_return_value = interpreterProxy->integerObjectOf((SqOggGetRate((SqOggPtr *) oggPtr)));
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(2, _return_value);
	return null;
}

EXPORT(int) primitiveGetState(void) {
	char *oggPtr;
	int _return_value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	if (interpreterProxy->failed()) {
		return null;
	}
	_return_value = interpreterProxy->integerObjectOf((SqOggGetState((SqOggPtr *) oggPtr)));
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(2, _return_value);
	return null;
}

EXPORT(int) primitiveGetVendor(void) {
	char *oggPtr;
	char *buffer;
	int size;
	int _return_value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(2)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(2))));
	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(1)));
	buffer = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(1))));
	size = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	_return_value = interpreterProxy->integerObjectOf((SqOggGetVendor((SqOggPtr *) oggPtr, buffer, size)));
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(4, _return_value);
	return null;
}

EXPORT(int) primitiveOpen(void) {
	SqOggPtr * oggp;
	int oggOop;
	int mode;

	mode = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	oggOop = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classByteArray(), sizeof(SqOggPtr));
	oggp = ((SqOggPtr *) (interpreterProxy->firstIndexableField(oggOop)));
	SqOggOpen(mode, oggp);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(2, oggOop);
	return null;
}

EXPORT(int) primitivePacketFlush(void) {
	char *oggPtr;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	if (interpreterProxy->failed()) {
		return null;
	}
	SqOggPacketFlush((SqOggPtr *) oggPtr);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(1);
	return null;
}

EXPORT(int) primitiveRead(void) {
	SqOggPtr * oggp;
	int bufferOop;
	int size;
	char * buffer;
	int result;
	int oggOop;

	if (!((interpreterProxy->methodArgumentCount()) == 3)) {
		return interpreterProxy->primitiveFail();
	}
	size = interpreterProxy->stackIntegerValue(0);
	bufferOop = interpreterProxy->stackObjectValue(1);
	buffer = ((char *) (interpreterProxy->firstIndexableField(bufferOop)));
	oggOop = interpreterProxy->stackObjectValue(2);
	oggp = oggInstanceOf(oggOop);
	interpreterProxy->success(interpreterProxy->isWordsOrBytes(bufferOop));
	if (interpreterProxy->failed()) {
		return null;
	}
	result = SqOggRead(oggp, buffer, size);
	interpreterProxy->pop(4);
	return interpreterProxy->pushInteger(result);
}

EXPORT(int) primitiveReadSize(void) {
	char *oggPtr;
	int _return_value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	if (interpreterProxy->failed()) {
		return null;
	}
	_return_value = interpreterProxy->integerObjectOf((SqOggReadSize((SqOggPtr *) oggPtr)));
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(2, _return_value);
	return null;
}

EXPORT(int) primitiveSetChannels(void) {
	char *oggPtr;
	int value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(1)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(1))));
	value = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	SqOggSetChannels((SqOggPtr *) oggPtr, value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(2);
	return null;
}

EXPORT(int) primitiveSetQuality(void) {
	char *oggPtr;
	double value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(1)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(1))));
	value = interpreterProxy->stackFloatValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	SqOggSetQuality((SqOggPtr *) oggPtr, value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(2);
	return null;
}

EXPORT(int) primitiveSetRate(void) {
	char *oggPtr;
	int value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(1)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(1))));
	value = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	SqOggSetRate((SqOggPtr *) oggPtr, value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(2);
	return null;
}

EXPORT(int) primitiveVersion(void) {
	int _return_value;

	_return_value = interpreterProxy->integerObjectOf(1);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	Write to the buffer. It is written traditional InterpreterPlugin way
	because the second buffer can be words or bytes array */

EXPORT(int) primitiveWrite(void) {
	int size;
	int chunkSize;
	int result;
	int bufSize;
	SqOggPtr * oggp;
	int i;
	char * buffer;
	int bufferOop;
	int oggOop;

	bufSize = 4096;
	if (!((interpreterProxy->methodArgumentCount()) == 3)) {
		return interpreterProxy->primitiveFail();
	}
	size = interpreterProxy->stackIntegerValue(0);
	bufferOop = interpreterProxy->stackObjectValue(1);
	buffer = ((char *) (interpreterProxy->firstIndexableField(bufferOop)));
	oggOop = interpreterProxy->stackObjectValue(2);
	oggp = oggInstanceOf(oggOop);
	interpreterProxy->success(interpreterProxy->isWordsOrBytes(bufferOop));
	if (interpreterProxy->failed()) {
		return null;
	}
	for (i = 0; i <= (size - 1); i += bufSize) {
		if ((i + bufSize) < size) {
			chunkSize = bufSize;
		} else {
			chunkSize = size - i;
		}
		result = SqOggWrite(oggp, buffer + i, chunkSize);
	}
	interpreterProxy->pop(4);
	return interpreterProxy->pushInteger(result);
}

EXPORT(int) primitiveWriteEOS(void) {
	char *oggPtr;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	oggPtr = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	if (interpreterProxy->failed()) {
		return null;
	}
	SqOggWriteEOS((SqOggPtr *) oggPtr);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(1);
	return null;
}


/*	Note: This is coded so that is can be run from Squeak. */

EXPORT(int) setInterpreter(struct VirtualMachine* anInterpreter) {
	int ok;

	interpreterProxy = anInterpreter;
	ok = interpreterProxy->majorVersion() == VM_PROXY_MAJOR;
	if (ok == 0) {
		return 0;
	}
	ok = interpreterProxy->minorVersion() >= VM_PROXY_MINOR;
	return ok;
}

static int sqAssert(int aBool) {
	/* missing DebugCode */;
}


#ifdef SQUEAK_BUILTIN_PLUGIN


void* OggPlugin_exports[][3] = {
	{"OggPlugin", "primitiveVersion", (void*)primitiveVersion},
	{"OggPlugin", "primitiveExtractMono", (void*)primitiveExtractMono},
	{"OggPlugin", "primitiveClose", (void*)primitiveClose},
	{"OggPlugin", "primitiveWriteEOS", (void*)primitiveWriteEOS},
	{"OggPlugin", "primitiveReadSize", (void*)primitiveReadSize},
	{"OggPlugin", "primitiveGetRate", (void*)primitiveGetRate},
	{"OggPlugin", "primitiveRead", (void*)primitiveRead},
	{"OggPlugin", "primitiveGetComment", (void*)primitiveGetComment},
	{"OggPlugin", "primitiveSetChannels", (void*)primitiveSetChannels},
	{"OggPlugin", "getModuleName", (void*)getModuleName},
	{"OggPlugin", "primitiveGetChannels", (void*)primitiveGetChannels},
	{"OggPlugin", "setInterpreter", (void*)setInterpreter},
	{"OggPlugin", "primitivePacketFlush", (void*)primitivePacketFlush},
	{"OggPlugin", "primitiveWrite", (void*)primitiveWrite},
	{"OggPlugin", "primitiveGetVendor", (void*)primitiveGetVendor},
	{"OggPlugin", "primitiveGetState", (void*)primitiveGetState},
	{"OggPlugin", "primitiveSetQuality", (void*)primitiveSetQuality},
	{"OggPlugin", "primitiveOpen", (void*)primitiveOpen},
	{"OggPlugin", "primitiveSetRate", (void*)primitiveSetRate},
	{"OggPlugin", "primitiveGetCommentSize", (void*)primitiveGetCommentSize},
	{NULL, NULL, NULL}
};


#endif /* ifdef SQ_BUILTIN_PLUGIN */

