#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sq.h"
#include "interp_prototypes.h"

static sqInt majorVersion(void) {
	return VM_PROXY_MAJOR;
}

static sqInt minorVersion(void) {
	return VM_PROXY_MINOR;
}

#ifdef VM_OBJECTIFIED


/* Functions for support of legacy plugins */
sqInt legacy_isInMemory(sqInt address) {
	return isInMemory(mainVM, address);
}
sqInt legacy_positive64BitIntegerFor(sqLong integerValue) {
	return positive64BitIntegerFor(mainVM, integerValue);
}
sqInt legacy_stackIntegerValue(sqInt offset) {
	return stackIntegerValue(mainVM, offset);
}
sqInt legacy_copyBits() {
	return copyBits(mainVM);
}
#define legacy_ioFilenamefromStringofLengthresolveAliases ioFilenamefromStringofLengthresolveAliases
sqInt legacy_setCompilerInitialized(sqInt newFlag) {
	return setCompilerInitialized(mainVM, newFlag);
}
sqInt legacy_pushFloat(double  f) {
	return pushFloat(mainVM, f);
}
double legacy_floatValueOf(sqInt oop) {
	return floatValueOf(mainVM, oop);
}
sqInt legacy_pushRemappableOop(sqInt oop) {
	return pushRemappableOop(mainVM, oop);
}
sqInt legacy_displayObject() {
	return displayObject(mainVM);
}
sqInt legacy_classPoint() {
	return classPoint(mainVM);
}
sqInt legacy_includesBehaviorThatOf(sqInt aClass, sqInt aSuperclass) {
	return includesBehaviorThatOf(mainVM, aClass, aSuperclass);
}
#define legacy_integerValueOf integerValueOf
#define legacy_vmEndianness vmEndianness
sqInt legacy_makePointwithxValueyValue(sqInt xValue, sqInt yValue) {
	return makePointwithxValueyValue(mainVM, xValue, yValue);
}
sqInt legacy_classExternalStructure() {
	return classExternalStructure(mainVM);
}
#define legacy_integerObjectOf integerObjectOf
sqInt legacy_stackValue(sqInt offset) {
	return stackValue(mainVM, offset);
}
#define legacy_byteSizeOf byteSizeOf
#define legacy_byteSwapped byteSwapped
sqInt legacy_stSizeOf(sqInt oop) {
	return stSizeOf(mainVM, oop);
}
sqInt legacy_classSemaphore() {
	return classSemaphore(mainVM);
}
sqInt legacy_classExternalLibrary() {
	return classExternalLibrary(mainVM);
}
sqInt legacy_failed() {
	return failed(mainVM);
}
sqInt legacy_positive32BitIntegerFor(sqInt integerValue) {
	return positive32BitIntegerFor(mainVM, integerValue);
}
sqInt legacy_popRemappableOop() {
	return popRemappableOop(mainVM);
}
sqInt legacy_forceInterruptCheck() {
	return forceInterruptCheck(mainVM);
}
#define legacy_isIndexable isIndexable
sqLong legacy_positive64BitValueOf(sqInt oop) {
	return positive64BitValueOf(mainVM, oop);
}
#define legacy_fetchPointerofObject fetchPointerofObject
sqInt legacy_fullDisplayUpdate() {
	return fullDisplayUpdate(mainVM);
}
sqInt legacy_stackObjectValue(sqInt offset) {
	return stackObjectValue(mainVM, offset);
}
sqInt legacy_nilObject() {
	return nilObject(mainVM);
}
sqInt legacy_pushBool(sqInt trueOrFalse) {
	return pushBool(mainVM, trueOrFalse);
}
sqInt legacy_methodPrimitiveIndex() {
	return methodPrimitiveIndex(mainVM);
}
sqInt legacy_stObjectat(sqInt array, sqInt index) {
	return stObjectat(mainVM, array, index);
}
sqInt legacy_falseObject() {
	return falseObject(mainVM);
}
sqInt legacy_push(sqInt object) {
	return push(mainVM, object);
}
sqInt legacy_classExternalData() {
	return classExternalData(mainVM);
}
sqInt legacy_isFloatObject(sqInt oop) {
	return isFloatObject(mainVM, oop);
}
void * legacy_compilerHookVector() {
	return compilerHookVector(mainVM);
}
#define legacy_isIntegerObject isIntegerObject
sqInt legacy_signalSemaphoreWithIndex(sqInt index) {
	return signalSemaphoreWithIndex(mainVM, index);
}
#define legacy_literalofMethod literalofMethod
sqInt legacy_storePointerofObjectwithValue(sqInt fieldIndex, sqInt oop, sqInt valuePointer) {
	return storePointerofObjectwithValue(mainVM, fieldIndex, oop, valuePointer);
}
sqInt legacy_primitiveFail() {
	return primitiveFail(mainVM);
}
sqInt legacy_sizeOfSTArrayFromCPrimitive(void * cPtr) {
	return sizeOfSTArrayFromCPrimitive(mainVM, cPtr);
}
#define legacy_firstFixedField firstFixedField
sqInt legacy_isKindOf(sqInt oop, char * className) {
	return isKindOf(mainVM, oop, className);
}
sqInt legacy_signed64BitIntegerFor(sqLong integerValue) {
	return signed64BitIntegerFor(mainVM, integerValue);
}
sqInt legacy_floatObjectOf(double  aFloat) {
	return floatObjectOf(mainVM, aFloat);
}
#define legacy_arrayValueOf arrayValueOf
double legacy_fetchFloatofObject(sqInt fieldIndex, sqInt objectPointer) {
	return fetchFloatofObject(mainVM, fieldIndex, objectPointer);
}
sqInt legacy_characterTable() {
	return characterTable(mainVM);
}
void * legacy_firstIndexableField(sqInt oop) {
	return firstIndexableField(mainVM, oop);
}
#define legacy_isArray isArray
sqInt legacy_stObjectatput(sqInt array, sqInt index, sqInt value) {
	return stObjectatput(mainVM, array, index, value);
}
sqInt legacy_signed32BitValueOf(sqInt oop) {
	return signed32BitValueOf(mainVM, oop);
}
#define legacy_isWordsOrBytes isWordsOrBytes
sqInt legacy_success(sqInt successValue) {
	return success(mainVM, successValue);
}
sqInt legacy_classFloat() {
	return classFloat(mainVM);
}
double legacy_stackFloatValue(sqInt offset) {
	return stackFloatValue(mainVM, offset);
}
#define legacy_obsoleteDontUseThisFetchWordofObject obsoleteDontUseThisFetchWordofObject
sqInt legacy_fullGC() {
	return fullGC(mainVM);
}
sqInt legacy_classByteArray() {
	return classByteArray(mainVM);
}
sqInt legacy_getThisSessionID() {
	return getThisSessionID(mainVM);
}
#define legacy_argumentCountOf argumentCountOf
sqInt legacy_clone(sqInt oop) {
	return clone(mainVM, oop);
}
sqInt legacy_classString() {
	return classString(mainVM);
}
sqInt legacy_copyBitsFromtoat(sqInt x0, sqInt x1, sqInt y) {
	return copyBitsFromtoat(mainVM, x0, x1, y);
}
#define legacy_isWords isWords
#define legacy_literalCountOf literalCountOf
sqInt legacy_classLargePositiveInteger() {
	return classLargePositiveInteger(mainVM);
}
sqInt legacy_checkedIntegerValueOf(sqInt intOop) {
	return checkedIntegerValueOf(mainVM, intOop);
}
sqInt legacy_classLargeNegativeInteger() {
	return classLargeNegativeInteger(mainVM);
}
sqInt legacy_signed32BitIntegerFor(sqInt integerValue) {
	return signed32BitIntegerFor(mainVM, integerValue);
}
sqInt legacy_instantiateClassindexableSize(sqInt classPointer, sqInt size) {
	return instantiateClassindexableSize(mainVM, classPointer, size);
}
sqInt legacy_popthenPush(sqInt nItems, sqInt oop) {
	return popthenPush(mainVM, nItems, oop);
}
sqInt legacy_incrementalGC() {
	return incrementalGC(mainVM);
}
sqInt legacy_classCharacter() {
	return classCharacter(mainVM);
}
void * legacy_fetchArrayofObject(sqInt fieldIndex, sqInt objectPointer) {
	return fetchArrayofObject(mainVM, fieldIndex, objectPointer);
}
sqInt legacy_primitiveMethod() {
	return primitiveMethod(mainVM);
}
#define legacy_isPointers isPointers
sqInt legacy_classBitmap() {
	return classBitmap(mainVM);
}
#define legacy_slotSizeOf slotSizeOf
sqInt legacy_becomewith(sqInt array1, sqInt array2) {
	return becomewith(mainVM, array1, array2);
}
sqInt legacy_showDisplayBitsLeftTopRightBottom(sqInt aForm, sqInt l, sqInt t, sqInt r, sqInt b) {
	return showDisplayBitsLeftTopRightBottom(mainVM, aForm, l, t, r, b);
}
sqInt legacy_pushInteger(sqInt integerValue) {
	return pushInteger(mainVM, integerValue);
}
sqInt legacy_fetchClassOf(sqInt oop) {
	return fetchClassOf(mainVM, oop);
}
sqInt legacy_fetchIntegerofObject(sqInt fieldIndex, sqInt objectPointer) {
	return fetchIntegerofObject(mainVM, fieldIndex, objectPointer);
}
sqLong legacy_signed64BitValueOf(sqInt oop) {
	return signed64BitValueOf(mainVM, oop);
}
sqInt legacy_classSmallInteger() {
	return classSmallInteger(mainVM);
}
sqInt legacy_classExternalAddress() {
	return classExternalAddress(mainVM);
}
sqInt legacy_isMemberOf(sqInt oop, char * className) {
	return isMemberOf(mainVM, oop, className);
}
sqInt legacy_classExternalFunction() {
	return classExternalFunction(mainVM);
}
#define legacy_superclassOf superclassOf
sqInt legacy_classArray() {
	return classArray(mainVM);
}
sqInt legacy_storeIntegerofObjectwithValue(sqInt fieldIndex, sqInt objectPointer, sqInt integerValue) {
	return storeIntegerofObjectwithValue(mainVM, fieldIndex, objectPointer, integerValue);
}
sqInt legacy_trueObject() {
	return trueObject(mainVM);
}
#define legacy_primitiveIndexOf primitiveIndexOf
sqInt legacy_booleanValueOf(sqInt obj) {
	return booleanValueOf(mainVM, obj);
}
sqInt legacy_positive32BitValueOf(sqInt oop) {
	return positive32BitValueOf(mainVM, oop);
}
sqInt legacy_loadBitBltFrom(sqInt bb) {
	return loadBitBltFrom(mainVM, bb);
}
#define legacy_isBytes isBytes
#define legacy_fetchLong32ofObject fetchLong32ofObject
sqInt legacy_methodArgumentCount() {
	return methodArgumentCount(mainVM);
}
#define legacy_isIntegerValue isIntegerValue
sqInt legacy_pop(sqInt nItems) {
	return pop(mainVM, nItems);
}
#define legacy_isWeak isWeak

#define VM_FN(name) legacy_##name

#else

#define VM_FN(name) name

#endif





struct VirtualMachine VM = { 
	minorVersion,
	majorVersion,

	/* InterpreterProxy methodsFor: 'stack access' */
	VM_FN(pop),
	VM_FN(popthenPush),
	VM_FN(push),
	VM_FN(pushBool),
	VM_FN(pushFloat),
	VM_FN(pushInteger),
	VM_FN(stackFloatValue),
	VM_FN(stackIntegerValue),
	VM_FN(stackObjectValue),
	VM_FN(stackValue),
	
	/* InterpreterProxy methodsFor: 'object access' */
	VM_FN(argumentCountOf),
	VM_FN(arrayValueOf),
	VM_FN(byteSizeOf),
	VM_FN(fetchArrayofObject),
	VM_FN(fetchClassOf),
	VM_FN(fetchFloatofObject),
	VM_FN(fetchIntegerofObject),
	VM_FN(fetchPointerofObject),
	VM_FN(obsoleteDontUseThisFetchWordofObject),
	VM_FN(firstFixedField),
	VM_FN(firstIndexableField),
	VM_FN(literalofMethod),
	VM_FN(literalCountOf),
	VM_FN(methodArgumentCount),
	VM_FN(methodPrimitiveIndex),
	VM_FN(primitiveIndexOf),
	VM_FN(sizeOfSTArrayFromCPrimitive),
	VM_FN(slotSizeOf),
	VM_FN(stObjectat),
	VM_FN(stObjectatput),
	VM_FN(stSizeOf),
	VM_FN(storeIntegerofObjectwithValue),
	VM_FN(storePointerofObjectwithValue),

	/* InterpreterProxy methodsFor: 'testing' */
	VM_FN(isKindOf),
	VM_FN(isMemberOf),
	VM_FN(isBytes),
	VM_FN(isFloatObject),
	VM_FN(isIndexable),
	VM_FN(isIntegerObject),
	VM_FN(isIntegerValue),
	VM_FN(isPointers),
	VM_FN(isWeak),
	VM_FN(isWords),
	VM_FN(isWordsOrBytes),
	      
	/* InterpreterProxy methodsFor: 'converting' */
	VM_FN(booleanValueOf),
	VM_FN(checkedIntegerValueOf),
	VM_FN(floatObjectOf),
	VM_FN(floatValueOf),
	VM_FN(integerObjectOf),
	VM_FN(integerValueOf),
	VM_FN(positive32BitIntegerFor),
	VM_FN(positive32BitValueOf),
       
	/* InterpreterProxy methodsFor: 'special objects' */
	VM_FN(characterTable),
	VM_FN(displayObject),
	VM_FN(falseObject),
	VM_FN(nilObject),
	VM_FN(trueObject),
	
	/* InterpreterProxy methodsFor: 'special classes' */
	VM_FN(classArray),
	VM_FN(classBitmap),
	VM_FN(classByteArray),
	VM_FN(classCharacter),
	VM_FN(classFloat),
	VM_FN(classLargePositiveInteger),
	VM_FN(classPoint),
	VM_FN(classSemaphore),
	VM_FN(classSmallInteger),
	VM_FN(classString),
	
	/* InterpreterProxy methodsFor: 'instance creation' */
	VM_FN(clone),
	VM_FN(instantiateClassindexableSize),
	VM_FN(makePointwithxValueyValue),
	VM_FN(popRemappableOop),
	VM_FN(pushRemappableOop),
	
	/* InterpreterProxy methodsFor: 'other' */
	VM_FN(becomewith),
	VM_FN(byteSwapped),
	VM_FN(failed),
	VM_FN(fullDisplayUpdate),
	VM_FN(fullGC),
	VM_FN(incrementalGC),
	VM_FN(primitiveFail),
	VM_FN(showDisplayBitsLeftTopRightBottom),
	VM_FN(signalSemaphoreWithIndex),
	VM_FN(success),
	VM_FN(superclassOf),

	(CompilerHook *(*)(void))VM_FN(compilerHookVector),
	VM_FN(setCompilerInitialized),

#if VM_PROXY_MINOR > 1

	/* InterpreterProxy methodsFor: 'BitBlt support' */
	VM_FN(loadBitBltFrom),
	VM_FN(copyBits),
	VM_FN(copyBitsFromtoat),

#endif

#if VM_PROXY_MINOR > 2

	VM_FN(classLargeNegativeInteger),
	VM_FN(signed32BitIntegerFor),
	VM_FN(signed32BitValueOf),
	VM_FN(includesBehaviorThatOf),
	VM_FN(primitiveMethod),

	/* InterpreterProxy methodsFor: 'FFI support' */
	VM_FN(classExternalAddress),
	VM_FN(classExternalData),
	VM_FN(classExternalFunction),
	VM_FN(classExternalLibrary),
	VM_FN(classExternalStructure),
/**/	ioLoadModuleOfLength,
/**/	ioLoadSymbolOfLengthFromModule,
	VM_FN(isInMemory),

#endif

#if VM_PROXY_MINOR > 3

/**/	ioLoadFunctionFrom,
/**/	ioMicroMSecs,

#endif

#if VM_PROXY_MINOR > 4

	VM_FN(positive64BitIntegerFor),
	VM_FN(positive64BitValueOf),
	VM_FN(signed64BitIntegerFor),
	VM_FN(signed64BitValueOf),
#endif

#if VM_PROXY_MINOR > 5
	VM_FN(isArray),
	VM_FN(forceInterruptCheck),
#endif

#if VM_PROXY_MINOR > 6
	VM_FN(fetchLong32ofObject),
	VM_FN(getThisSessionID),
	VM_FN(ioFilenamefromStringofLengthresolveAliases),
	VM_FN(vmEndianness)
#endif

};

struct VirtualMachine* sqGetInterpreterProxy(void)
{
	return &VM;
}

/*************** VM Proxy for objectified VM ***********************************/

#ifdef VM_OBJECTIFIED

sqInt objVMminorVersion(void)
{
	return OBJVM_PROXY_MINOR;
}
sqInt objVMmajorVersion(void)
{
	return OBJVM_PROXY_MAJOR;
}

struct ObjVirtualMachine ObjVM = { 

	objVMminorVersion,
	objVMmajorVersion,

	/* InterpreterProxy methodsFor: 'stack access' */
	pop,
        popthenPush,
	push,
	pushBool,
	pushFloat,
	pushInteger,
	stackFloatValue,
	stackIntegerValue,
	stackObjectValue,
	stackValue,

	/* InterpreterProxy methodsFor: 'object access' */

	argumentCountOf,
	arrayValueOf,
	byteSizeOf,
	fetchArrayofObject,
	fetchClassOf,
	fetchFloatofObject,
	fetchIntegerofObject,
	fetchPointerofObject,
	firstFixedField,
	firstIndexableField,
	literalofMethod,
	literalCountOf,
	methodArgumentCount,
	methodPrimitiveIndex,
	primitiveIndexOf,
	sizeOfSTArrayFromCPrimitive,
	slotSizeOf,
	stObjectat,
	stObjectatput,
	stSizeOf,
	storeIntegerofObjectwithValue,
	storePointerofObjectwithValue,

	/* InterpreterProxy methodsFor: 'testing' */

	isKindOf,
	isMemberOf,
	isBytes,
	isFloatObject,
	isIndexable,
	isIntegerObject,
	isIntegerValue,
	isPointers,
	isWeak,
	isWords,
	isWordsOrBytes,

	/* InterpreterProxy methodsFor: 'converting' */

	booleanValueOf,
	checkedIntegerValueOf,
	floatObjectOf,
	floatValueOf,
	integerObjectOf,
	integerValueOf,
	positive32BitIntegerFor,
	positive32BitValueOf,

	/* InterpreterProxy methodsFor: 'special objects' */

	characterTable,
	displayObject,
	falseObject,
	nilObject,
	trueObject,

	/* InterpreterProxy methodsFor: 'special classes' */

	classArray,
	classBitmap,
	classByteArray,
	classCharacter,
	classFloat,
	classLargePositiveInteger,
	classPoint,
	classSemaphore,
	classSmallInteger,
	classString,

	/* InterpreterProxy methodsFor: 'instance creation' */

	clone,
	instantiateClassindexableSize,
	makePointwithxValueyValue,
	popRemappableOop,
	pushRemappableOop,

	/* InterpreterProxy methodsFor: 'other' */

	becomewith,
	byteSwapped,
	failed,
	fullDisplayUpdate,
	fullGC,
	incrementalGC,
	primitiveFail,
	showDisplayBitsLeftTopRightBottom,
	signalSemaphoreWithIndex,
	success,
	superclassOf,

	/* InterpreterProxy methodsFor: 'compiler' */

	(CompilerHook *(*)(INTERPRETER_ARG))compilerHookVector,
	setCompilerInitialized,

	/* InterpreterProxy methodsFor: 'BitBlt support' */

	loadBitBltFrom,
	copyBits,
	copyBitsFromtoat,

	classLargeNegativeInteger,
	signed32BitIntegerFor,
	signed32BitValueOf,
	includesBehaviorThatOf,
	primitiveMethod,

	/* InterpreterProxy methodsFor: 'FFI support' */

	classExternalAddress,
	classExternalData,
	classExternalFunction,
	classExternalLibrary,
	classExternalStructure,
	ioLoadModuleOfLength,
	ioLoadSymbolOfLengthFromModule,
	isInMemory,

	ioLoadFunctionFrom,
	ioMicroMSecs,

	positive64BitIntegerFor,
	positive64BitValueOf,
	signed64BitIntegerFor,
	signed64BitValueOf,

	isArray,
	forceInterruptCheck,
	fetchLong32ofObject,
	getThisSessionID,
	ioFilenamefromStringofLengthresolveAliases,
	vmEndianness

#if OBJVM_PROXY_MINOR > 1
	/* put new functions here */
#endif

};


struct ObjVirtualMachine* sqGetObjInterpreterProxy(void)
{
	return &ObjVM;
}

#endif /* VM_OBJECTIFIED */
