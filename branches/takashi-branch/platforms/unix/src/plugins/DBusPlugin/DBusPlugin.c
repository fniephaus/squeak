/* Automatically generated from Squeak on an Array(9 October 2007 7:00:22 pm) */

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
	typedef struct sqDBusData {
		DBusConnection* con;
		DBusWatch* watch;
		int semaphore;
	} sqDBusData;

#include "sqMemoryAccess.h"


/*** Constants ***/

/*** Function Prototypes ***/
static sqInt argumentsAddBooltoIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddBytetoIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddDoubletoIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddInt16toIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddInt32toIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddInt64toIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddObjectPathtoIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddSignaturetoIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddStringtoIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddUInt16toIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddUInt32toIter(sqInt oop, DBusMessageIter* iter);
static sqInt argumentsAddUInt64toIter(sqInt oop, DBusMessageIter* iter);
static sqInt buildStringOopFromCharP(const char* charP);
static DBusMessage * createErrorTofrom(char* dest, sqInt msgOop);
static DBusMessage * createReplyTofrom(char* dest, sqInt msgOop);
static char * fetchStringofObject(sqInt n, sqInt oop);
static sqInt getBasicTypevalue(int t, void* val);
static DBusConnection * getConnectionFromOop(sqInt oop);
static VirtualMachine * getInterpreter(void);
#pragma export on
EXPORT(const char*) getModuleName(void);
#pragma export off
static DBusConnection * getSessionBusConnection(void);
static DBusConnection * getSystemBusConnection(void);
static sqInt halt(void);
static sqInt handleReadForFDwithDataandFlag(int fd, sqDBusData* data, int flag);
static sqInt handleflag(int fd, int flag);
#pragma export on
EXPORT(sqInt) initialiseModule(void);
#pragma export off
static sqInt iterOpenContainercontains(sqInt t, char* s);
static sqInt msg(char * s);
static sqDBusData* newDataStructWithConnectionandSemaphore(DBusConnection * con, int semaphore);
#pragma export on
EXPORT(sqInt) primitiveDBusAddMatch(void);
EXPORT(sqInt) primitiveDBusAppendBasicArgument(void);
EXPORT(sqInt) primitiveDBusArgumentGetBool(void);
EXPORT(sqInt) primitiveDBusArgumentGetByte(void);
EXPORT(sqInt) primitiveDBusArgumentGetDouble(void);
EXPORT(sqInt) primitiveDBusArgumentGetInt16(void);
EXPORT(sqInt) primitiveDBusArgumentGetInt32(void);
EXPORT(sqInt) primitiveDBusArgumentGetInt64(void);
EXPORT(sqInt) primitiveDBusArgumentGetObjectPath(void);
EXPORT(sqInt) primitiveDBusArgumentGetSignature(void);
EXPORT(sqInt) primitiveDBusArgumentGetString(void);
EXPORT(sqInt) primitiveDBusArgumentGetType(void);
EXPORT(sqInt) primitiveDBusArgumentGetUInt16(void);
EXPORT(sqInt) primitiveDBusArgumentGetUInt32(void);
EXPORT(sqInt) primitiveDBusArgumentGetUInt64(void);
EXPORT(sqInt) primitiveDBusConnectionClose(void);
EXPORT(sqInt) primitiveDBusConnectionDispatchStatus(void);
EXPORT(sqInt) primitiveDBusConnectionPopMessage(void);
EXPORT(sqInt) primitiveDBusConnectionRegisterSemaphore(void);
EXPORT(sqInt) primitiveDBusConnectToSessionBus(void);
EXPORT(sqInt) primitiveDBusConnectToSystemBus(void);
EXPORT(sqInt) primitiveDBusCreateMessageFrom(void);
EXPORT(sqInt) primitiveDBusInitializeWriteIterator(void);
EXPORT(sqInt) primitiveDBusIteratorSignature(void);
EXPORT(sqInt) primitiveDBusIterCloseContainer(void);
EXPORT(sqInt) primitiveDBusIterOpenContainerContains(void);
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
EXPORT(sqInt) primitiveDBusMessageHasArguments(void);
EXPORT(sqInt) primitiveDBusNextIterator(void);
EXPORT(sqInt) primitiveDBusPopMessageIterator(void);
EXPORT(sqInt) primitiveDBusPushMessageIterator(void);
EXPORT(sqInt) primitiveDBusReadWriteConnection(void);
EXPORT(sqInt) primitiveDBusRegisterName(void);
EXPORT(sqInt) primitiveDBusReleaseName(void);
EXPORT(sqInt) primitiveDBusRemoveMatch(void);
EXPORT(sqInt) primitiveDBusSendMessage(void);
EXPORT(sqInt) primitiveDBusSendMessageWithReply(void);
EXPORT(sqInt) setInterpreter(struct VirtualMachine* anInterpreter);
EXPORT(sqInt) shutdownModule(void);
#pragma export off
static sqInt sqAssert(sqInt aBool);
static dbus_bool_t sqDBusPluginAddWatchwithData(DBusWatch* watch, void* data);
static void sqDBusPluginFreeWatchData(void* data);
static void sqDBusPluginRemoveWatchwithData(DBusWatch* watch, void* data);
static void sqDBusPluginToggleWatchwithData(DBusWatch* watch, void* data);
static char * stringOopToChar(sqInt oop);
/*** Variables ***/
static DBusConnection* connections[2];
static int curIter;
static int curWriteIter;
static dbus_bool_t hasArgument;

#ifdef SQUEAK_BUILTIN_PLUGIN
extern
#endif
struct VirtualMachine* interpreterProxy;
static DBusMessage* message;
static DBusMessageIter messageIter[DBUS_MAXIMUM_TYPE_RECURSION_DEPTH];
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"DBusPlugin 9 October 2007 (i)"
#else
	"DBusPlugin 9 October 2007 (e)"
#endif
;
static DBusMessage* writeMessage;
static DBusMessageIter writeMessageIter[DBUS_MAXIMUM_TYPE_RECURSION_DEPTH];


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

static sqInt argumentsAddBytetoIter(sqInt oop, DBusMessageIter* iter) {
	sqInt intArg;

	intArg = interpreterProxy->checkedIntegerValueOf(oop);
	if (interpreterProxy->failed()) {
		msg("Wrong oop type");
		return 0;
	}
	if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_BYTE, &intArg))) {
		msg("Can not add Byte argument.");
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

static sqInt argumentsAddInt16toIter(sqInt oop, DBusMessageIter* iter) {
	dbus_int16_t intArg;

	if (!((oop & 1))) {
		msg("Oop is no integer value");
		return interpreterProxy->primitiveFail();
	}
	intArg = (oop >> 1);
	if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_INT16, &intArg))) {
		msg("Can not add Int16 argument.");
		return interpreterProxy->primitiveFail();
	}
}

static sqInt argumentsAddInt32toIter(sqInt oop, DBusMessageIter* iter) {
	dbus_int32_t intArg;

	intArg = interpreterProxy->signed32BitValueOf(oop);
	if (interpreterProxy->failed()) {
		msg("Wrong oop type");
		return 0;
	}
	if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_INT32, &intArg))) {
		msg("Can not add Int32 argument.");
		return interpreterProxy->primitiveFail();
	}
}

static sqInt argumentsAddInt64toIter(sqInt oop, DBusMessageIter* iter) {
	dbus_int64_t intArg;

	intArg = interpreterProxy->signed64BitValueOf(oop);
	if (interpreterProxy->failed()) {
		msg("Wrong oop type");
		return 0;
	}
	if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_INT64, &intArg))) {
		msg("Can not add Int64 argument.");
		return interpreterProxy->primitiveFail();
	}
}

static sqInt argumentsAddObjectPathtoIter(sqInt oop, DBusMessageIter* iter) {
	char* arg;

	arg = stringOopToChar(oop);
	if (!(arg == null)) {
		if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_OBJECT_PATH, &arg))) {
			msg("Can not add Path argument.");
		}
		free(arg);
	}
}

static sqInt argumentsAddSignaturetoIter(sqInt oop, DBusMessageIter* iter) {
	char* arg;

	arg = stringOopToChar(oop);
	if (!(arg == null)) {
		if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_SIGNATURE, &arg))) {
			msg("Can not add Signature argument.");
		}
		free(arg);
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

static sqInt argumentsAddUInt16toIter(sqInt oop, DBusMessageIter* iter) {
	dbus_uint16_t intArg;

	if (!((oop & 1))) {
		msg("Oop is no integer value");
		return interpreterProxy->primitiveFail();
	}
	intArg = (oop >> 1);
	if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_UINT16, &intArg))) {
		msg("Can not add UInt16 argument.");
		return interpreterProxy->primitiveFail();
	}
}

static sqInt argumentsAddUInt32toIter(sqInt oop, DBusMessageIter* iter) {
	dbus_uint32_t intArg;

	intArg = interpreterProxy->positive32BitValueOf(oop);
	if (interpreterProxy->failed()) {
		msg("Wrong oop type");
		return 0;
	}
	if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_UINT32, &intArg))) {
		msg("Can not add UInt32 argument.");
		return interpreterProxy->primitiveFail();
	}
}

static sqInt argumentsAddUInt64toIter(sqInt oop, DBusMessageIter* iter) {
	dbus_uint64_t intArg;

	intArg = interpreterProxy->positive64BitValueOf(oop);
	if (interpreterProxy->failed()) {
		msg("Wrong oop type");
		return 0;
	}
	if (!(dbus_message_iter_append_basic(iter, DBUS_TYPE_UINT64, &intArg))) {
		msg("Can not add UInt64 argument.");
		return interpreterProxy->primitiveFail();
	}
}


/*	answers an string oop for the char pointer */

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


/*	creates an error message for a received message */

static DBusMessage * createErrorTofrom(char* dest, sqInt msgOop) {
	DBusMessage * error;
	dbus_uint32_t serial;
	char * name;
	sqInt serialOop;
	sqInt p;

	serialOop = interpreterProxy->fetchPointerofObject(7, msgOop);
	if (interpreterProxy->failed()) {
		return null;
	}

	/* extract the name of the error oop */

	serial = interpreterProxy->positive32BitValueOf(serialOop);
	/* begin fetchString:ofObject: */
	p = interpreterProxy->fetchPointerofObject(8, msgOop);
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


/*	answers a message which is send as reply for an previously received message */

static DBusMessage * createReplyTofrom(char* dest, sqInt msgOop) {
	dbus_uint32_t serial;
	sqInt serialOop;
	DBusMessage * reply;

	serialOop = interpreterProxy->fetchPointerofObject(7, msgOop);
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

static char * fetchStringofObject(sqInt n, sqInt oop) {
	sqInt p;

	p = interpreterProxy->fetchPointerofObject(n, oop);
	return stringOopToChar(p);
}

static sqInt getBasicTypevalue(int t, void* val) {
	int type;

	if (!(hasArgument)) {
		return interpreterProxy->primitiveFail();
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[curIter]);
	if (!(type == t)) {
		msg("Wrong argument type");
		return interpreterProxy->primitiveFail();
	}
	dbus_message_iter_get_basic(&messageIter[curIter], val );
}


/*	answers the corresponding dbus connection for the oop representation */

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


/*	anwers a connection to the session bus. The connection access is implementet as singleton so an open connection is reused */

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


/*	anwers a connection to the system bus. The connection access is implementet as singleton so an open connection is reused */

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


/*	a handler function which is called when new messages arrived in the dbus queue */

static sqInt handleReadForFDwithDataandFlag(int fd, sqDBusData* data, int flag) {
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


/*	opens a new container and increase writeIterator. arrays, structs, dictionarys and variants are container types in dbus */

static sqInt iterOpenContainercontains(sqInt t, char* s) {
	if (dbus_message_iter_open_container( &writeMessageIter[curWriteIter], t, s, &writeMessageIter[curWriteIter+1])) {
		curWriteIter += 1;
	} else {
		msg("Could not create container");
		return interpreterProxy->primitiveFail();
	}
	return interpreterProxy->trueObject();
}

static sqInt msg(char * s) {
	fprintf(stderr, "\n%s: %s", moduleName, s);
}


/*	answers an new data struct which is needed for handling watchers */

static sqDBusData* newDataStructWithConnectionandSemaphore(DBusConnection * con, int semaphore) {
	sqDBusData* t;

	t = null;
	t = (sqDBusData*)malloc(sizeof(sqDBusData));
	if (t == null) {
		msg("Cannot allocate memory for data structure");
		return null;
	}
	t->con = con; 
				t->semaphore = semaphore;
				t->watch = NULL;
	return t;
}


/*	Register a match rule */

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


/*	append a basic type argument to the currently topmost iterator in the iterator stack */

EXPORT(sqInt) primitiveDBusAppendBasicArgument(void) {
	DBusMessageIter* iter;
	sqInt arg;
	sqInt t;
	sqInt _return_value;

	arg = interpreterProxy->stackValue(1);
	t = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	iter = &(writeMessageIter[curWriteIter]);
	if (t == (DBUS_TYPE_BYTE)) {
		_return_value = argumentsAddBytetoIter(arg, iter);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(3, _return_value);
		return null;
	}
	if (t == (DBUS_TYPE_INT16)) {
		_return_value = argumentsAddInt16toIter(arg, iter);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(3, _return_value);
		return null;
	}
	if (t == (DBUS_TYPE_INT32)) {
		_return_value = argumentsAddInt32toIter(arg, iter);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(3, _return_value);
		return null;
	}
	if (t == (DBUS_TYPE_INT64)) {
		_return_value = argumentsAddInt64toIter(arg, iter);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(3, _return_value);
		return null;
	}
	if (t == (DBUS_TYPE_UINT16)) {
		_return_value = argumentsAddUInt16toIter(arg, iter);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(3, _return_value);
		return null;
	}
	if (t == (DBUS_TYPE_UINT32)) {
		_return_value = argumentsAddUInt32toIter(arg, iter);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(3, _return_value);
		return null;
	}
	if (t == (DBUS_TYPE_UINT64)) {
		_return_value = argumentsAddUInt64toIter(arg, iter);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(3, _return_value);
		return null;
	}
	if (t == (DBUS_TYPE_DOUBLE)) {
		_return_value = argumentsAddDoubletoIter(arg, iter);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(3, _return_value);
		return null;
	}
	if (t == (DBUS_TYPE_STRING)) {
		_return_value = argumentsAddStringtoIter(arg, iter);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(3, _return_value);
		return null;
	}
	if (t == (DBUS_TYPE_BOOLEAN)) {
		_return_value = argumentsAddBooltoIter(arg, iter);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(3, _return_value);
		return null;
	}
	if (t == (DBUS_TYPE_OBJECT_PATH)) {
		_return_value = argumentsAddObjectPathtoIter(arg, iter);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(3, _return_value);
		return null;
	}
	if (t == (DBUS_TYPE_SIGNATURE)) {
		_return_value = argumentsAddSignaturetoIter(arg, iter);
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(3, _return_value);
		return null;
	}
	msg("unknown argument type");
	interpreterProxy->primitiveFail();
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->pop(2);
	return null;
}


/*	returns a bool argument */

EXPORT(sqInt) primitiveDBusArgumentGetBool(void) {
	int value;
	int type;
	sqInt _return_value;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[curIter]);
	if (!(type == (DBUS_TYPE_BOOLEAN))) {
		msg("Wrong argument type: DBUS_TYPE_BOOLEAN.");
		interpreterProxy->primitiveFail();
		return null;
	}
	value = 0;
	dbus_message_iter_get_basic(&messageIter[curIter], &value );
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


/*	read a char argument and return a SmallIntegerOop */

EXPORT(sqInt) primitiveDBusArgumentGetByte(void) {
	char value;
	sqInt _return_value;

	value = 0;
	getBasicTypevalue(DBUS_TYPE_BYTE, &value);
	_return_value = interpreterProxy->integerObjectOf(value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	read a double argument and answer float oop */

EXPORT(sqInt) primitiveDBusArgumentGetDouble(void) {
	double value;
	int type;
	sqInt _return_value;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[curIter]);
	if (!(type == (DBUS_TYPE_DOUBLE))) {
		msg("Wrong argument type: DBUS_TYPE_DOUBLE.");
		interpreterProxy->primitiveFail();
		return null;
	}
	value = 0;
	dbus_message_iter_get_basic(&messageIter[curIter], &value );
	_return_value = interpreterProxy->floatObjectOf(value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	read a signed integer argument and return a SmallIntegerOop */

EXPORT(sqInt) primitiveDBusArgumentGetInt16(void) {
	dbus_int16_t value;
	sqInt _return_value;

	value = 0;
	getBasicTypevalue(DBUS_TYPE_INT16, &value);
	_return_value = interpreterProxy->integerObjectOf(value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	read a signed long argument and return a LargeIntegerOop depends on the sign */

EXPORT(sqInt) primitiveDBusArgumentGetInt32(void) {
	dbus_int32_t value;
	sqInt _return_value;

	value = 0;
	getBasicTypevalue(DBUS_TYPE_INT32, &value);
	_return_value = interpreterProxy->signed32BitIntegerFor(value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	read a signed long long argument and return a LargeIntegerOop depends on the sign */

EXPORT(sqInt) primitiveDBusArgumentGetInt64(void) {
	dbus_int64_t value;
	sqInt _return_value;

	value = 0;
	getBasicTypevalue(DBUS_TYPE_INT64, &value);
	_return_value = interpreterProxy->signed64BitIntegerFor(value);
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
	type = dbus_message_iter_get_arg_type(&messageIter[curIter]);
	if (!(type == (DBUS_TYPE_OBJECT_PATH))) {
		msg("Wrong argument type: DBUS_TYPE_OBJECT_PATH.");
		interpreterProxy->primitiveFail();
		return null;
	}
	str = "";
	dbus_message_iter_get_basic(&messageIter[curIter], &str );
	_return_value = buildStringOopFromCharP(str);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	read a signature from the arguments */

EXPORT(sqInt) primitiveDBusArgumentGetSignature(void) {
	char* value;
	sqInt _return_value;

	value = 0;
	getBasicTypevalue(DBUS_TYPE_SIGNATURE, &value);
	_return_value = buildStringOopFromCharP(value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	returns the readed string argument as oop */

EXPORT(sqInt) primitiveDBusArgumentGetString(void) {
	int type;
	char * str;
	sqInt _return_value;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[curIter]);
	if (!(type == (DBUS_TYPE_STRING))) {
		msg("Wrong argument type: DBUS_TYPE_STRING.");
		interpreterProxy->primitiveFail();
		return null;
	}
	str = "";
	dbus_message_iter_get_basic(&messageIter[curIter], &str );
	_return_value = buildStringOopFromCharP(str);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
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
	type = dbus_message_iter_get_arg_type(&messageIter[curIter]);
	_return_value = interpreterProxy->integerObjectOf(type);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	read a unsigned int argument and return a SmallIntegerOop */

EXPORT(sqInt) primitiveDBusArgumentGetUInt16(void) {
	dbus_uint16_t value;
	sqInt _return_value;

	value = 0;
	getBasicTypevalue(DBUS_TYPE_UINT16, &value);
	_return_value = interpreterProxy->integerObjectOf(value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	read a unsigned long argument and returns a LargeInteger oop */

EXPORT(sqInt) primitiveDBusArgumentGetUInt32(void) {
	dbus_uint32_t value;
	sqInt _return_value;

	value = 0;
	getBasicTypevalue(DBUS_TYPE_UINT32, &value);
	_return_value = interpreterProxy->positive32BitIntegerFor(value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	read a unsigned long long argument and returns a LargeInteger oop */

EXPORT(sqInt) primitiveDBusArgumentGetUInt64(void) {
	long long value;
	sqInt _return_value;

	value = 0;
	getBasicTypevalue(DBUS_TYPE_UINT64, &value);
	_return_value = interpreterProxy->positive64BitIntegerFor(value);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	close the recivers connection */

EXPORT(sqInt) primitiveDBusConnectionClose(void) {
	sqInt h;
	sqInt rcvOop;

	rcvOop = interpreterProxy->stackValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	h = interpreterProxy->fetchIntegerofObject(0, rcvOop);
	dbus_connection_close(connections[h]);
				dbus_connection_unref(connections[h]);
	connections[h] = null;
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
		curIter = 0;
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
	hasArgument = dbus_message_iter_init(message, &(messageIter[curIter]));
	_return_value = interpreterProxy->integerObjectOf(msgType);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	Registers watch functions for which will called when dbus want to register/remove a watch. The semaphore will signaled when dbus receives new messages */

EXPORT(sqInt) primitiveDBusConnectionRegisterSemaphore(void) {
	DBusConnection* conn;
	sqInt rcvOop;
	sqDBusData* data;
	sqInt semaphore;

	semaphore = interpreterProxy->stackIntegerValue(0);
	rcvOop = interpreterProxy->stackValue(1);
	if (interpreterProxy->failed()) {
		return null;
	}

	/* create a new data structure */

	conn = getConnectionFromOop(rcvOop);
	data = newDataStructWithConnectionandSemaphore(conn, semaphore);
	if (data == null) {
		interpreterProxy->primitiveFail();
		return null;
	}
	if (!(dbus_connection_set_watch_functions(conn,
		sqDBusPluginAddWatchwithData,
		sqDBusPluginRemoveWatchwithData,
		sqDBusPluginToggleWatchwithData, 
		data,
		sqDBusPluginFreeWatchData))) {
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


/*	create a dbus message from the given oop */

EXPORT(sqInt) primitiveDBusCreateMessageFrom(void) {
	char * method;
	int msgType;
	char * path;
	char * destination;
	DBusMessage * tmpMsg;
	char * interface;
	sqInt m;
	sqInt _return_value;
	sqInt p;
	sqInt p1;
	sqInt p2;
	sqInt p3;

	m = interpreterProxy->stackValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	msgType = interpreterProxy->fetchIntegerofObject(0, m);
	if (interpreterProxy->failed()) {
		msg("Can not fetch message type.");
		interpreterProxy->primitiveFail();
		return null;
	}
	/* begin fetchString:ofObject: */
	p = interpreterProxy->fetchPointerofObject(1, m);
	path = stringOopToChar(p);
	/* begin fetchString:ofObject: */
	p1 = interpreterProxy->fetchPointerofObject(2, m);
	interface = stringOopToChar(p1);
	/* begin fetchString:ofObject: */
	p2 = interpreterProxy->fetchPointerofObject(3, m);
	method = stringOopToChar(p2);
	/* begin fetchString:ofObject: */
	p3 = interpreterProxy->fetchPointerofObject(5, m);
	destination = stringOopToChar(p3);

	/* create Signal */

	tmpMsg = null;
	if (msgType == 4) {
		tmpMsg = dbus_message_new_signal(path, interface, method);
	}
	if (msgType == 1) {
		tmpMsg = dbus_message_new_method_call(destination, path, interface, method);
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

	/* cleaning */

	writeMessage = tmpMsg;
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
	_return_value = interpreterProxy->trueObject();
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(2, _return_value);
	return null;
}


/*	initialize an interator to write message arguments */

EXPORT(sqInt) primitiveDBusInitializeWriteIterator(void) {
	if (writeMessage == null) {
		null;
	} else {
		curWriteIter = 0;
		dbus_message_iter_init_append(writeMessage, &writeMessageIter[curWriteIter]);
	}
	if (interpreterProxy->failed()) {
		return null;
	}
	return null;
}


/*	answer the signature of the current iterator  */

EXPORT(sqInt) primitiveDBusIteratorSignature(void) {
	char* sig;
	sqInt strOop;

	sig = dbus_message_iter_get_signature(&messageIter[curIter]);
	strOop = buildStringOopFromCharP(sig);
	dbus_free(sig);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, strOop);
	return null;
}


/*	pops the topmost container from the iterator stack */

EXPORT(sqInt) primitiveDBusIterCloseContainer(void) {
	if (curWriteIter == 0) {
		msg("No open container available");
		interpreterProxy->primitiveFail();
		return null;
	}
	if (dbus_message_iter_close_container( &writeMessageIter[curWriteIter-1], &writeMessageIter[curWriteIter])) {
		curWriteIter -= 1;
	} else {
		msg("Could not close container");
		interpreterProxy->primitiveFail();
		return null;
	}
	if (interpreterProxy->failed()) {
		return null;
	}
	return null;
}


/*	push a new container to the iterator stack. s is the signature which describes the content of the container */

EXPORT(sqInt) primitiveDBusIterOpenContainerContains(void) {
	sqInt openContainer;
	char* cSig;
	sqInt t;
	sqInt s;
	sqInt _return_value;

	t = interpreterProxy->stackIntegerValue(1);
	s = interpreterProxy->stackValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	cSig = null;
	if (!(s == null)) {
		cSig = stringOopToChar(s);
		if (cSig == null) {
			if (interpreterProxy->failed()) {
				return null;
			}
			interpreterProxy->popthenPush(3, 0);
			return null;
		}
	}
	openContainer = 0;
	if (t == (DBUS_TYPE_ARRAY)) {
		openContainer = 1;
		if (s == null) {
			msg("To write an array you have to specify the signature of the contained elements");
			interpreterProxy->primitiveFail();
			return null;
		}
	}
	if (t == (DBUS_TYPE_VARIANT)) {
		openContainer = 1;
		if (s == null) {
			msg("To write a variant you have to specify the signature of the contained element");
			interpreterProxy->primitiveFail();
			return null;
		}
	}
	if (t == (DBUS_TYPE_STRUCT)) {
		openContainer = 1;
	}
	if (t == (DBUS_TYPE_DICT_ENTRY)) {
		openContainer = 1;
	}
	if (!(openContainer)) {
		fprintf(stderr, "\n%d is no container type", t);
		interpreterProxy->primitiveFail();
		return null;
	}
	_return_value = iterOpenContainercontains(t, cSig);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(3, _return_value);
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


/*	answer the name name of the received error (ERROR only) */

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


/*	returns true if the received message has arguments */

EXPORT(sqInt) primitiveDBusMessageHasArguments(void) {
	sqInt _return_value;

	if (!(hasArgument)) {
		_return_value = interpreterProxy->falseObject();
		if (interpreterProxy->failed()) {
			return null;
		}
		interpreterProxy->popthenPush(1, _return_value);
		return null;
	}
	_return_value = interpreterProxy->trueObject();
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	set the next message iterator as the current. anwers if there is a next iterator  */

EXPORT(sqInt) primitiveDBusNextIterator(void) {
	sqInt _return_value;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	hasArgument = dbus_message_iter_next(&messageIter[curIter]);
	_return_value = (hasArgument? interpreterProxy->trueObject(): interpreterProxy->falseObject());
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	pops a message iterator from the iterator stack */

EXPORT(sqInt) primitiveDBusPopMessageIterator(void) {
	sqInt _return_value;

	if (curIter > 0) {
		curIter -= 1;
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


/*	push an iterator for reading variants, arrays or structs */

EXPORT(sqInt) primitiveDBusPushMessageIterator(void) {
	int type;

	if (!(hasArgument)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	type = 0;
	type = dbus_message_iter_get_arg_type(&messageIter[curIter]);
	if (!(type == (DBUS_TYPE_VARIANT))) {
		if (!(type == (DBUS_TYPE_ARRAY))) {
			if (!(type == (DBUS_TYPE_DICT_ENTRY))) {
				if (!(type == (DBUS_TYPE_STRUCT))) {
					fprintf(stderr, "Wrong argument type: %s", dbus_message_iter_get_signature(&messageIter[curIter]) );
					interpreterProxy->primitiveFail();
					return null;
				}
			}
		}
	}
	dbus_message_iter_recurse(&messageIter[curIter], &messageIter[curIter+1]);
	curIter += 1;
	hasArgument = 1;
	if (interpreterProxy->failed()) {
		return null;
	}
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
		curIter = 0;
		dbus_message_unref(message);
	}
	dbus_connection_read_write(conn, 1); 
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
	hasArgument = dbus_message_iter_init(message, &(messageIter[curIter]));
	_return_value = interpreterProxy->integerObjectOf(msgType);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	Register a name at the connection */

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


/*	Release a registered name */

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


/*	send the prevoisly created message and returns the serial of the message */

EXPORT(sqInt) primitiveDBusSendMessage(void) {
	DBusConnection * conn;
	sqInt msgSend;
	sqInt rcvOop;
	dbus_uint32_t serial;
	sqInt _return_value;

	rcvOop = interpreterProxy->stackValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	if (writeMessage == null) {
		interpreterProxy->primitiveFail();
		return null;
	}
	conn = getConnectionFromOop(rcvOop);
	serial = 0;
	msgSend = 1;
	if (dbus_connection_send(conn, writeMessage, &serial)) {
		dbus_connection_flush(conn);
	} else {
		msg("message was not sended");
		msgSend = 0;
	}
	dbus_message_unref(writeMessage);
	if (!(msgSend)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	_return_value = interpreterProxy->integerObjectOf(serial);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
	return null;
}


/*	send the previously created message with reply and returns the serial of the message */

EXPORT(sqInt) primitiveDBusSendMessageWithReply(void) {
	DBusConnection * conn;
	sqInt msgSend;
	sqInt rcvOop;
	dbus_uint32_t serial;
	sqInt _return_value;

	rcvOop = interpreterProxy->stackValue(0);
	if (interpreterProxy->failed()) {
		return null;
	}
	if (writeMessage == null) {
		interpreterProxy->primitiveFail();
		return null;
	}
	conn = getConnectionFromOop(rcvOop);
	serial = 0;
	msgSend = 1;
	if (dbus_connection_send_with_reply(conn, writeMessage, NULL, -1)) {
		serial = dbus_message_get_serial(writeMessage);
		dbus_connection_flush(conn);
	} else {
		msg("message was not sended");
		msgSend = 0;
	}
	dbus_message_unref(writeMessage);
	if (!(msgSend)) {
		interpreterProxy->primitiveFail();
		return null;
	}
	_return_value = interpreterProxy->integerObjectOf(serial);
	if (interpreterProxy->failed()) {
		return null;
	}
	interpreterProxy->popthenPush(1, _return_value);
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
			connections[i] = null;
		}
	}
	return 1;
}

static sqInt sqAssert(sqInt aBool) {
	/* missing DebugCode */;
}


/*	is called when the dbus want to add a watch handle. a filedescriptor for the watch will registered in the squeak loop */

static dbus_bool_t sqDBusPluginAddWatchwithData(DBusWatch* watch, void* data) {
	sqInt flag;
	sqInt fd;


	/* get the file descriptor of the watch */

	flag = dbus_watch_get_flags(watch);

	/* set watch in data structure */

	fd = dbus_watch_get_fd(watch);
	((sqDBusData*)data)->watch = watch;
	if (flag & DBUS_WATCH_READABLE) {
		aioEnable(fd, data, NULL);
							aioHandle(fd, handleReadForFDwithDataandFlag , 1<<0 | 1<<1 | 1<<3);
	}
	return 1;
}


/*	function is called after a watch is dissabled an the data of the watch can be freed */

static void sqDBusPluginFreeWatchData(void* data) {
	free(data);
}


/*	is called when the dbus want to remove a watch handle, the filedescriptor will disabled */

static void sqDBusPluginRemoveWatchwithData(DBusWatch* watch, void* data) {
	sqInt fd;

	fd = dbus_watch_get_fd(watch);
	aioDisable(fd);
}


/*	toggle the filedescriptor */

static void sqDBusPluginToggleWatchwithData(DBusWatch* watch, void* data) {
	sqInt enable;
	sqInt fd;

	enable = dbus_watch_get_enabled(watch);
	fd = dbus_watch_get_fd(watch);
	if (enable) {
		((sqDBusData*)data)->watch = watch;
		aioEnable(fd, data, NULL);
	} else {
		aioDisable(fd);
	}
}


/*	convert a string oop to an char pointer which stores the content of the oop */

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
	{"DBusPlugin", "primitiveDBusPopMessageIterator", (void*)primitiveDBusPopMessageIterator},
	{"DBusPlugin", "primitiveDBusRemoveMatch", (void*)primitiveDBusRemoveMatch},
	{"DBusPlugin", "getModuleName", (void*)getModuleName},
	{"DBusPlugin", "primitiveDBusMessageGetSerial", (void*)primitiveDBusMessageGetSerial},
	{"DBusPlugin", "primitiveDBusCreateMessageFrom", (void*)primitiveDBusCreateMessageFrom},
	{"DBusPlugin", "primitiveDBusArgumentGetObjectPath", (void*)primitiveDBusArgumentGetObjectPath},
	{"DBusPlugin", "primitiveDBusInitializeWriteIterator", (void*)primitiveDBusInitializeWriteIterator},
	{"DBusPlugin", "primitiveDBusArgumentGetByte", (void*)primitiveDBusArgumentGetByte},
	{"DBusPlugin", "primitiveDBusNextIterator", (void*)primitiveDBusNextIterator},
	{"DBusPlugin", "primitiveDBusArgumentGetInt32", (void*)primitiveDBusArgumentGetInt32},
	{"DBusPlugin", "primitiveDBusConnectionPopMessage", (void*)primitiveDBusConnectionPopMessage},
	{"DBusPlugin", "primitiveDBusArgumentGetType", (void*)primitiveDBusArgumentGetType},
	{"DBusPlugin", "primitiveDBusArgumentGetUInt16", (void*)primitiveDBusArgumentGetUInt16},
	{"DBusPlugin", "primitiveDBusConnectToSessionBus", (void*)primitiveDBusConnectToSessionBus},
	{"DBusPlugin", "primitiveDBusIteratorSignature", (void*)primitiveDBusIteratorSignature},
	{"DBusPlugin", "primitiveDBusMessageHasArguments", (void*)primitiveDBusMessageHasArguments},
	{"DBusPlugin", "primitiveDBusArgumentGetSignature", (void*)primitiveDBusArgumentGetSignature},
	{"DBusPlugin", "primitiveDBusArgumentGetInt64", (void*)primitiveDBusArgumentGetInt64},
	{"DBusPlugin", "primitiveDBusAppendBasicArgument", (void*)primitiveDBusAppendBasicArgument},
	{"DBusPlugin", "primitiveDBusMessageGetErrorName", (void*)primitiveDBusMessageGetErrorName},
	{"DBusPlugin", "shutdownModule", (void*)shutdownModule},
	{"DBusPlugin", "primitiveDBusMessageGetSignature", (void*)primitiveDBusMessageGetSignature},
	{"DBusPlugin", "primitiveDBusMessageGetMember", (void*)primitiveDBusMessageGetMember},
	{"DBusPlugin", "primitiveDBusSendMessage", (void*)primitiveDBusSendMessage},
	{"DBusPlugin", "primitiveDBusConnectToSystemBus", (void*)primitiveDBusConnectToSystemBus},
	{"DBusPlugin", "primitiveDBusArgumentGetUInt32", (void*)primitiveDBusArgumentGetUInt32},
	{"DBusPlugin", "primitiveDBusMessageGetPath", (void*)primitiveDBusMessageGetPath},
	{"DBusPlugin", "primitiveDBusSendMessageWithReply", (void*)primitiveDBusSendMessageWithReply},
	{"DBusPlugin", "primitiveDBusRegisterName", (void*)primitiveDBusRegisterName},
	{"DBusPlugin", "primitiveDBusMessageGetSender", (void*)primitiveDBusMessageGetSender},
	{"DBusPlugin", "primitiveDBusArgumentGetBool", (void*)primitiveDBusArgumentGetBool},
	{"DBusPlugin", "primitiveDBusPushMessageIterator", (void*)primitiveDBusPushMessageIterator},
	{"DBusPlugin", "primitiveDBusConnectionRegisterSemaphore", (void*)primitiveDBusConnectionRegisterSemaphore},
	{"DBusPlugin", "primitiveDBusArgumentGetUInt64", (void*)primitiveDBusArgumentGetUInt64},
	{"DBusPlugin", "primitiveDBusMessageGetDestination", (void*)primitiveDBusMessageGetDestination},
	{"DBusPlugin", "primitiveDBusConnectionClose", (void*)primitiveDBusConnectionClose},
	{"DBusPlugin", "primitiveDBusConnectionDispatchStatus", (void*)primitiveDBusConnectionDispatchStatus},
	{"DBusPlugin", "primitiveDBusIterCloseContainer", (void*)primitiveDBusIterCloseContainer},
	{"DBusPlugin", "primitiveDBusReleaseName", (void*)primitiveDBusReleaseName},
	{"DBusPlugin", "setInterpreter", (void*)setInterpreter},
	{"DBusPlugin", "primitiveDBusArgumentGetString", (void*)primitiveDBusArgumentGetString},
	{"DBusPlugin", "primitiveDBusReadWriteConnection", (void*)primitiveDBusReadWriteConnection},
	{"DBusPlugin", "primitiveDBusIterOpenContainerContains", (void*)primitiveDBusIterOpenContainerContains},
	{"DBusPlugin", "primitiveDBusMessageGetInterface", (void*)primitiveDBusMessageGetInterface},
	{"DBusPlugin", "primitiveDBusMessageGetReplySerial", (void*)primitiveDBusMessageGetReplySerial},
	{"DBusPlugin", "primitiveDBusArgumentGetDouble", (void*)primitiveDBusArgumentGetDouble},
	{"DBusPlugin", "initialiseModule", (void*)initialiseModule},
	{"DBusPlugin", "primitiveDBusAddMatch", (void*)primitiveDBusAddMatch},
	{"DBusPlugin", "primitiveDBusArgumentGetInt16", (void*)primitiveDBusArgumentGetInt16},
	{"DBusPlugin", "primitiveDBusMessageGetNoReply", (void*)primitiveDBusMessageGetNoReply},
	{NULL, NULL, NULL}
};


#endif /* ifdef SQ_BUILTIN_PLUGIN */
