/* Automatically generated by
	SmartSyntaxPluginCodeGenerator VMMaker.oscog-eem.1716 uuid: 9115c93b-f425-4118-96e7-7130adeff1f2
   from
	JoystickTabletPlugin VMMaker.oscog-eem.1716 uuid: 9115c93b-f425-4118-96e7-7130adeff1f2
 */
static char __buildInfo[] = "JoystickTabletPlugin VMMaker.oscog-eem.1716 uuid: 9115c93b-f425-4118-96e7-7130adeff1f2 " __DATE__ ;



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

#include "JoystickTabletPlugin.h"
#include "sqMemoryAccess.h"


/*** Function Prototypes ***/
EXPORT(const char*) getModuleName(void);
EXPORT(sqInt) initialiseModule(void);
EXPORT(sqInt) primitiveGetTabletParameters(void);
EXPORT(sqInt) primitiveReadJoystick(void);
EXPORT(sqInt) primitiveReadTablet(void);
EXPORT(sqInt) setInterpreter(struct VirtualMachine*anInterpreter);
EXPORT(sqInt) shutdownModule(void);
static sqInt sqAssert(sqInt aBool);


/*** Variables ***/

#if !defined(SQUEAK_BUILTIN_PLUGIN)
static sqInt (*classBitmap)(void);
static sqInt (*failed)(void);
static void * (*firstIndexableField)(sqInt oop);
static sqInt (*instantiateClassindexableSize)(sqInt classPointer, sqInt size);
static sqInt (*popthenPush)(sqInt nItems, sqInt oop);
static sqInt (*positive32BitIntegerFor)(sqInt integerValue);
static sqInt (*stackIntegerValue)(sqInt offset);
static sqInt (*success)(sqInt aBoolean);
#else /* !defined(SQUEAK_BUILTIN_PLUGIN) */
extern sqInt classBitmap(void);
extern sqInt failed(void);
extern void * firstIndexableField(sqInt oop);
extern sqInt instantiateClassindexableSize(sqInt classPointer, sqInt size);
extern sqInt popthenPush(sqInt nItems, sqInt oop);
extern sqInt positive32BitIntegerFor(sqInt integerValue);
extern sqInt stackIntegerValue(sqInt offset);
extern sqInt success(sqInt aBoolean);
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"JoystickTabletPlugin VMMaker.oscog-eem.1716 (i)"
#else
	"JoystickTabletPlugin VMMaker.oscog-eem.1716 (e)"
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

	/* JoystickTabletPlugin>>#initialiseModule */
EXPORT(sqInt)
initialiseModule(void)
{
	return joystickInit();
}


/*	Get information on the pen tablet attached to this machine. Fail if there
	is no tablet. If successful, the result is an array of integers; see the
	Smalltalk call on this primitive for its interpretation.
 */

	/* JoystickTabletPlugin>>#primitiveGetTabletParameters: */
EXPORT(sqInt)
primitiveGetTabletParameters(void)
{
	sqInt cursorIndex;
	sqInt result;
	int *  resultPtr;
	sqInt resultSize;

	cursorIndex = stackIntegerValue(0);
	if (failed()) {
		return null;
	}
	resultSize = tabletResultSize();
	result = instantiateClassindexableSize(classBitmap(), resultSize);
	resultPtr = ((int *)firstIndexableField(result));
	success(tabletGetParameters(cursorIndex, resultPtr));
	if (failed()) {
		return null;
	}
	popthenPush(2, result);
	return null;
}


/*	Read an input word from the joystick with the given index. */

	/* JoystickTabletPlugin>>#primitiveReadJoystick: */
EXPORT(sqInt)
primitiveReadJoystick(void)
{
	sqInt index;
	sqInt _return_value;

	index = stackIntegerValue(0);
	if (failed()) {
		return null;
	}
	if (failed()) {
		return null;
	}
	_return_value = positive32BitIntegerFor((joystickRead(index)));
	if (failed()) {
		return null;
	}
	popthenPush(2, _return_value);
	return null;
}


/*	Get the current state of the cursor of the pen tablet specified by my
	argument. Fail if there is no tablet. If successful, the result is an
	array of integers; see the Smalltalk call on this primitive for its
	interpretation. 
 */

	/* JoystickTabletPlugin>>#primitiveReadTablet: */
EXPORT(sqInt)
primitiveReadTablet(void)
{
	sqInt cursorIndex;
	sqInt result;
	int *  resultPtr;
	sqInt resultSize;

	cursorIndex = stackIntegerValue(0);
	if (failed()) {
		return null;
	}
	resultSize = tabletResultSize();
	result = instantiateClassindexableSize(classBitmap(), resultSize);
	resultPtr = ((int *)firstIndexableField(result));
	success(tabletRead(cursorIndex, resultPtr));
	if (failed()) {
		return null;
	}
	popthenPush(2, result);
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
		classBitmap = interpreterProxy->classBitmap;
		failed = interpreterProxy->failed;
		firstIndexableField = interpreterProxy->firstIndexableField;
		instantiateClassindexableSize = interpreterProxy->instantiateClassindexableSize;
		popthenPush = interpreterProxy->popthenPush;
		positive32BitIntegerFor = interpreterProxy->positive32BitIntegerFor;
		stackIntegerValue = interpreterProxy->stackIntegerValue;
		success = interpreterProxy->success;
#endif /* !defined(SQUEAK_BUILTIN_PLUGIN) */
	}
	return ok;
}

	/* JoystickTabletPlugin>>#shutdownModule */
EXPORT(sqInt)
shutdownModule(void)
{
	return joystickShutdown();
}

	/* SmartSyntaxInterpreterPlugin>>#sqAssert: */
static sqInt
sqAssert(sqInt aBool)
{
	/* missing DebugCode */;
	return aBool;
}


#ifdef SQUEAK_BUILTIN_PLUGIN

static char _m[] = "JoystickTabletPlugin";
void* JoystickTabletPlugin_exports[][3] = {
	{(void*)_m, "getModuleName", (void*)getModuleName},
	{(void*)_m, "initialiseModule", (void*)initialiseModule},
	{(void*)_m, "primitiveGetTabletParameters\000\000", (void*)primitiveGetTabletParameters},
	{(void*)_m, "primitiveReadJoystick\000\000", (void*)primitiveReadJoystick},
	{(void*)_m, "primitiveReadTablet\000\000", (void*)primitiveReadTablet},
	{(void*)_m, "setInterpreter", (void*)setInterpreter},
	{(void*)_m, "shutdownModule\000\377", (void*)shutdownModule},
	{NULL, NULL, NULL}
};

#else /* ifdef SQ_BUILTIN_PLUGIN */

signed char primitiveGetTabletParametersAccessorDepth = 0;
signed char primitiveReadJoystickAccessorDepth = 0;
signed char primitiveReadTabletAccessorDepth = 0;

#endif /* ifdef SQ_BUILTIN_PLUGIN */
