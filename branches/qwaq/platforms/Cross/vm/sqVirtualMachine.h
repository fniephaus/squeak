#ifndef _SqueakVM_H
#define _SqueakVM_H

/* Increment the following number if you change the order of
   functions listed or if you remove functions */

#define VM_PROXY_MAJOR 1

/* Note: You can define a different VM_PROXY_MINOR if the plugin
   should work with older VMs. */
#ifndef VM_PROXY_MINOR
/* Increment the following number if you add functions at the end */
#define VM_PROXY_MINOR 7
#endif


#include "sqMemoryAccess.h"


/* The following file contains all function prototypes exported by VM , which can be used outside
   We need sqLong to be defined before including it. */

#if VM_PROXY_MINOR > 4

#  if !defined(sqLong)
#   if _MSC_VER
#     define sqLong __int64
#   else
#     define sqLong long long
#   endif
#  endif

#endif

/* The function prototypes, exported by VM */

typedef sqInt (*CompilerHook)();

struct VirtualMachine* sqGetInterpreterProxy(void);


/******************** VirtualMachine for legacy plugins **********************************/

struct VirtualMachine* sqGetInterpreterProxy(void);

typedef struct VirtualMachine {
	sqInt (*minorVersion)(void);
	sqInt (*majorVersion)(void);

	/* InterpreterProxy methodsFor: 'stack access' */

	sqInt  (*pop)(sqInt nItems);
	sqInt  (*popthenPush)(sqInt nItems, sqInt oop);
	sqInt  (*push)(sqInt object);
	sqInt  (*pushBool)(sqInt trueOrFalse);
	sqInt  (*pushFloat)(double f);
	sqInt  (*pushInteger)(sqInt integerValue);
	double (*stackFloatValue)(sqInt offset);
	sqInt  (*stackIntegerValue)(sqInt offset);
	sqInt  (*stackObjectValue)(sqInt offset);
	sqInt  (*stackValue)(sqInt offset);

	/* InterpreterProxy methodsFor: 'object access' */

	sqInt  (*argumentCountOf)(sqInt methodPointer);
	void  *(*arrayValueOf)(sqInt oop);
	sqInt  (*byteSizeOf)(sqInt oop);
	void  *(*fetchArrayofObject)(sqInt fieldIndex, sqInt objectPointer);
	sqInt  (*fetchClassOf)(sqInt oop);
	double (*fetchFloatofObject)(sqInt fieldIndex, sqInt objectPointer);
	sqInt  (*fetchIntegerofObject)(sqInt fieldIndex, sqInt objectPointer);
	sqInt  (*fetchPointerofObject)(sqInt fieldIndex, sqInt oop);
/*  sqInt  (*fetchWordofObject)(sqInt fieldFieldIndex, sqInt oop); *
 * has been rescinded as of VMMaker 3.8 and the 64bitclean VM      *
 * work. To support old plugins we keep a valid function in        *
 * the same location in the VM struct but rename it to             *
 * something utterly horrible to scare off the natives. A new      *
 * equivalent but 64 bit valid function is added as                *
 * 'fetchLong32OfObject'                                           */
	sqInt  (*obsoleteDontUseThisFetchWordofObject)(sqInt fieldFieldIndex, sqInt oop);
	void  *(*firstFixedField)(sqInt oop);
	void  *(*firstIndexableField)(sqInt oop);
	sqInt  (*literalofMethod)(sqInt offset, sqInt methodPointer);
	sqInt  (*literalCountOf)(sqInt methodPointer);
	sqInt  (*methodArgumentCount)(void);
	sqInt  (*methodPrimitiveIndex)(void);
	sqInt  (*primitiveIndexOf)(sqInt methodPointer);
	sqInt  (*sizeOfSTArrayFromCPrimitive)(void *cPtr);
	sqInt  (*slotSizeOf)(sqInt oop);
	sqInt  (*stObjectat)(sqInt array, sqInt fieldIndex);
	sqInt  (*stObjectatput)(sqInt array, sqInt fieldIndex, sqInt value);
	sqInt  (*stSizeOf)(sqInt oop);
	sqInt  (*storeIntegerofObjectwithValue)(sqInt fieldIndex, sqInt oop, sqInt integer);
	sqInt  (*storePointerofObjectwithValue)(sqInt fieldIndex, sqInt oop, sqInt valuePointer);

	/* InterpreterProxy methodsFor: 'testing' */

	sqInt (*isKindOf)(sqInt oop, char *aString);
	sqInt (*isMemberOf)(sqInt oop, char *aString);
	sqInt (*isBytes)(sqInt oop);
	sqInt (*isFloatObject)(sqInt oop);
	sqInt (*isIndexable)(sqInt oop);
	sqInt (*isIntegerObject)(sqInt objectPointer);
	sqInt (*isIntegerValue)(sqInt intValue);
	sqInt (*isPointers)(sqInt oop);
	sqInt (*isWeak)(sqInt oop);
	sqInt (*isWords)(sqInt oop);
	sqInt (*isWordsOrBytes)(sqInt oop);

	/* InterpreterProxy methodsFor: 'converting' */

	sqInt  (*booleanValueOf)(sqInt obj);
	sqInt  (*checkedIntegerValueOf)(sqInt intOop);
	sqInt  (*floatObjectOf)(double aFloat);
	double (*floatValueOf)(sqInt oop);
	sqInt  (*integerObjectOf)(sqInt value);
	sqInt  (*integerValueOf)(sqInt oop);
	sqInt  (*positive32BitIntegerFor)(sqInt integerValue);
	sqInt  (*positive32BitValueOf)(sqInt oop);

	/* InterpreterProxy methodsFor: 'special objects' */

	sqInt (*characterTable)(void);
	sqInt (*displayObject)(void);
	sqInt (*falseObject)(void);
	sqInt (*nilObject)(void);
	sqInt (*trueObject)(void);

	/* InterpreterProxy methodsFor: 'special classes' */

	sqInt (*classArray)(void);
	sqInt (*classBitmap)(void);
	sqInt (*classByteArray)(void);
	sqInt (*classCharacter)(void);
	sqInt (*classFloat)(void);
	sqInt (*classLargePositiveInteger)(void);
	sqInt (*classPoint)(void);
	sqInt (*classSemaphore)(void);
	sqInt (*classSmallInteger)(void);
	sqInt (*classString)(void);

	/* InterpreterProxy methodsFor: 'instance creation' */

	sqInt (*clone)(sqInt oop);
	sqInt (*instantiateClassindexableSize)(sqInt classPointer, sqInt size);
	sqInt (*makePointwithxValueyValue)(sqInt xValue, sqInt yValue);
	sqInt (*popRemappableOop)(void);
	sqInt (*pushRemappableOop)(sqInt oop);

	/* InterpreterProxy methodsFor: 'other' */

	sqInt (*becomewith)(sqInt array1, sqInt array2);
	sqInt (*byteSwapped)(sqInt w);
	sqInt (*failed)(void);
	sqInt (*fullDisplayUpdate)(void);
	sqInt (*fullGC)(void);
	sqInt (*incrementalGC)(void);
	sqInt (*primitiveFail)(void);
	sqInt (*showDisplayBitsLeftTopRightBottom)(sqInt aForm, sqInt l, sqInt t, sqInt r, sqInt b);
	sqInt (*signalSemaphoreWithIndex)(sqInt semaIndex);
	sqInt (*success)(sqInt aBoolean);
	sqInt (*superclassOf)(sqInt classPointer);

	/* InterpreterProxy methodsFor: 'compiler' */

	CompilerHook *(*compilerHookVector)(void);
	sqInt          (*setCompilerInitialized)(sqInt initFlag);

#if VM_PROXY_MINOR > 1

	/* InterpreterProxy methodsFor: 'BitBlt support' */

	sqInt (*loadBitBltFrom)(sqInt bbOop);
	sqInt (*copyBits)(void);
	sqInt (*copyBitsFromtoat)(sqInt leftX, sqInt rightX, sqInt yValue);

#endif

#if VM_PROXY_MINOR > 2

	sqInt (*classLargeNegativeInteger)(void);
	sqInt (*signed32BitIntegerFor)(sqInt integerValue);
	sqInt (*signed32BitValueOf)(sqInt oop);
	sqInt (*includesBehaviorThatOf)(sqInt aClass, sqInt aSuperClass);
	sqInt (*primitiveMethod)(void);

	/* InterpreterProxy methodsFor: 'FFI support' */

	sqInt (*classExternalAddress)(void);
	sqInt (*classExternalData)(void);
	sqInt (*classExternalFunction)(void);
	sqInt (*classExternalLibrary)(void);
	sqInt (*classExternalStructure)(void);
	void* (*ioLoadModuleOfLength)(sqInt modIndex, sqInt modLength);
	void* (*ioLoadSymbolOfLengthFromModule)(sqInt fnIndex, sqInt fnLength, void* handle);
	sqInt (*isInMemory)(sqInt address);

#endif

#if VM_PROXY_MINOR > 3

	void* (*ioLoadFunctionFrom)(char *fnName, char *modName);
	sqInt (*ioMicroMSecs)(void);

#endif

#if VM_PROXY_MINOR > 4

#  if !defined(sqLong)
#   if _MSC_VER
#     define sqLong __int64
#   else
#     define sqLong long long
#   endif
#  endif

	sqInt  (*positive64BitIntegerFor)(sqLong integerValue);
	sqLong (*positive64BitValueOf)(sqInt oop);
	sqInt  (*signed64BitIntegerFor)(sqLong integerValue);
	sqLong (*signed64BitValueOf)(sqInt oop);

#endif

#if VM_PROXY_MINOR > 5
	sqInt (*isArray)(sqInt oop);
	sqInt (*forceInterruptCheck)(void);
#endif

#if VM_PROXY_MINOR > 6
	sqInt  (*fetchLong32ofObject)(sqInt fieldFieldIndex, sqInt oop);
	sqInt  (*getThisSessionID)(void);
	sqInt	  (*ioFilenamefromStringofLengthresolveAliases)(char* aCharBuffer, char* filenameIndex, sqInt filenameLength, sqInt resolveFlag);
	sqInt  (*vmEndianness)(void);	
#endif


} VirtualMachine;



/******************** ObjVirtualMachine for objectified plugins **********************************/

#ifdef VM_OBJECTIFIED

#define OBJVM_PROXY_MAJOR 2
#ifndef OBJVM_PROXY_MINOR
/* Increment the following number if you add functions at the end */
#define OBJVM_PROXY_MINOR 1
#endif

struct ObjVirtualMachine * sqGetObjInterpreterProxy(void);

typedef struct ObjVirtualMachine {
	sqInt (*minorVersion)(void);
	sqInt (*majorVersion)(void);
	/* InterpreterProxy methodsFor: 'stack access' */

	sqInt  (*pop)(INTERPRETER_ARG_COMMA sqInt nItems);
	sqInt  (*popthenPush)(INTERPRETER_ARG_COMMA sqInt nItems, sqInt oop);
	sqInt  (*push)(INTERPRETER_ARG_COMMA sqInt object);
	sqInt  (*pushBool)(INTERPRETER_ARG_COMMA sqInt trueOrFalse);
	sqInt  (*pushFloat)(INTERPRETER_ARG_COMMA double f);
	sqInt  (*pushInteger)(INTERPRETER_ARG_COMMA sqInt integerValue);
	double (*stackFloatValue)(INTERPRETER_ARG_COMMA sqInt offset);
	sqInt  (*stackIntegerValue)(INTERPRETER_ARG_COMMA sqInt offset);
	sqInt  (*stackObjectValue)(INTERPRETER_ARG_COMMA sqInt offset);
	sqInt  (*stackValue)(INTERPRETER_ARG_COMMA sqInt offset);

	/* InterpreterProxy methodsFor: 'object access' */

	sqInt  (*argumentCountOf)(sqInt methodPointer);
	void  *(*arrayValueOf)(sqInt oop);
	sqInt  (*byteSizeOf)(sqInt oop);
	void  *(*fetchArrayofObject)(INTERPRETER_ARG_COMMA sqInt fieldIndex, sqInt objectPointer);
	sqInt  (*fetchClassOf)(INTERPRETER_ARG_COMMA sqInt oop);
	double (*fetchFloatofObject)(INTERPRETER_ARG_COMMA sqInt fieldIndex, sqInt objectPointer);
	sqInt  (*fetchIntegerofObject)(INTERPRETER_ARG_COMMA sqInt fieldIndex, sqInt objectPointer);
	sqInt  (*fetchPointerofObject)(sqInt fieldIndex, sqInt oop);
/*  sqInt  (*fetchWordofObject)(sqInt fieldFieldIndex, sqInt oop); *
 * has been rescinded as of VMMaker 3.8 and the 64bitclean VM      *
 * work. To support old plugins we keep a valid function in        *
 * the same location in the VM struct but rename it to             *
 * something utterly horrible to scare off the natives. A new      *
 * equivalent but 64 bit valid function is added as                *
 * 'fetchLong32OfObject'                                           */
/*	sqInt  (*obsoleteDontUseThisFetchWordofObject)(sqInt fieldFieldIndex, sqInt oop);  */


	void  *(*firstFixedField)(sqInt oop);
	void  *(*firstIndexableField)(INTERPRETER_ARG_COMMA sqInt oop);
	sqInt  (*literalofMethod)(sqInt offset, sqInt methodPointer);
	sqInt  (*literalCountOf)(sqInt methodPointer);
	sqInt  (*methodArgumentCount)(INTERPRETER_ARG);
	sqInt  (*methodPrimitiveIndex)(INTERPRETER_ARG);
	sqInt  (*primitiveIndexOf)(sqInt methodPointer);
	sqInt  (*sizeOfSTArrayFromCPrimitive)(INTERPRETER_ARG_COMMA void *cPtr);
	sqInt  (*slotSizeOf)(sqInt oop);
	sqInt  (*stObjectat)(INTERPRETER_ARG_COMMA sqInt array, sqInt fieldIndex);
	sqInt  (*stObjectatput)(INTERPRETER_ARG_COMMA sqInt array, sqInt fieldIndex, sqInt value);
	sqInt  (*stSizeOf)(INTERPRETER_ARG_COMMA sqInt oop);
	sqInt  (*storeIntegerofObjectwithValue)(INTERPRETER_ARG_COMMA sqInt fieldIndex, sqInt oop, sqInt integer);
	sqInt  (*storePointerofObjectwithValue)(INTERPRETER_ARG_COMMA sqInt fieldIndex, sqInt oop, sqInt valuePointer);

	/* InterpreterProxy methodsFor: 'testing' */

	sqInt (*isKindOf)(INTERPRETER_ARG_COMMA sqInt oop, char *aString);
	sqInt (*isMemberOf)(INTERPRETER_ARG_COMMA sqInt oop, char *aString);
	sqInt (*isBytes)(sqInt oop);
	sqInt (*isFloatObject)(INTERPRETER_ARG_COMMA sqInt oop);
	sqInt (*isIndexable)(sqInt oop);
	sqInt (*isIntegerObject)(sqInt objectPointer);
	sqInt (*isIntegerValue)(sqInt intValue);
	sqInt (*isPointers)(sqInt oop);
	sqInt (*isWeak)(sqInt oop);
	sqInt (*isWords)(sqInt oop);
	sqInt (*isWordsOrBytes)(sqInt oop);

	/* InterpreterProxy methodsFor: 'converting' */

	sqInt  (*booleanValueOf)(INTERPRETER_ARG_COMMA sqInt obj);
	sqInt  (*checkedIntegerValueOf)(INTERPRETER_ARG_COMMA sqInt intOop);
	sqInt  (*floatObjectOf)(INTERPRETER_ARG_COMMA double aFloat);
	double (*floatValueOf)(INTERPRETER_ARG_COMMA sqInt oop);
	sqInt  (*integerObjectOf)(sqInt value);
	sqInt  (*integerValueOf)(sqInt oop);
	sqInt  (*positive32BitIntegerFor)(INTERPRETER_ARG_COMMA sqInt integerValue);
	sqInt  (*positive32BitValueOf)(INTERPRETER_ARG_COMMA sqInt oop);

	/* InterpreterProxy methodsFor: 'special objects' */

	sqInt (*characterTable)(INTERPRETER_ARG);
	sqInt (*displayObject)(INTERPRETER_ARG);
	sqInt (*falseObject)(INTERPRETER_ARG);
	sqInt (*nilObject)(INTERPRETER_ARG);
	sqInt (*trueObject)(INTERPRETER_ARG);

	/* InterpreterProxy methodsFor: 'special classes' */

	sqInt (*classArray)(INTERPRETER_ARG);
	sqInt (*classBitmap)(INTERPRETER_ARG);
	sqInt (*classByteArray)(INTERPRETER_ARG);
	sqInt (*classCharacter)(INTERPRETER_ARG);
	sqInt (*classFloat)(INTERPRETER_ARG);
	sqInt (*classLargePositiveInteger)(INTERPRETER_ARG);
	sqInt (*classPoint)(INTERPRETER_ARG);
	sqInt (*classSemaphore)(INTERPRETER_ARG);
	sqInt (*classSmallInteger)(INTERPRETER_ARG);
	sqInt (*classString)(INTERPRETER_ARG);

	/* InterpreterProxy methodsFor: 'instance creation' */

	sqInt (*clone)(INTERPRETER_ARG_COMMA sqInt oop);
	sqInt (*instantiateClassindexableSize)(INTERPRETER_ARG_COMMA sqInt classPointer, sqInt size);
	sqInt (*makePointwithxValueyValue)(INTERPRETER_ARG_COMMA sqInt xValue, sqInt yValue);
	sqInt (*popRemappableOop)(INTERPRETER_ARG);
	sqInt (*pushRemappableOop)(INTERPRETER_ARG_COMMA sqInt oop);

	/* InterpreterProxy methodsFor: 'other' */

	sqInt (*becomewith)(INTERPRETER_ARG_COMMA sqInt array1, sqInt array2);
	sqInt (*byteSwapped)(sqInt w);
	sqInt (*failed)(INTERPRETER_ARG);
	sqInt (*fullDisplayUpdate)(INTERPRETER_ARG);
	sqInt (*fullGC)(INTERPRETER_ARG);
	sqInt (*incrementalGC)(INTERPRETER_ARG);
	sqInt (*primitiveFail)(INTERPRETER_ARG);
	sqInt (*showDisplayBitsLeftTopRightBottom)(INTERPRETER_ARG_COMMA sqInt aForm, sqInt l, sqInt t, sqInt r, sqInt b);
	sqInt (*signalSemaphoreWithIndex)(INTERPRETER_ARG_COMMA sqInt semaIndex);
	sqInt (*success)(INTERPRETER_ARG_COMMA sqInt aBoolean);
	sqInt (*superclassOf)(sqInt classPointer);

	/* InterpreterProxy methodsFor: 'compiler' */

	CompilerHook *(*compilerHookVector)(INTERPRETER_ARG);
	sqInt          (*setCompilerInitialized)(INTERPRETER_ARG_COMMA sqInt initFlag);

	/* InterpreterProxy methodsFor: 'BitBlt support' */

	sqInt (*loadBitBltFrom)(INTERPRETER_ARG_COMMA sqInt bbOop);
	sqInt (*copyBits)(INTERPRETER_ARG);
	sqInt (*copyBitsFromtoat)(INTERPRETER_ARG_COMMA sqInt leftX, sqInt rightX, sqInt yValue);

	sqInt (*classLargeNegativeInteger)(INTERPRETER_ARG);
	sqInt (*signed32BitIntegerFor)(INTERPRETER_ARG_COMMA sqInt integerValue);
	sqInt (*signed32BitValueOf)(INTERPRETER_ARG_COMMA sqInt oop);
	sqInt (*includesBehaviorThatOf)(INTERPRETER_ARG_COMMA sqInt aClass, sqInt aSuperClass);
	sqInt (*primitiveMethod)(INTERPRETER_ARG);

	/* InterpreterProxy methodsFor: 'FFI support' */

	sqInt (*classExternalAddress)(INTERPRETER_ARG);
	sqInt (*classExternalData)(INTERPRETER_ARG);
	sqInt (*classExternalFunction)(INTERPRETER_ARG);
	sqInt (*classExternalLibrary)(INTERPRETER_ARG);
	sqInt (*classExternalStructure)(INTERPRETER_ARG);
	void* (*ioLoadModuleOfLength)(sqInt modIndex, sqInt modLength);
	void* (*ioLoadSymbolOfLengthFromModule)(sqInt fnIndex, sqInt fnLength, void* handle);
	sqInt (*isInMemory)(INTERPRETER_ARG_COMMA sqInt address);

	void* (*ioLoadFunctionFrom)(char *fnName, char *modName);
	sqInt (*ioMicroMSecs)(void);

	sqInt  (*positive64BitIntegerFor)(INTERPRETER_ARG_COMMA sqLong integerValue);
	sqLong (*positive64BitValueOf)(INTERPRETER_ARG_COMMA sqInt oop);
	sqInt  (*signed64BitIntegerFor)(INTERPRETER_ARG_COMMA sqLong integerValue);
	sqLong (*signed64BitValueOf)(INTERPRETER_ARG_COMMA sqInt oop);

	sqInt  (*isArray)(sqInt oop);
	sqInt  (*forceInterruptCheck)(INTERPRETER_ARG);
	sqInt  (*fetchLong32ofObject)(sqInt fieldFieldIndex, sqInt oop);
	sqInt  (*getThisSessionID)(INTERPRETER_ARG);
	sqInt  (*ioFilenamefromStringofLengthresolveAliases)(char* aCharBuffer, char* filenameIndex, sqInt filenameLength, sqInt resolveFlag);
	sqInt  (*vmEndianness)(void);	

#if OBJVM_PROXY_MINOR > 1
	/* put new functions here */
#endif

} ObjVirtualMachine;

#endif /* VM_OBJECTIFIED */

#endif /* _SqueakVM_H */
