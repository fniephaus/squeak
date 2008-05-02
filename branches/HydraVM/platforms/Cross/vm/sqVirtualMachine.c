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

/* include function wrappers for legacy plugins support */

#include "interp_prototypes.inc"


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
	getVMFunctionPointerBySelector
};


struct ObjVirtualMachine* sqGetObjInterpreterProxy(void)
{
	return &ObjVM;
}

/* interpreterProxy for internal plugins */
#ifdef VM_OBJECTIFIED
struct ObjVirtualMachine * _objInterpreterProxy = &ObjVM;
#endif
struct VirtualMachine * _interpreterProxy = &VM;


#endif /* VM_OBJECTIFIED */
