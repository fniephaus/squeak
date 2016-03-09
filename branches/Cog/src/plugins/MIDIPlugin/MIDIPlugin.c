/* Automatically generated by
	SmartSyntaxPluginCodeGenerator VMMaker.oscog-eem.1716 uuid: 9115c93b-f425-4118-96e7-7130adeff1f2
   from
	MIDIPlugin VMMaker.oscog-eem.1716 uuid: 9115c93b-f425-4118-96e7-7130adeff1f2
 */
static char __buildInfo[] = "MIDIPlugin VMMaker.oscog-eem.1716 uuid: 9115c93b-f425-4118-96e7-7130adeff1f2 " __DATE__ ;



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Default EXPORT macro that does nothing (see comment in sq.h): */
#define EXPORT(returnType) returnType

/* Do not include the entire sq.h file but just those parts needed. */
#include "sqConfig.h"			/* Configuration options */
#include "sqVirtualMachine.h"	/*  The virtual machine proxy definition */
#include "sqPlatformSpecific.h"	/* Platform specific definitions */

#define true 1
#define false 0
#define null 0  /* using 'null' because nil is predefined in Think C */
#ifdef SQUEAK_BUILTIN_PLUGIN
# undef EXPORT
# define EXPORT(returnType) static returnType
#endif

#include "MIDIPlugin.h"
#include "sqMemoryAccess.h"


/*** Function Prototypes ***/
EXPORT(const char*) getModuleName(void);
EXPORT(sqInt) initialiseModule(void);
EXPORT(sqInt) primitiveMIDIClosePort(void);
EXPORT(sqInt) primitiveMIDIGetClock(void);
EXPORT(sqInt) primitiveMIDIGetPortCount(void);
EXPORT(sqInt) primitiveMIDIGetPortDirectionality(void);
EXPORT(sqInt) primitiveMIDIGetPortName(void);
EXPORT(sqInt) primitiveMIDIOpenPort(void);
EXPORT(sqInt) primitiveMIDIParameterGet(void);
EXPORT(sqInt) primitiveMIDIParameterGetOrSet(void);
EXPORT(sqInt) primitiveMIDIParameterSet(void);
EXPORT(sqInt) primitiveMIDIRead(void);
EXPORT(sqInt) primitiveMIDIWrite(void);
EXPORT(sqInt) setInterpreter(struct VirtualMachine*anInterpreter);
EXPORT(sqInt) shutdownModule(void);
static sqInt sqAssert(sqInt aBool);


/*** Variables ***/

#if !defined(SQUEAK_BUILTIN_PLUGIN)
static sqInt (*byteSizeOf)(sqInt oop);
static sqInt (*classString)(void);
static sqInt (*failed)(void);
static void * (*firstIndexableField)(sqInt oop);
static sqInt (*instantiateClassindexableSize)(sqInt classPointer, sqInt size);
static sqInt (*integerObjectOf)(sqInt value);
static sqInt (*isBytes)(sqInt oop);
static sqInt (*methodArgumentCount)(void);
static sqInt (*pop)(sqInt nItems);
static sqInt (*popthenPush)(sqInt nItems, sqInt oop);
static sqInt (*stackIntegerValue)(sqInt offset);
static sqInt (*stackValue)(sqInt offset);
static sqInt (*success)(sqInt aBoolean);
#else /* !defined(SQUEAK_BUILTIN_PLUGIN) */
extern sqInt byteSizeOf(sqInt oop);
extern sqInt classString(void);
extern sqInt failed(void);
extern void * firstIndexableField(sqInt oop);
extern sqInt instantiateClassindexableSize(sqInt classPointer, sqInt size);
extern sqInt integerObjectOf(sqInt value);
extern sqInt isBytes(sqInt oop);
extern sqInt methodArgumentCount(void);
extern sqInt pop(sqInt nItems);
extern sqInt popthenPush(sqInt nItems, sqInt oop);
extern sqInt stackIntegerValue(sqInt offset);
extern sqInt stackValue(sqInt offset);
extern sqInt success(sqInt aBoolean);
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"MIDIPlugin VMMaker.oscog-eem.1716 (i)"
#else
	"MIDIPlugin VMMaker.oscog-eem.1716 (e)"
#endif
;



/*	Note: This is hardcoded so it can be run from Squeak.
	The module name is used for validating a module *after*
	it is loaded to check if it does really contain the module
	we're thinking it contains. This is important! */

	/* InterpreterPlugin>>#getModuleName */
EXPORT(const char*)
getModuleName(void)
{
	return moduleName;
}

	/* MIDIPlugin>>#initialiseModule */
EXPORT(sqInt)
initialiseModule(void)
{
	return midiInit();
}

	/* MIDIPlugin>>#primitiveMIDIClosePort: */
EXPORT(sqInt)
primitiveMIDIClosePort(void)
{
	sqInt portNum;

	portNum = stackIntegerValue(0);
	if (failed()) {
		return null;
	}
	sqMIDIClosePort(portNum);
	if (failed()) {
		return null;
	}
	pop(1);
	return null;
}


/*	Return the value of the MIDI clock as a SmallInteger. The range is limited
	to SmallInteger maxVal / 2 to allow scheduling MIDI events into the future
	without overflowing a SmallInteger. The sqMIDIGetClock function is assumed
	to wrap at or before 16r20000000.
 */

	/* MIDIPlugin>>#primitiveMIDIGetClock */
EXPORT(sqInt)
primitiveMIDIGetClock(void)
{
	sqInt clockValue;
	sqInt _return_value;

	clockValue = (sqMIDIGetClock()) & 0x1FFFFFFF;
	if (failed()) {
		return null;
	}
	_return_value = integerObjectOf(clockValue);
	if (failed()) {
		return null;
	}
	popthenPush(1, _return_value);
	return null;
}

	/* MIDIPlugin>>#primitiveMIDIGetPortCount */
EXPORT(sqInt)
primitiveMIDIGetPortCount(void)
{
	sqInt n;
	sqInt _return_value;

	n = sqMIDIGetPortCount();
	if (failed()) {
		return null;
	}
	_return_value = integerObjectOf(n);
	if (failed()) {
		return null;
	}
	popthenPush(1, _return_value);
	return null;
}

	/* MIDIPlugin>>#primitiveMIDIGetPortDirectionality: */
EXPORT(sqInt)
primitiveMIDIGetPortDirectionality(void)
{
	sqInt dir;
	sqInt portNum;
	sqInt _return_value;

	portNum = stackIntegerValue(0);
	if (failed()) {
		return null;
	}
	dir = sqMIDIGetPortDirectionality(portNum);
	if (failed()) {
		return null;
	}
	_return_value = integerObjectOf(dir);
	if (failed()) {
		return null;
	}
	popthenPush(2, _return_value);
	return null;
}

	/* MIDIPlugin>>#primitiveMIDIGetPortName: */
EXPORT(sqInt)
primitiveMIDIGetPortName(void)
{
	sqInt nameObj;
	char portName[256];
	sqInt portNum;
	sqInt sz;

	portNum = stackIntegerValue(0);
	if (failed()) {
		return null;
	}
	sz = sqMIDIGetPortName(portNum, portName, 0xFF);
	nameObj = instantiateClassindexableSize(classString(), sz);
	if (failed()) {
		return null;
	}
	memcpy(((char *)firstIndexableField(nameObj)), portName, sz);
	if (failed()) {
		return null;
	}
	popthenPush(2, nameObj);
	return null;
}

	/* MIDIPlugin>>#primitiveMIDIOpenPort:sema:speed: */
EXPORT(sqInt)
primitiveMIDIOpenPort(void)
{
	sqInt clockRate;
	sqInt portNum;
	sqInt semaIndex;

	portNum = stackIntegerValue(2);
	semaIndex = stackIntegerValue(1);
	clockRate = stackIntegerValue(0);
	if (failed()) {
		return null;
	}
	sqMIDIOpenPort(portNum, semaIndex, clockRate);
	if (failed()) {
		return null;
	}
	pop(3);
	return null;
}


/*	read parameter */

	/* MIDIPlugin>>#primitiveMIDIParameterGet: */
EXPORT(sqInt)
primitiveMIDIParameterGet(void)
{
	sqInt currentValue;
	sqInt whichParameter;
	sqInt _return_value;

	whichParameter = stackIntegerValue(0);
	if (failed()) {
		return null;
	}
	currentValue = sqMIDIParameterGet(whichParameter);
	if (failed()) {
		return null;
	}
	_return_value = integerObjectOf(currentValue);
	if (failed()) {
		return null;
	}
	popthenPush(2, _return_value);
	return null;
}


/*	Backward compatibility */

	/* MIDIPlugin>>#primitiveMIDIParameterGetOrSet */
EXPORT(sqInt)
primitiveMIDIParameterGetOrSet(void)
{
	if ((methodArgumentCount()) == 1) {
		return primitiveMIDIParameterGet();
	}
	else {
		return primitiveMIDIParameterSet();
	}
}


/*	write parameter */

	/* MIDIPlugin>>#primitiveMIDIParameterSet:value: */
EXPORT(sqInt)
primitiveMIDIParameterSet(void)
{
	sqInt newValue;
	sqInt whichParameter;

	whichParameter = stackIntegerValue(1);
	newValue = stackIntegerValue(0);
	if (failed()) {
		return null;
	}
	sqMIDIParameterSet(whichParameter, newValue);
	if (failed()) {
		return null;
	}
	pop(2);
	return null;
}

	/* MIDIPlugin>>#primitiveMIDIRead:into: */
EXPORT(sqInt)
primitiveMIDIRead(void)
{
	char *array;
	sqInt arrayLength;
	sqInt bytesRead;
	sqInt portNum;
	sqInt _return_value;

	portNum = stackIntegerValue(1);
	success(isBytes(stackValue(0)));
	array = ((char *) (firstIndexableField(stackValue(0))));
	if (failed()) {
		return null;
	}
	arrayLength = byteSizeOf(((sqInt)(long)(array) - BaseHeaderSize));
	bytesRead = sqMIDIPortReadInto(portNum, arrayLength, ((sqInt)array));
	if (failed()) {
		return null;
	}
	_return_value = integerObjectOf(bytesRead);
	if (failed()) {
		return null;
	}
	popthenPush(3, _return_value);
	return null;
}

	/* MIDIPlugin>>#primitiveMIDIWrite:from:at: */
EXPORT(sqInt)
primitiveMIDIWrite(void)
{
	char *array;
	sqInt arrayLength;
	sqInt bytesWritten;
	sqInt portNum;
	sqInt time;
	sqInt _return_value;

	portNum = stackIntegerValue(2);
	success(isBytes(stackValue(1)));
	array = ((char *) (firstIndexableField(stackValue(1))));
	time = stackIntegerValue(0);
	if (failed()) {
		return null;
	}
	arrayLength = byteSizeOf(((sqInt)(long)(array) - BaseHeaderSize));
	bytesWritten = sqMIDIPortWriteFromAt(portNum, arrayLength, ((sqInt)array), time);
	if (failed()) {
		return null;
	}
	_return_value = integerObjectOf(bytesWritten);
	if (failed()) {
		return null;
	}
	popthenPush(4, _return_value);
	return null;
}


/*	Note: This is coded so that it can be run in Squeak. */

	/* InterpreterPlugin>>#setInterpreter: */
EXPORT(sqInt)
setInterpreter(struct VirtualMachine*anInterpreter)
{
	sqInt ok;

	interpreterProxy = anInterpreter;
	ok = ((interpreterProxy->majorVersion()) == (VM_PROXY_MAJOR))
	 && ((interpreterProxy->minorVersion()) >= (VM_PROXY_MINOR));
	if (ok) {
		
#if !defined(SQUEAK_BUILTIN_PLUGIN)
		byteSizeOf = interpreterProxy->byteSizeOf;
		classString = interpreterProxy->classString;
		failed = interpreterProxy->failed;
		firstIndexableField = interpreterProxy->firstIndexableField;
		instantiateClassindexableSize = interpreterProxy->instantiateClassindexableSize;
		integerObjectOf = interpreterProxy->integerObjectOf;
		isBytes = interpreterProxy->isBytes;
		methodArgumentCount = interpreterProxy->methodArgumentCount;
		pop = interpreterProxy->pop;
		popthenPush = interpreterProxy->popthenPush;
		stackIntegerValue = interpreterProxy->stackIntegerValue;
		stackValue = interpreterProxy->stackValue;
		success = interpreterProxy->success;
#endif /* !defined(SQUEAK_BUILTIN_PLUGIN) */
	}
	return ok;
}

	/* MIDIPlugin>>#shutdownModule */
EXPORT(sqInt)
shutdownModule(void)
{
	return midiShutdown();
}

	/* SmartSyntaxInterpreterPlugin>>#sqAssert: */
static sqInt
sqAssert(sqInt aBool)
{
	/* missing DebugCode */;
	return aBool;
}


#ifdef SQUEAK_BUILTIN_PLUGIN

static char _m[] = "MIDIPlugin";
void* MIDIPlugin_exports[][3] = {
	{(void*)_m, "getModuleName", (void*)getModuleName},
	{(void*)_m, "initialiseModule", (void*)initialiseModule},
	{(void*)_m, "primitiveMIDIClosePort\000\000", (void*)primitiveMIDIClosePort},
	{(void*)_m, "primitiveMIDIGetClock\000\377", (void*)primitiveMIDIGetClock},
	{(void*)_m, "primitiveMIDIGetPortCount\000\377", (void*)primitiveMIDIGetPortCount},
	{(void*)_m, "primitiveMIDIGetPortDirectionality\000\000", (void*)primitiveMIDIGetPortDirectionality},
	{(void*)_m, "primitiveMIDIGetPortName\000\000", (void*)primitiveMIDIGetPortName},
	{(void*)_m, "primitiveMIDIOpenPort\000\000", (void*)primitiveMIDIOpenPort},
	{(void*)_m, "primitiveMIDIParameterGet\000\000", (void*)primitiveMIDIParameterGet},
	{(void*)_m, "primitiveMIDIParameterGetOrSet\000\000", (void*)primitiveMIDIParameterGetOrSet},
	{(void*)_m, "primitiveMIDIParameterSet\000\000", (void*)primitiveMIDIParameterSet},
	{(void*)_m, "primitiveMIDIRead\000\000", (void*)primitiveMIDIRead},
	{(void*)_m, "primitiveMIDIWrite\000\000", (void*)primitiveMIDIWrite},
	{(void*)_m, "setInterpreter", (void*)setInterpreter},
	{(void*)_m, "shutdownModule\000\377", (void*)shutdownModule},
	{NULL, NULL, NULL}
};

#else /* ifdef SQ_BUILTIN_PLUGIN */

signed char primitiveMIDIClosePortAccessorDepth = 0;
signed char primitiveMIDIGetPortDirectionalityAccessorDepth = 0;
signed char primitiveMIDIGetPortNameAccessorDepth = 0;
signed char primitiveMIDIOpenPortAccessorDepth = 0;
signed char primitiveMIDIParameterGetAccessorDepth = 0;
signed char primitiveMIDIParameterGetOrSetAccessorDepth = 0;
signed char primitiveMIDIParameterSetAccessorDepth = 0;
signed char primitiveMIDIReadAccessorDepth = 0;
signed char primitiveMIDIWriteAccessorDepth = 0;

#endif /* ifdef SQ_BUILTIN_PLUGIN */
