/* Automatically generated by
	CCodeGeneratorGlobalStructure VMMaker.oscog-eem.258 uuid: da1433f1-de50-475f-be33-f462b300a2ea
 */


/*** Function Prototypes ***/
sqInt activeProcess(void);
sqInt addressCouldBeObj(sqInt address);
sqInt addressCouldBeOop(sqInt address);
usqInt argumentCountAddress(void);
sqInt argumentCountOfMethodHeader(sqInt header);
sqInt argumentCountOf(sqInt methodPointer);
void assertValidMachineCodeFrame(sqInt instrPtr);
void beRootIfOld(sqInt oop);
sqInt byteSizeOf(sqInt oop);
sqInt byteSwapped(sqInt w);
void callForCogCompiledCodeCompaction(void);
sqInt canContextSwitchIfActivatingheader(sqInt theMethod, sqInt methodHeader);
void ceActivateFailingPrimitiveMethod(sqInt aPrimitiveMethod);
sqInt ceActiveContext(void);
sqInt ceBaseFrameReturn(sqInt returnValue);
sqInt ceCannotResume(void);
void ceCheckForInterrupts(void);
void ceCheckProfileTick(void);
sqInt ceContextinstVar(sqInt maybeContext, sqInt slotIndex);
sqInt ceContextinstVarvalue(sqInt maybeMarriedContext, sqInt slotIndex, sqInt anOop);
sqInt ceCounterTripped(sqInt condition);
sqInt ceDynamicSuperSendtonumArgs(sqInt selector, sqInt rcvr, sqInt numArgs);
sqInt ceExplicitReceiverAt(sqInt level);
sqInt ceInterpretMethodFromPICreceiver(sqInt aMethodObj, sqInt rcvr);
void ceMNUFromPICMNUMethodreceiver(sqInt aMethodObj, sqInt rcvr);
sqInt ceNewArraySlotSize(sqInt slotSize);
sqInt ceNonLocalReturn(sqInt returnValue);
sqInt cePositive32BitIntegerFor(usqInt anInteger);
sqInt ceReturnToInterpreter(sqInt anOop);
sqInt ceSendAborttonumArgs(sqInt selector, sqInt rcvr, sqInt numArgs);
sqInt ceSendFromInLineCacheMiss(CogMethod *oPIC);
sqInt ceSendMustBeBoolean(sqInt anObject);
sqInt ceSendsupertonumArgs(sqInt selector, sqInt superNormalBar, sqInt rcvr, sqInt numArgs);
void ceStackOverflow(sqInt contextSwitchIfNotNil);
sqInt ceStoreCheck(sqInt anOop);
void ceTraceBlockActivation(void);
void ceTraceLinkedSend(sqInt theReceiver);
void ceTraceStoreOfinto(sqInt aValue, sqInt anObject);
sqInt characterTable(void);
sqInt checkAllAccessibleObjectsOkay(void);
void checkAssertsEnabledInCoInterpreter(void);
sqInt checkedLongAt(sqInt byteAddress);
sqInt checkOkayInterpreterObjects(sqInt writeBack);
sqInt checkOkayOop(usqInt oop);
sqInt checkOopHasOkayClass(sqInt obj);
sqInt classArray(void);
sqInt classFloat(void);
sqInt classFloatCompactIndex(void);
sqInt classHeader(sqInt oop);
sqInt classSmallInteger(void);
void clearTraceLog(void);
CogMethod * cogMethodOf(sqInt aMethodOop);
sqInt compactClassAt(sqInt ccIndex);
sqInt compactClassIndexOfHeader(sqInt header);
sqInt compactClassIndexOf(sqInt oop);
void compilationBreakpointFor(sqInt selectorOop);
sqInt createClosureNumArgsnumCopiedstartpc(sqInt numArgs, sqInt numCopied, sqInt initialIP);
char * cStringOrNullFor(sqInt oop);
sqInt disownVM(sqInt flags);
sqInt doSignalSemaphoreWithIndex(sqInt index);
void dumpPrimTraceLog(void);
void dumpTraceLog(void);
sqInt eeInstantiateClassindexableSize(sqInt classPointer, sqInt size);
extern void error(char *s);
void executeCogMethodFromLinkedSendwithReceiver(CogMethod *cogMethod, sqInt rcvr);
void executeCogMethodFromLinkedSendwithReceiverandCacheTag(CogMethod *cogMethod, sqInt rcvr, sqInt cacheTag);
sqInt failed(void);
sqInt falseObject(void);
sqInt fetchByteofObject(sqInt byteIndex, sqInt oop);
void findStringBeginningWith(char *aCString);
void findString(char *aCString);
void flushExternalPrimitiveOf(sqInt methodObj);
sqInt formatOfClass(sqInt classPointer);
usqInt framePointerAddress(void);
usqInt freeStartAddress(void);
void (*functionPointerForCompiledMethodprimitiveIndex(sqInt methodObj, sqInt primIndex))(void) ;
void (*functionPointerForinClass(sqInt primIdx,sqInt theClass))(void) ;
usqLong getNextWakeupUsecs(void);
sqInt * getStackPointer(void);
sqOSThread getVMOSThread(void);
sqInt headerIndicatesAlternateBytecodeSet(sqInt methodHeader);
sqInt headerOf(sqInt methodPointer);
sqInt highBit(usqInt anUnsignedValue);
void ifValidWriteBackStackPointersSaveTo(void *theCFP, void *theCSP, char **savedFPP, char **savedSPP);
sqInt implicitReceiverFormixinimplementing(sqInt rcvr, sqInt mixin, sqInt selector);
sqInt initialPCForHeadermethod(sqInt methodHeader, sqInt theMethod);
sqInt instanceSizeOf(sqInt classObj);
sqInt instantiateClassindexableSize(sqInt classPointer, sqInt size);
usqInt instructionPointerAddress(void);
usqInt interpretAddress(void);
sqInt isCogMethodReference(sqInt methodHeader);
sqInt isCompiledMethod(sqInt oop);
sqInt isMarked(sqInt oop);
sqInt isNonIntegerObject(sqInt objectPointer);
sqInt isOopCompiledMethod(sqInt oop);
sqInt isOopImmutable(sqInt anOop);
sqInt isOopMutable(sqInt anOop);
sqInt isQuickPrimitiveIndex(sqInt anInteger);
sqInt isReallyYoungObject(sqInt obj);
sqInt isYoungObject(sqInt obj);
sqInt isYoung(sqInt oop);
sqInt isKindOfClass(sqInt oop, sqInt aClass);
sqInt lastPointerOf(sqInt oop);
sqInt leakCheckBecome(void);
sqInt leakCheckFullGC(void);
sqInt leakCheckIncrementalGC(void);
sqInt lengthOf(sqInt oop);
sqInt literalCountOfHeader(sqInt headerPointer);
sqInt literalofMethod(sqInt offset, sqInt methodPointer);
void longPrintOop(sqInt oop);
void longPrintReferencesTo(sqInt anOop);
sqInt longStoreBytecodeForHeader(sqInt methodHeader);
sqInt lookupreceiver(sqInt selector, sqInt rcvr);
void markActiveMethodsAndReferents(void);
void markAndTrace(sqInt oop);
sqInt maybeSplObj(sqInt index);
void * methodCacheAddress(void);
sqInt methodClassAssociationOf(sqInt methodPointer);
sqInt methodHasCogMethod(sqInt aMethodOop);
sqInt methodPrimitiveIndex(void);
sqInt methodShouldBeCogged(sqInt aMethodObj);
sqInt methodUsesAlternateBytecodeSet(sqInt aMethodObj);
CogMethod * mframeHomeMethodExport(void);
CogMethod * mframeHomeMethod(char *theFP);
sqInt mMethodClass(void);
usqInt newMethodAddress(void);
usqInt nextProfileTickAddress(void);
sqInt nilObject(void);
sqInt noAssertHeaderOf(sqInt methodPointer);
sqInt nullHeaderForMachineCodeMethod(void);
sqInt objectAfter(sqInt oop);
sqInt objectBefore(sqInt address);
sqInt objectExactlyBefore(sqInt oop);
sqInt objectIsOld(sqInt anObject);
sqInt ownVM(sqInt threadIndexAndFlags);
sqInt popRemappableOop(void);
sqInt popStack(void);
sqInt primErrTable(void);
usqInt primFailCodeAddress(void);
void primitiveClosureValueNoContextSwitch(void);
sqInt primitiveErrorTable(void);
sqInt primitiveFail(void);
usqInt primitiveFailAddress(void);
sqInt primitiveFailFor(sqInt reasonCode);
sqInt primitiveFailureCode(void);
usqInt primitiveFunctionPointerAddress(void);
sqInt primitiveIndexOfMethodheader(sqInt theMethod, sqInt methodHeader);
sqInt primitiveIndexOf(sqInt methodPointer);
sqInt primitivePropertyFlags(sqInt primIndex);
void * primTraceLogAddress(void);
usqInt primTraceLogIndexAddress(void);
void printAllStacks(void);
void printCallStackOf(sqInt aContextOrProcess);
void printChar(sqInt aByte);
void printCogMethod(CogMethod *cogMethod);
void printHex(sqInt n);
void printInstancesOf(sqInt aClassOop);
void printMethodDictionaryOf(sqInt behavior);
void printMethodDictionary(sqInt dictionary);
void printMethodReferencesTo(sqInt anOop);
void printObjectsFromto(sqInt startAddress, sqInt endAddress);
void printProcessStack(sqInt aProcess);
void printProcsOnList(sqInt procList);
void printReferencesTo(sqInt anOop);
void print(char *s);
void pushRemappableOop(sqInt oop);
sqInt quickPrimitiveConstantFor(sqInt aQuickPrimitiveIndex);
int (*quickPrimitiveGeneratorFor(sqInt aQuickPrimitiveIndex))(void) ;
sqInt quickPrimitiveInstVarIndexFor(sqInt primIndex);
sqInt rawHeaderOf(sqInt methodPointer);
void rawHeaderOfput(sqInt methodOop, sqInt cogMethodOrMethodHeader);
sqInt remap(sqInt oop);
usqInt scavengeThresholdAddress(void);
void scheduleIncrementalGC(void);
void setBreakSelector(char *aString);
void setNextWakeupUsecs(usqLong value);
void shortPrintFrameAndNCallers(char *theFP, sqInt n);
sqInt signalNoResume(sqInt aSemaphore);
usqInt sizeOfAlienData(sqInt oop);
sqInt sizeOfCallPrimitiveBytecode(sqInt methodHeader);
sqInt sizeOfLongStoreTempBytecode(sqInt methodHeader);
sqInt specialSelectorNumArgs(sqInt index);
sqInt specialSelector(sqInt index);
sqInt splObj(sqInt index);
usqInt stackLimitAddress(void);
usqInt stackPointerAddress(void);
sqInt stackPositiveMachineIntegerValue(sqInt offset);
sqInt stackSignedMachineIntegerValue(sqInt offset);
sqInt stackTop(void);
sqInt stackValue(sqInt offset);
void * startOfAlienData(sqInt oop);
sqInt startPCOfMethodHeader(sqInt aCompiledMethodHeader);
sqInt startPCOfMethod(sqInt aCompiledMethod);
sqInt storePointerUncheckedofObjectwithValue(sqInt fieldIndex, sqInt oop, sqInt valuePointer);
sqInt stringForCString(const char *aCString);
sqInt tempCountOf(sqInt methodPointer);
void tenuringIncrementalGC(void);
sqInt topRemappableOop(void);
sqInt trueObject(void);
void updateStackZoneReferencesToCompiledCodePreCompaction(void);
usqInt vmOwnerLockAddress(void);
extern void warning(char *s);
usqInt youngStartAddress(void);


/*** Global Variables ***/
char * breakSelector;
sqInt breakSelectorLength ;
sqInt checkForLeaks;
sqInt deferDisplayUpdates;
sqInt desiredCogCodeSize;
sqInt desiredEdenBytes;
sqInt desiredNumStackPages;
sqInt disownCount;
sqInt extraVMMemory;
sqInt inIOProcessEvents;
struct VirtualMachine* interpreterProxy;
sqInt maxLiteralCountForCompile ;
sqInt minBackwardJumpCountForCompile ;
volatile int sendTrace;
void * showSurfaceFn;
sqInt suppressHeartbeatFlag;
sqInt willNotThreadWarnCount;


/*** Macros ***/
#define classFieldOffset() (0 - BaseHeaderSize)
#define compactClassFieldLSB() 12
#define compactClassFieldWidth() 5
#define compilationBreakpoint(sel, len) do { \
	if ((len) == breakSelectorLength \
	 && !strncmp((char *)((sel) + BaseHeaderSize), breakSelector, breakSelectorLength)) { \
		suppressHeartbeatFlag = 1; \
		compilationBreakpointFor(sel); \
	} \
} while (0)
#define instFormatFieldLSB() 8
#define instFormatFieldWidth() 4

