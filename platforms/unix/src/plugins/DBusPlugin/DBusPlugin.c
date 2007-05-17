/* Automatically generated from Squeak on an Array(15 May 2007 6:42:31 pm) */

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
#include "aio.h"
	#define DBUS_API_SUBJECT_TO_CHANGE
#include <dbus/dbus.h>
	struct sqDBusData{
		DBusConnection* con;
		DBusWatch* watch;
		int semaphore;
	};
	typedef struct sqDBusData* dbusWatchData;

#include "sqMemoryAccess.h"


/*** Constants ***/

/*** Function Prototypes ***/
static sqInt appendArgumentstosignature(sqInt argsOop, DBusMessageIter* msgIter, char * signature);
static char* appendArgumentwithSignaturetoIter(sqInt argOop, char * sig, DBusMessageIter* msgIter);
static sqInt argumentsAddArraytoIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddBooltoIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddDoubletoIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddInt32toIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddStringtoIter(sqInt oop, DBusMessageIter* iter);
static sqInt buildStringOopFromCharP(const char* charP);
static DBusMessage * createErrorTofrom(char* dest, sqInt msgOop);
static DBusMessage * createReplyTofrom(char* dest, sqInt msgOop);
static sqInt fetchBooleanofObject(sqInt n, sqInt oop);
static char * fetchStringofObject(sqInt n, sqInt oop);
static DBusConnection * getConnectionFromOop(sqInt oop);
static VirtualMachine * getInterpreter(void);
#pragma export on
EXPORT(const char*) getModuleName(void);
#pragma export off
static DBusConnection * getSessionBusConnection(void);
static DBusConnection * getSystemBusConnection(void);
static sqInt halt(void);
static sqInt handleReadForFDwithDataandFlag(int fd, dbusWatchData data, int flag);
static sqInt handleflag(int fd, int flag);
#pragma export on
EXPORT(sqInt) initialiseModule(void);
#pragma export off
static sqInt msg(char * s);
static dbusWatchData newDataStructWitchConnectionandSemaphore(DBusConnection * con, int semaphore);
static sqInt pendingCallNotifyFunction(void);
static sqInt pendingCallNotifyFunctionData(DBusPendingCall* pendingCall, void* data);
#pragma export on
EXPORT(sqInt) primitiveAnswerSeventeen(void);
EXPORT(sqInt) primitiveDBusAddMatch(void);
EXPORT(sqInt) primitiveDBusArgumentGetArray(void);
EXPORT(sqInt) primitiveDBusArgumentGetBool(void);
EXPORT(sqInt) primitiveDBusArgumentGetDictionary(void);
EXPORT(sqInt) primitiveDBusArgumentGetDouble(void);
EXPORT(sqInt) primitiveDBusArgumentGetInt32(void);
EXPORT(sqInt) primitiveDBusArgumentGetObjectPath(void);
EXPORT(sqInt) primitiveDBusArgumentGetString(void);
EXPORT(sqInt) primitiveDBusArgumentGetStruct(void);
EXPORT(sqInt) primitiveDBusArgumentGetType(void);
EXPORT(sqInt) primitiveDBusArgumentGetVariant(void);
EXPORT(sqInt) primitiveDBusConnectionDispatch(void);
EXPORT(sqInt) primitiveDBusConnectionDispatchStatus(void);
EXPORT(sqInt) primitiveDBusConnectionPopMessage(void);
EXPORT(sqInt) primitiveDBusConnectionRegisterSemaphore(void);
EXPORT(sqInt) primitiveDBusConnectToSessionBus(void);
EXPORT(sqInt) primitiveDBusConnectToSystemBus(void);
EXPORT(sqInt) primitiveDBusMessageGetDestination(void);
EXPORT(sqInt) primitiveDBusMessageGetErrorName(void);
EXPORT(sqInt) primitiveDBusMessageGetInterface(void);
EXPORT(sqInt) primitiveDBusMessageGetMember(void);
EXPORT(sqInt) primitiveDBusMessageGetNoReply(void);
EXPORT(sqInt) primitiveDBusMessageGetPath(void);
EXPORT(sqInt) primitiveDBusMessageGetReplySerial(void);
EXPORT(sqInt) primitiveDBusMessageGetSender(void);
EXPORT(sqInt) primitiveDBusMessageGetSerial(void);
EXPORT(sqInt) primitiveDBusMessageGetSignature(void);
EXPORT(sqInt) primitiveDBusNextIterator(void);
EXPORT(sqInt) primitiveDBusPopIterator(void);
EXPORT(sqInt) primitiveDBusReadWriteConnection(void);
EXPORT(sqInt) primitiveDBusRegisterName(void);
EXPORT(sqInt) primitiveDBusReleaseName(void);
EXPORT(sqInt) primitiveDBusRemoveMatch(void);
EXPORT(sqInt) primitiveDBusSendMessage(void);
EXPORT(sqInt) setInterpreter(struct VirtualMachine* anInterpreter);
EXPORT(sqInt) shutdownModule(void);
#pragma export off
static sqInt sqAssert(sqInt aBool);
static sqInt sqDBusPluginAddWatchwithData(DBusWatch* watch, dbusWatchData data);
static sqInt sqDBusPluginFreeWatchData(dbusWatchData data);
static sqInt sqDBusPluginRemoveWatchwithData(DBusWatch* watch, dbusWatchData data);
static sqInt sqDBusPluginToggleWatchwithData(DBusWatch* watch, dbusWatchData data);
static char * stringOopToChar(sqInt oop);
/*** Variables ***/
static DBusConnection* connections[2];
static sqInt currIter;
static dbus_bool_t hasArgument;

#ifdef SQUEAK_BUILTIN_PLUGIN
extern
#endif
struct VirtualMachine* interpreterProxy;
static DBusMessage* message;
static DBusMessageIter messageIter[DBUS_MAXIMUM_TYPE_RECURSION_DEPTH];
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"DBusPlugin 15 May 2007 (i)"
#else
	"DBusPlugin 15 May 2007 (e)"
#endif
;


static sqInt appendArgumentstosignature(sqInt argsOop, DBusMessageIter* msgIter, char * signature) {
	sqInt i;
	char* s;
	int argCount;
	sqInt valOop;
	int * arg;

	argCount = interpreterProxy->stSizeOf(argsOop);
	arg = ((int *) (interpreterProxy->firstIndexableField(argsOop)));
	s = signature;
	for (i = 0; i <= (argCount - 1); i += 1) {
		valOop = arg[i];
		s = appendArgumentwithSignaturetoIter(valOop, s, msgIter);
	}
}

static char* appendArgumentwithSignaturetoIter(sqInt argOop, char * sig, DBusMessageIter* msgIter) {
	char s;

	s = 's';
	s = *sig;
	if (s == 'i') {
		argumentsAddInt32toIter(argOop, msgIter);
	} else {
		if (s == 'd') {
			argumentsAddDoubletoIter(argOop, msgIter);
		} else {
			if (s == 's') {
				argumentsAddStringtoIter(argOop, msgIter);
			} else {
				if (s == 'b') {
					argumentsAddBooltoIter(argOop, msgIter);
				} else {
					msg("unknown argument signature");
					interpreterProxy->primitiveFail();
				}
			}
		}
	}
	return sig + 1;
}

static sqInt argumentsAddArraytoIter(sqInt oop, DBusMessageIter* iter) {
	int arg;

	arg = 1;
	if (oop == (interpreterProxy->falseObject())) {
		arg = 0;
	}
	if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_BOOLEAN, &arg))) {
		msg("Can not add Boolean argument.");
		return interpreterProxy->primitiveFail();
	}
}

static sqInt argumentsAddBooltoIter(sqInt oop, DBusMessageIter* iter) {
	int arg;

	arg = 1;
	if (oop == (interpreterProxy->falseObject())) {
		arg = 0;
	}
	if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_BOOLEAN, &arg))) {
		msg("Can not add Boolean argument.");
		return interpreterProxy->primitiveFail();
	}
}

static sqInt argumentsAddDoubletoIter(sqInt oop, DBusMessageIter* iter) {
	double arg;

	if (!(interpreterProxy->isFloatObject(oop))) {
		return interpreterProxy->primitiveFail();
	}
	arg = interpreterProxy->floatValueOf(oop);
	if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_DOUBLE, &arg))) {
		msg("Can not add Double argument.");
		return interpreterProxy->primitiveFail();
	}
}

static sqInt argumentsAddInt32toIter(sqInt oop, DBusMessageIter* iter) {
	int intArg;

	intArg = interpreterProxy->checkedIntegerValueOf(oop);
	if (interpreterProxy->failed()) {
		return interpreterProxy->primitiveFail();
	}
	if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_INT32, &intArg))) {
		msg("Can not add Int32 argument.");
		return interpreterProxy->primitiveFail();
	}
}

static sqInt argumentsAddStringtoIter(sqInt oop, DBusMessageIter* iter) {
	char* arg;

	arg = stringOopToChar(oop);
	if (!(arg == null)) {
		if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_STRING, &arg))) {
			msg("Can not add String argument.");
		}
		free(arg);
	}
}

static sqInt buildStringOopFromCharP(const char* charP) {
	int size;
	sqInt strObj;
	char * strPtr;

	if (charP == null) {
		return interpreterProxy->nilObject();
	}
	size = 0;
	size = strlen(charP);
	strObj = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classString(), size);
	if (interpreterProxy->failed()) {
		return null;
	}
	strPtr = ((char *) interpreterProxy->firstIndexableField(strObj));
	memcpy(strPtr, charP, size);
	return strObj;
}

static DBusMessage * createErrorTofrom(char* dest, sqInt msgOop) {
	DBusMessage * error;
	dbus_uint32_t serial;
	char * name;
	sqInt serialOop;
	sqInt p;

	serialOop = interpreterProxy->fetchPointerofObject(8, msgOop);
	if (interpreterProxy->failed()) {
		return null;
	}

	/* extract the name of the error oop */

	serial = interpreterProxy->positive32BitValueOf(serialOop);
	/* begin fetchString:ofObject: */
	p = interpreterProxy->fetchPointerofObject(9, msgOop);
	name = stringOopToChar(p);
	error = null;
	error = dbus_message_new(DBUS_MESSAGE_TYPE_ERROR);
				dbus_message_set_destination(error, dest);
				dbus_message_set_error_name(error, name);
			     dbus_message_set_no_reply(error, TRUE);
				dbus_message_set_reply_serial(error, serial);
				free(name);
	return error;
}

static DBusMessage * createReplyTofrom(char* dest, sqInt msgOop) {
	dbus_uint32_t serial;
	sqInt serialOop;
	DBusMessage * reply;

	serialOop = interpreterProxy->fetchPointerofObject(8, msgOop);
	if (interpreterProxy->failed()) {
		return null;
	}

	/* create a return message */

	serial = interpreterProxy->positive32BitValueOf(serialOop);
	reply = null;
	reply = dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_RETURN);
				dbus_message_set_destination(reply, dest);
			     dbus_message_set_no_reply(reply, TRUE);
				dbus_message_set_reply_serial(reply, serial);
	return reply;
}

static sqInt fetchBooleanofObject(sqInt n, sqInt oop) {
	sqInt p;
	sqInt b;

	p = interpreterProxy->fetchPointerofObject(n, oop);
	b = booleanValueOf(p);
	if (interpreterProxy->failed()) {
		msg("Can not fetch a string from oop");
		interpreterProxy->primitiveFail();
	}
	return b;
}

static char * fetchStringofObject(sqInt n, sqInt oop) {
	sqInt p;

	p = interpreterProxy->fetchPointerofObject(n, oop);
	return stringOopToChar(p);
}

static DBusConnection * getConnectionFromOop(sqInt oop) {
	DBusConnection* conn;
	int h;

	h = interpreterProxy->fetchIntegerofObject(0, oop);
	if (h == 1) {
		conn = getSystemBusConnection();
	}
	conn = getSessionBusConnection();
	if (conn == null) {
		msg("Connection is nil");
		interpreterProxy->primitiveFail();
		return null;
	}
	return conn;
}


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

static DBusConnection * getSessionBusConnection(void) {
	DBusError error;
	DBusConnection* con;

	if ((connections[0]) != null) {
		return connections[0];
	}
	dbus_error_init(&error);

	/* connect to the DBus system and check for error */

	con = null;
	con = dbus_bus_get_private( DBUS_BUS_SESSION, &error);
	if (dbus_error_is_set (&error)) {
		msg(((char*) (error.message)));
		dbus_error_free(&error);
		return null;
	}
	connections[0] = con;
	return con;
}

static DBusConnection * getSystemBusConnection(void) {
	DBusError error;
	DBusConnection* con;

	if (!((connections[1]) == null)) {
		return connections[1];
	}
	dbus_error_init(&error);

	/* connect to the system bus and check for an error */

	con = null;
	con = dbus_bus_get_private( DBUS_BUS_SYSTEM, &error);
	if (dbus_error_is_set (&error)) {
		msg(((char*) (error.message)));
		dbus_error_free(&error);
		return null;
	}
	connections[1] = con;
	return connections[1];
}

static sqInt halt(void) {
	;
}

static sqInt handleReadForFDwithDataandFlag(int fd, dbusWatchData data, int flag) {
	sqInt semaphore;

	dbus_watch_handle(data->watch, DBUS_WATCH_READABLE);
	handleflag(fd, flag);
	semaphore = data->semaphore;
	interpreterProxy->signalSemaphoreWithIndex(semaphore);
}

static sqInt handleflag(int fd, int flag) {
	aioHandle(fd, handleReadForFDwithDataandFlag , flag);
}

EXPORT(sqInt) initialiseModule(void) {
	connections[0] = null;
	connections[1] = null;
	return 1;
}

static sqInt msg(char * s) {
	fprintf(stderr, "\n%s: %s", moduleName, s);
}

static dbusWatchData newDataStructWitchConnectionandSemaphore(DBusConnection * con, int semaphore) {
	dbusWatchData t;

	t = null;
	t = (dbusWatchData)malloc(sizeof(dbusWatchData));
	if (t == null) {
		msg("Cannot allocate memory for data structure");
		return null;
	}
	t->con = con; 
				t->semaphore = semaphore;
				t->watch = NULL;
	return t;
}

static sqInt pendingCallNotifyFunction(void) {
	msg("Notify something");
}

static sqInt pendingCallNotifyFunctionData(DBusPendingCall* pendingCall, void* data) {
	msg("Notify something");
}

EXPORT(sqInt) primitiveAnswerSeventeen(void) {
	int msgType;
	sqInt m;
	sqInt _return_value;

	m = interpreterProxy->stackValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	msg("check oop");
	msgType = interpreterProxy->fetchIntegerofObject(0, m);
	if (interpreterProxy->failed()) {
		interpreterProxy->primitiveFail();
		return null;
	}
	_return_value = interpreterProxy->integerObjectOf((msgType + 20));
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(2, _return_value);
	return null;
}


/*	Register match rule */

EXPORT(sqInt) primitiveDBusAddMatch(void) {
	DBusConnection * conn;
	DBusError error;
	sqInt rcvOop;
	char *rule;
	sqInt _return_value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	rule = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	rcvOop = interpreterProxy->stackValue(1);
	if (interpreterProxy->failed()) {
		return null;
	}

	/* initialize error value */

	conn = getConnectionFromOop(rcvOop);
	dbus_error_init(&error);
	dbus_bus_add_match( conn, rule, &error);
		dbus_connection_flush(conn);
	if (dbus_error_is_set(&error)) {
		msg(((char*) (error.message)));
		dbus_error_free(&error);
		interpreterProxy->primitiveFail();
	}
	_return_value = interpreterProxy->trueObject();
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(2, _return_value);
	return null;
}


/*	returns a String argument) */

EXPORT(sqInt) primitiveDBusArgumentGetArray(void) {
	int type;
	char * str;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[currIter]);
	if (!(type == (DBUS_TYPE_ARRAY))) {
		msg("Wrong argument type: DBUS_TYPE_ARRAY.");
		interpreterProxy->primitiveFail();
		return null;
	}
	dbus_message_iter_recurse(&messageIter[currIter], &messageIter[currIter+1]);
	currIter += 1;
	hasArgument = 1;
	if (interpreterProxy->failed()) {
		return null;
	}
	return null;
}


/*	returns a bool argument) */

EXPORT(sqInt) primitiveDBusArgumentGetBool(void) {
	int value;
	int type;
	sqInt _return_value;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[currIter]);
	if (!(type == (DBUS_TYPE_BOOLEAN))) {
		msg("Wrong argument type: DBUS_TYPE_BOOLEAN.");
		interpreterProxy->primitiveFail();
		return null;
	}
	value = 0;
	dbus_message_iter_get_basic(&messageIter[currIter], &value );
	if (value) {
		_return_value = interpreterProxy->trueObject();
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(1, _return_value);
		return null;
	}
	_return_value = interpreterProxy->falseObject();
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns a String argument) */

EXPORT(sqInt) primitiveDBusArgumentGetDictionary(void) {
	int type;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[currIter]);
	if (!(type == (DBUS_TYPE_DICT_ENTRY))) {
		msg("Wrong argument type: DBUS_TYPE_DICT_ENTRY.");
		msg("Wrong argument type.");
		interpreterProxy->primitiveFail();
		return null;
	}
	dbus_message_iter_recurse(&messageIter[currIter], &messageIter[currIter+1]);
	currIter += 1;
	hasArgument = 1;
	if (interpreterProxy->failed()) {
		return null;
	}
	return null;
}


/*	returns a double argument) */

EXPORT(sqInt) primitiveDBusArgumentGetDouble(void) {
	double value;
	int type;
	sqInt _return_value;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[currIter]);
	if (!(type == (DBUS_TYPE_DOUBLE))) {
		msg("Wrong argument type: DBUS_TYPE_DOUBLE.");
		interpreterProxy->primitiveFail();
		return null;
	}
	value = 0;
	dbus_message_iter_get_basic(&messageIter[currIter], &value );
	_return_value = interpreterProxy->floatObjectOf(value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns a integer argument) */

EXPORT(sqInt) primitiveDBusArgumentGetInt32(void) {
	int value;
	int type;
	sqInt _return_value;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[currIter]);
	if (!(type == (DBUS_TYPE_INT32))) {
		msg("Wrong argument type: DBUS_TYPE_INT32.");
		interpreterProxy->primitiveFail();
		return null;
	}
	value = 0;
	dbus_message_iter_get_basic(&messageIter[currIter], &value );
	_return_value = interpreterProxy->signed32BitIntegerFor(value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns path to an object argument as a string */

EXPORT(sqInt) primitiveDBusArgumentGetObjectPath(void) {
	int type;
	char * str;
	sqInt _return_value;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[currIter]);
	if (!(type == (DBUS_TYPE_OBJECT_PATH))) {
		msg("Wrong argument type: DBUS_TYPE_OBJECT_PATH.");
		interpreterProxy->primitiveFail();
		return null;
	}
	str = "";
	dbus_message_iter_get_basic(&messageIter[currIter], &str );
	_return_value = buildStringOopFromCharP(str);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns a String argument) */

EXPORT(sqInt) primitiveDBusArgumentGetString(void) {
	int type;
	char * str;
	sqInt _return_value;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[currIter]);
	if (!(type == (DBUS_TYPE_STRING))) {
		msg("Wrong argument type: DBUS_TYPE_STRING.");
		interpreterProxy->primitiveFail();
		return null;
	}
	str = "";
	dbus_message_iter_get_basic(&messageIter[currIter], &str );
	_return_value = buildStringOopFromCharP(str);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns a String argument) */

EXPORT(sqInt) primitiveDBusArgumentGetStruct(void) {
	int type;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[currIter]);
	if (!(type == (DBUS_TYPE_STRUCT))) {
		msg("Wrong argument type: DBUS_TYPE_STRUCT.");
		interpreterProxy->primitiveFail();
		return null;
	}
	dbus_message_iter_recurse(&messageIter[currIter], &messageIter[currIter+1]);
	currIter += 1;
	hasArgument = 1;
	if (interpreterProxy->failed()) {
		return null;
	}
	return null;
}


/*	returns the type of the current argument or nil) */

EXPORT(sqInt) primitiveDBusArgumentGetType(void) {
	int type;
	sqInt _return_value;

	if (!(hasArgument)) {
		_return_value = interpreterProxy->nilObject();
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(1, _return_value);
		return null;
	}
	type = dbus_message_iter_get_arg_type(&messageIter[currIter]);
	_return_value = interpreterProxy->integerObjectOf(type);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	push an iterator for reading a variant */

EXPORT(sqInt) primitiveDBusArgumentGetVariant(void) {
	int type;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[currIter]);
	if (!(type == (DBUS_TYPE_VARIANT))) {
		msg("Wrong argument type: DBUS_TYPE_VARIANT.");
		interpreterProxy->primitiveFail();
		return null;
	}
	dbus_message_iter_recurse(&messageIter[currIter], &messageIter[currIter+1]);
	currIter += 1;
	hasArgument = 1;
	if (interpreterProxy->failed()) {
		return null;
	}
	return null;
}


/*	read the connection for next available message */

EXPORT(sqInt) primitiveDBusConnectionDispatch(void) {
	DBusConnection * conn;
	sqInt rcvOop;

	rcvOop = interpreterProxy->stackValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	conn = getConnectionFromOop(rcvOop);
	dbus_connection_dispatch(conn);
	if (interpreterProxy->failed()) {
		return null;
	}
	return null;
}


/*	answer the status of the connection */

EXPORT(sqInt) primitiveDBusConnectionDispatchStatus(void) {
	DBusConnection* conn;
	sqInt status;
	sqInt rcvOop;
	sqInt _return_value;

	rcvOop = interpreterProxy->stackValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	conn = getConnectionFromOop(rcvOop);
	status = null;
	status = dbus_connection_get_dispatch_status(conn);
	_return_value = interpreterProxy->integerObjectOf(status);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	read the connection for next available message */

EXPORT(sqInt) primitiveDBusConnectionPopMessage(void) {
	int msgType;
	DBusConnection * conn;
	sqInt rcvOop;
	sqInt _return_value;

	rcvOop = interpreterProxy->stackValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}

	/* clear the old message */

	conn = getConnectionFromOop(rcvOop);
	if (!(message == null)) {
		hasArgument = 0;
		currIter = 0;
		dbus_message_unref(message);
	}
	message = dbus_connection_pop_message(conn);
	if (message == null) {
		_return_value = interpreterProxy->integerObjectOf(0);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(1, _return_value);
		return null;
	}

	/* if message contains argument initialize iterator */

	msgType = dbus_message_get_type(message);
	hasArgument = dbus_message_iter_init(message, &(messageIter[currIter]));
	_return_value = interpreterProxy->integerObjectOf(msgType);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	Registers watch functions for which will called when dbus want to register/remove a watch. The semaphore will signaled when dbus recives new messages */

EXPORT(sqInt) primitiveDBusConnectionRegisterSemaphore(void) {
	DBusConnection* conn;
	sqInt rcvOop;
	dbusWatchData data;
	sqInt semaphore;

	semaphore = interpreterProxy->stackIntegerValue(0);
	rcvOop = interpreterProxy->stackValue(1);
	if (interpreterProxy->failed()) {
		return null;
	}

	/* create a new data structure */

	conn = getConnectionFromOop(rcvOop);
	data = newDataStructWitchConnectionandSemaphore(conn, semaphore);
	if (data == null) {
		interpreterProxy->primitiveFail();
		return null;
	}
	if (!(dbus_connection_set_watch_functions(conn, (DBusAddWatchFunction) sqDBusPluginAddWatchwithData,
															(DBusRemoveWatchFunction) sqDBusPluginRemoveWatchwithData,
															(DBusWatchToggledFunction) sqDBusPluginToggleWatchwithData, 
															data,
															(DBusFreeFunction) sqDBusPluginFreeWatchData))) {
		msg("Can not set the watch functions");
		interpreterProxy->primitiveFail();
		return null;
	}
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(1);
	return null;
}


/*	Creates a new connection to the system bus and returns a handle or nil */

EXPORT(sqInt) primitiveDBusConnectToSessionBus(void) {
	sqInt _return_value;

	if ((getSessionBusConnection()) != null) {
		_return_value = interpreterProxy->integerObjectOf(0);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(1, _return_value);
		return null;
	}
	_return_value = interpreterProxy->nilObject();
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	Creates a new connection to the system bus and returns a handle or nil */

EXPORT(sqInt) primitiveDBusConnectToSystemBus(void) {
	sqInt _return_value;

	if (!((getSystemBusConnection()) == null)) {
		_return_value = interpreterProxy->integerObjectOf(1);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(1, _return_value);
		return null;
	}
	_return_value = interpreterProxy->nilObject();
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns the destination of the received message or nil if there is none set */

EXPORT(sqInt) primitiveDBusMessageGetDestination(void) {
	const char * str;
	sqInt _return_value;

	str = null;
	str = dbus_message_get_destination(message);
	if (str == null) {
		return null;
	}
	_return_value = buildStringOopFromCharP(str);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns the error name (ERROR only) */

EXPORT(sqInt) primitiveDBusMessageGetErrorName(void) {
	int msgType;
	const char * str;
	sqInt _return_value;


	/* check if it is an error message */

	msgType = 0;
	msgType = dbus_message_get_type(message);
	if (!(msgType == 3)) {
		_return_value = interpreterProxy->nilObject();
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(1, _return_value);
		return null;
	}
	str = dbus_message_get_error_name(message);
	_return_value = buildStringOopFromCharP(str);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns the interface the message is being sent to (METHOD_CALL) or emitted from (SIGNAL) */

EXPORT(sqInt) primitiveDBusMessageGetInterface(void) {
	const char * str;
	sqInt _return_value;

	str = dbus_message_get_interface(message);
	_return_value = buildStringOopFromCharP(str);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns the interface member beeing invoked (METHOD_CALL) or emitted (SIGNAL) */

EXPORT(sqInt) primitiveDBusMessageGetMember(void) {
	const char * str;
	sqInt _return_value;

	str = null;
	str = dbus_message_get_member(message);
	_return_value = buildStringOopFromCharP(str);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns true if the method call expects no reply otherwise returns false */

EXPORT(sqInt) primitiveDBusMessageGetNoReply(void) {
	sqInt _return_value;

	if (dbus_message_get_no_reply(message)) {
		_return_value = interpreterProxy->trueObject();
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(1, _return_value);
		return null;
	}
	_return_value = interpreterProxy->falseObject();
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns the object path the message is being sent to (METHOD_CALL) or being emitted from (SIGNAL) */

EXPORT(sqInt) primitiveDBusMessageGetPath(void) {
	const char * path;
	sqInt _return_value;

	path = dbus_message_get_path(message);
	_return_value = buildStringOopFromCharP(path);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns the serial of the message to which this message is a reply or 0 if none has been specified */

EXPORT(sqInt) primitiveDBusMessageGetReplySerial(void) {
	dbus_uint32_t serial;
	sqInt _return_value;

	serial = 0;
	serial = dbus_message_get_reply_serial(message);
	_return_value = interpreterProxy->positive32BitIntegerFor(serial);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns the unique name of the connection which orginatet the message, or nil if unknown or inapplicable */

EXPORT(sqInt) primitiveDBusMessageGetSender(void) {
	const char * str;
	sqInt _return_value;

	str = null;
	str = dbus_message_get_sender(message);
	if (str == null) {
		return null;
	}
	_return_value = buildStringOopFromCharP(str);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns the serial of the message or 0 if none has been specified */

EXPORT(sqInt) primitiveDBusMessageGetSerial(void) {
	dbus_uint32_t serial;
	sqInt _return_value;

	serial = 0;
	serial = dbus_message_get_serial(message);
	_return_value = interpreterProxy->positive32BitIntegerFor(serial);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns the type signature of the message */

EXPORT(sqInt) primitiveDBusMessageGetSignature(void) {
	const char * str;
	sqInt _return_value;

	str = null;
	str = dbus_message_get_signature(message);
	if (str == null) {
		return null;
	}
	_return_value = buildStringOopFromCharP(str);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns a String argument) */

EXPORT(sqInt) primitiveDBusNextIterator(void) {
	sqInt _return_value;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	hasArgument = dbus_message_iter_next(&messageIter[currIter]);
	_return_value = (hasArgument? interpreterProxy->trueObject(): interpreterProxy->falseObject());
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	pops a message iterator from the iterator stack */

EXPORT(sqInt) primitiveDBusPopIterator(void) {
	sqInt _return_value;

	if (currIter > 0) {
		currIter -= 1;
		hasArgument = 1;
		_return_value = interpreterProxy->trueObject();
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(1, _return_value);
		return null;
	}
	_return_value = interpreterProxy->falseObject();
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	read the connection for next available message */

EXPORT(sqInt) primitiveDBusReadWriteConnection(void) {
	int msgType;
	DBusConnection * conn;
	sqInt rcvOop;
	sqInt _return_value;

	rcvOop = interpreterProxy->stackValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}

	/* clear the old message */

	conn = getConnectionFromOop(rcvOop);
	if (!(message == null)) {
		hasArgument = 0;
		currIter = 0;
		dbus_message_unref(message);
	}
	dbus_connection_dispatch(conn);
		message = dbus_connection_pop_message(conn);
	if (message == null) {
		_return_value = interpreterProxy->integerObjectOf(0);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(1, _return_value);
		return null;
	}

	/* if message contains argument initialize iterator */

	msgType = dbus_message_get_type(message);
	hasArgument = dbus_message_iter_init(message, &(messageIter[currIter]));
	_return_value = interpreterProxy->integerObjectOf(msgType);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	Register Name */

EXPORT(sqInt) primitiveDBusRegisterName(void) {
	DBusConnection * conn;
	DBusError error;
	sqInt rcvOop;
	int ret;
	char *name;
	sqInt _return_value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	name = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	rcvOop = interpreterProxy->stackValue(1);
	if (interpreterProxy->failed()) {
		return null;
	}
	dbus_error_init(&error);

	/* initialize return value */

	conn = getConnectionFromOop(rcvOop);
	ret = 0;
	ret = dbus_bus_request_name( conn, name, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
	if (dbus_error_is_set(&error)) {
		msg(((char*) (error.message)));
		dbus_error_free(&error);
		interpreterProxy->primitiveFail();
	}
	_return_value = interpreterProxy->integerObjectOf(ret);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(2, _return_value);
	return null;
}


/*	Release Name */

EXPORT(sqInt) primitiveDBusReleaseName(void) {
	DBusConnection * conn;
	DBusError error;
	sqInt rcvOop;
	int ret;
	char *name;
	sqInt _return_value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	name = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	rcvOop = interpreterProxy->stackValue(1);
	if (interpreterProxy->failed()) {
		return null;
	}
	dbus_error_init(&error);

	/* initialize return value */

	conn = getConnectionFromOop(rcvOop);
	ret = 0;
	ret = dbus_bus_release_name( conn, name, &error);
	if (dbus_error_is_set(&error)) {
		msg(((char*) (error.message)));
		dbus_error_free(&error);
		interpreterProxy->primitiveFail();
	}
	_return_value = interpreterProxy->integerObjectOf(ret);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(2, _return_value);
	return null;
}


/*	Remove match rule */

EXPORT(sqInt) primitiveDBusRemoveMatch(void) {
	DBusConnection * conn;
	DBusError error;
	sqInt rcvOop;
	char *rule;
	sqInt _return_value;

	interpreterProxy->success(interpreterProxy->isBytes(interpreterProxy->stackValue(0)));
	rule = ((char *) (interpreterProxy->firstIndexableField(interpreterProxy->stackValue(0))));
	rcvOop = interpreterProxy->stackValue(1);
	if (interpreterProxy->failed()) {
		return null;
	}
	dbus_error_init(&error);

	/* add match rule */

	conn = getConnectionFromOop(rcvOop);
	dbus_bus_remove_match( conn, rule, &error);
				dbus_connection_flush(conn);
	if (dbus_error_is_set(&error)) {
		msg(((char*) (error.message)));
		dbus_error_free(&error);
		interpreterProxy->primitiveFail();
	}
	_return_value = interpreterProxy->trueObject();
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(2, _return_value);
	return null;
}


/*	Send a broadcast signal to the DBus deamon */

EXPORT(sqInt) primitiveDBusSendMessage(void) {
	DBusConnection * conn;
	char * signature;
	DBusError error;
	char * destination;
	sqInt messageWasSend;
	sqInt rcvOop;
	char * interface;
	sqInt sendWithReply;
	int msgType;
	char * path;
	DBusMessageIter argIter;
	DBusMessage * tmpMsg;
	dbus_uint32_t serial;
	char * method;
	sqInt m;
	sqInt args;
	sqInt _return_value;
	sqInt p;
	sqInt b;
	sqInt p1;
	sqInt p2;
	sqInt p3;
	sqInt p4;
	sqInt p5;

	m = interpreterProxy->stackValue(1);
	args = interpreterProxy->stackValue(0);
	rcvOop = interpreterProxy->stackValue(2);
	if (interpreterProxy->failed()) {
		return null;
	}
	msgType = interpreterProxy->fetchIntegerofObject(0, m);
	if (interpreterProxy->failed()) {
		msg("Can not fetch message type.");
		interpreterProxy->primitiveFail();
		return null;
	}

	/* extract path from the Oop */

	sendWithReply = 0;
	/* begin fetchString:ofObject: */
	p1 = interpreterProxy->fetchPointerofObject(1, m);
	path = stringOopToChar(p1);
	/* begin fetchString:ofObject: */
	p2 = interpreterProxy->fetchPointerofObject(2, m);
	interface = stringOopToChar(p2);
	/* begin fetchString:ofObject: */
	p3 = interpreterProxy->fetchPointerofObject(3, m);
	method = stringOopToChar(p3);
	/* begin fetchString:ofObject: */
	p4 = interpreterProxy->fetchPointerofObject(5, m);
	destination = stringOopToChar(p4);
	/* begin fetchString:ofObject: */
	p5 = interpreterProxy->fetchPointerofObject(6, m);
	signature = stringOopToChar(p5);
	dbus_error_init(&error);

	/* create new msg */

	conn = getConnectionFromOop(rcvOop);

	/* create Signal */

	tmpMsg = null;
	if (msgType == 4) {
		tmpMsg = dbus_message_new_signal(path, interface, method);
	}
	if (msgType == 1) {
		tmpMsg = dbus_message_new_method_call(destination, path, interface, method);
		/* begin fetchBoolean:ofObject: */
		p = interpreterProxy->fetchPointerofObject(9, m);
		b = booleanValueOf(p);
		if (interpreterProxy->failed()) {
			msg("Can not fetch a string from oop");
			interpreterProxy->primitiveFail();
		}
		sendWithReply = b;
	}
	if (msgType == 2) {
		tmpMsg = createReplyTofrom(destination, m);
	}
	if (msgType == 3) {
		tmpMsg = createErrorTofrom(destination, m);
	}
	if (tmpMsg == null) {
		msg("Can not create a message.");
		interpreterProxy->primitiveFail();
		return null;
	}
	dbus_message_iter_init_append(tmpMsg, &argIter);
	appendArgumentstosignature(args, &argIter, signature);
	if (!(interpreterProxy->failed())) {
		serial = 0;
		messageWasSend = interpreterProxy->trueObject();
		if (sendWithReply) {
			if (!(dbus_connection_send_with_reply(conn, tmpMsg, NULL, -1))) {
				messageWasSend = interpreterProxy->falseObject();
			}
			serial = dbus_message_get_serial(tmpMsg);
		} else {
			if (!(dbus_connection_send(conn, tmpMsg, &serial))) {
				messageWasSend = interpreterProxy->falseObject();
			}
		}
		dbus_connection_flush(conn);
		dbus_message_unref(tmpMsg);
	}
	if (!(path == null)) {
		free(path);
	}
	if (!(interface == null)) {
		free(interface);
	}
	if (!(method == null)) {
		free(method);
	}
	if (!(destination == null)) {
		free(destination);
	}
	if (!(signature == null)) {
		free(signature);
	}
	if (!(messageWasSend)) {
		msg("msg was not send");
	}
	_return_value = interpreterProxy->integerObjectOf(serial);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(3, _return_value);
	return null;
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


/*	if connected to a bus disconnect */

EXPORT(sqInt) shutdownModule(void) {
	sqInt i;

	for (i = 0; i <= 1; i += 1) {
		if ((connections[i]) != null) {
			dbus_connection_close(connections[i]);
				dbus_connection_unref(connections[i]);
			connections[i == null];
		}
	}
	return 1;
}

static sqInt sqAssert(sqInt aBool) {
	/* missing DebugCode */;
}


/*	function which is called when the dbus want to add a watch handle */

static sqInt sqDBusPluginAddWatchwithData(DBusWatch* watch, dbusWatchData data) {
	sqInt fd;
	sqInt flag;


	/* get the file descriptor of the watch */

	flag = dbus_watch_get_flags(watch);

	/* set watch in data structure */

	fd = dbus_watch_get_fd(watch);
	data->watch = watch;
	if (flag & DBUS_WATCH_READABLE) {
		aioEnable(fd, data, NULL);
							aioHandle(fd, handleReadForFDwithDataandFlag , 1<<0 | 1<<1 | 1<<3);
	}
}

static sqInt sqDBusPluginFreeWatchData(dbusWatchData data) {
	free(data);
}


/*	function which is called when the dbus want to add a watch handle */

static sqInt sqDBusPluginRemoveWatchwithData(DBusWatch* watch, dbusWatchData data) {
	sqInt fd;

	fd = dbus_watch_get_fd(watch);
	aioDisable(fd);
}


/*	 */

static sqInt sqDBusPluginToggleWatchwithData(DBusWatch* watch, dbusWatchData data) {
	sqInt enable;
	sqInt fd;

	enable = dbus_watch_get_enabled(watch);
	fd = dbus_watch_get_fd(watch);
	if (enable) {
		data->watch = watch;
		aioEnable(fd, data, NULL);
	} else {
		aioDisable(fd);
	}
}

static char * stringOopToChar(sqInt oop) {
	int size;
	char * tmpStr;
	char * t;

	tmpStr = ((char*) (interpreterProxy->arrayValueOf(oop)));
	if (interpreterProxy->failed()) {
		msg("not a string");
		return null;
	}
	size = interpreterProxy->byteSizeOf(oop);
	t = ((char*) (malloc(size+1)));
	memcpy(t, tmpStr, size);
	t[size] = 0;
	return t;
}


#ifdef SQUEAK_BUILTIN_PLUGIN


void* DBusPlugin_exports[][3] = {
	{"DBusPlugin", "primitiveAnswerSeventeen", (void*)primitiveAnswerSeventeen},
	{"DBusPlugin", "primitiveDBusRemoveMatch", (void*)primitiveDBusRemoveMatch},
	{"DBusPlugin", "getModuleName", (void*)getModuleName},
	{"DBusPlugin", "primitiveDBusMessageGetSerial", (void*)primitiveDBusMessageGetSerial},
	{"DBusPlugin", "primitiveDBusArgumentGetArray", (void*)primitiveDBusArgumentGetArray},
	{"DBusPlugin", "primitiveDBusArgumentGetObjectPath", (void*)primitiveDBusArgumentGetObjectPath},
	{"DBusPlugin", "primitiveDBusRegisterName", (void*)primitiveDBusRegisterName},
	{"DBusPlugin", "primitiveDBusArgumentGetVariant", (void*)primitiveDBusArgumentGetVariant},
	{"DBusPlugin", "primitiveDBusMessageGetSender", (void*)primitiveDBusMessageGetSender},
	{"DBusPlugin", "primitiveDBusArgumentGetBool", (void*)primitiveDBusArgumentGetBool},
	{"DBusPlugin", "primitiveDBusArgumentGetStruct", (void*)primitiveDBusArgumentGetStruct},
	{"DBusPlugin", "primitiveDBusConnectionRegisterSemaphore", (void*)primitiveDBusConnectionRegisterSemaphore},
	{"DBusPlugin", "primitiveDBusMessageGetDestination", (void*)primitiveDBusMessageGetDestination},
	{"DBusPlugin", "primitiveDBusNextIterator", (void*)primitiveDBusNextIterator},
	{"DBusPlugin", "primitiveDBusConnectionDispatchStatus", (void*)primitiveDBusConnectionDispatchStatus},
	{"DBusPlugin", "primitiveDBusArgumentGetInt32", (void*)primitiveDBusArgumentGetInt32},
	{"DBusPlugin", "primitiveDBusConnectionPopMessage", (void*)primitiveDBusConnectionPopMessage},
	{"DBusPlugin", "primitiveDBusArgumentGetType", (void*)primitiveDBusArgumentGetType},
	{"DBusPlugin", "primitiveDBusReleaseName", (void*)primitiveDBusReleaseName},
	{"DBusPlugin", "setInterpreter", (void*)setInterpreter},
	{"DBusPlugin", "primitiveDBusConnectToSessionBus", (void*)primitiveDBusConnectToSessionBus},
	{"DBusPlugin", "primitiveDBusPopIterator", (void*)primitiveDBusPopIterator},
	{"DBusPlugin", "primitiveDBusArgumentGetString", (void*)primitiveDBusArgumentGetString},
	{"DBusPlugin", "primitiveDBusReadWriteConnection", (void*)primitiveDBusReadWriteConnection},
	{"DBusPlugin", "primitiveDBusMessageGetInterface", (void*)primitiveDBusMessageGetInterface},
	{"DBusPlugin", "primitiveDBusConnectionDispatch", (void*)primitiveDBusConnectionDispatch},
	{"DBusPlugin", "primitiveDBusArgumentGetDictionary", (void*)primitiveDBusArgumentGetDictionary},
	{"DBusPlugin", "primitiveDBusMessageGetErrorName", (void*)primitiveDBusMessageGetErrorName},
	{"DBusPlugin", "shutdownModule", (void*)shutdownModule},
	{"DBusPlugin", "primitiveDBusSendMessage", (void*)primitiveDBusSendMessage},
	{"DBusPlugin", "primitiveDBusMessageGetReplySerial", (void*)primitiveDBusMessageGetReplySerial},
	{"DBusPlugin", "primitiveDBusArgumentGetDouble", (void*)primitiveDBusArgumentGetDouble},
	{"DBusPlugin", "primitiveDBusMessageGetSignature", (void*)primitiveDBusMessageGetSignature},
	{"DBusPlugin", "primitiveDBusMessageGetMember", (void*)primitiveDBusMessageGetMember},
	{"DBusPlugin", "initialiseModule", (void*)initialiseModule},
	{"DBusPlugin", "primitiveDBusAddMatch", (void*)primitiveDBusAddMatch},
	{"DBusPlugin", "primitiveDBusConnectToSystemBus", (void*)primitiveDBusConnectToSystemBus},
	{"DBusPlugin", "primitiveDBusMessageGetPath", (void*)primitiveDBusMessageGetPath},
	{"DBusPlugin", "primitiveDBusMessageGetNoReply", (void*)primitiveDBusMessageGetNoReply},
	{NULL, NULL, NULL}
};


#endif /* ifdef SQ_BUILTIN_PLUGIN */

