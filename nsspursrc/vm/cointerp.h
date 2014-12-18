/* Automatically generated by
	CCodeGeneratorGlobalStructure VMMaker.oscog-eem.992 uuid: bbbe8c5f-97d3-42fe-b068-76c6890a1733
 */


/*** Function Prototypes ***/


#if !PRODUCTION && defined(__GNUC__) && !defined(NoDbgRegParms)
# define NoDbgRegParms __attribute__ ((regparm (0)))
#endif

#if !defined(NoDbgRegParms)
# define NoDbgRegParms /*empty*/
#endif



#if defined(__GNUC__) && !defined(NeverInline)
# define NeverInline __attribute__ ((noinline))
#endif

#if !defined(NeverInline)
# define NeverInline /*empty*/
#endif

sqInt accessorDepthForPrimitiveIndex(sqInt primIndex);
usqInt argumentCountAddress(void);
void assertValidMachineCodeFrame(sqInt instrPtr);
void callForCogCompiledCodeCompaction(void);
void ceActivateFailingPrimitiveMethod(sqInt aPrimitiveMethod);
sqInt ceActiveContext(void);
sqInt ceBaseFrameReturn(sqInt returnValue);
sqInt ceCannotResume(void);
sqInt ceCheckForAndFollowForwardedPrimitiveState(void);
void ceCheckForInterrupts(void);
void ceCheckProfileTick(void);
sqInt ceContextinstVar(sqInt maybeContext, sqInt slotIndex);
sqInt ceContextinstVarvalue(sqInt maybeMarriedContext, sqInt slotIndex, sqInt anOop);
sqInt ceDynamicSuperSendtonumArgs(sqInt selector, sqInt rcvr, sqInt numArgs);
sqInt ceEnclosingObjectAt(sqInt level);
sqInt ceInterpretMethodFromPICreceiver(sqInt aMethodObj, sqInt rcvr);
sqInt ceMNUFromPICMNUMethodreceiver(sqInt aMethodObj, sqInt rcvr);
sqInt ceNewArraySlotSize(sqInt slotSize);
sqInt ceNonLocalReturn(sqInt returnValue);
sqInt ceReturnToInterpreter(sqInt anOop);
sqInt ceSendAborttonumArgs(sqInt selector, sqInt rcvr, sqInt numArgs);
sqInt ceSendFromInLineCacheMiss(CogMethod *cogMethodOrPIC);
sqInt ceSendMustBeBoolean(sqInt anObject);
sqInt ceSendsupertonumArgs(sqInt selector, sqInt superNormalBar, sqInt rcvr, sqInt numArgs);
void ceStackOverflow(sqInt contextSwitchIfNotNil);
void ceTraceBlockActivation(void);
void ceTraceLinkedSend(sqInt theReceiver);
void ceTraceStoreOfinto(sqInt aValue, sqInt anObject);
void checkAssertsEnabledInCoInterpreter(void);
void clearTraceLog(void);
CogMethod * cogMethodOf(sqInt aMethodOop);
void compilationBreakpointFor(sqInt selectorOop);
sqInt createClosureNumArgsnumCopiedstartpc(sqInt numArgs, sqInt numCopied, sqInt initialIP);
void dumpPrimTraceLog(void);
void dumpTraceLog(void);
void executeCogMethodfromLinkedSendWithReceiver(CogMethod *cogMethod, sqInt rcvr);
void executeCogMethodfromLinkedSendWithReceiverandCacheTag(CogMethod *cogMethod, sqInt rcvr, sqInt cacheTag);
sqInt flushExternalPrimitiveOf(sqInt methodObj);
usqInt framePointerAddress(void);
void (*functionPointerForCompiledMethodprimitiveIndex(sqInt methodObj, sqInt primIndex))(void) ;
sqInt getCheckAllocFiller(void);
void ifValidWriteBackStackPointersSaveTo(void *theCFP, void *theCSP, char **savedFPP, char **savedSPP);
sqInt implicitReceiverFormixinimplementing(sqInt rcvr, sqInt mixin, sqInt selector);
usqInt instructionPointerAddress(void);
usqInt interpretAddress(void);
sqInt isCogMethodReference(sqInt methodHeader);
sqInt lookupreceiver(sqInt selector, sqInt rcvr);
void markActiveMethodsAndReferents(void);
sqInt maxLookupNoMNUErrorCode(void);
void * methodCacheAddress(void);
sqInt methodHasCogMethod(sqInt aMethodOop);
sqInt methodNeedsLargeContext(sqInt methodObj);
sqInt methodShouldBeCogged(sqInt aMethodObj);
CogMethod * mframeHomeMethodExport(void);
CogMethod * mframeHomeMethod(char *theFP);
sqInt mMethodClass(void);
void mnuCompilationBreakpointFor(sqInt selectorOop);
usqInt newMethodAddress(void);
usqInt nextProfileTickAddress(void);
sqInt noAssertHeaderOf(sqInt methodPointer);
sqInt positive32BitIntegerFor(sqInt integerValue);
sqInt primErrTable(void);
usqInt primFailCodeAddress(void);
usqInt primitiveFailAddress(void);
sqInt primitivePropertyFlags(sqInt primIndex);
void * primTraceLogAddress(void);
usqInt primTraceLogIndexAddress(void);
void printCogMethod(CogMethod *cogMethod);
void printFrameWithSP(char *theFP, char *theSP);
void printMethodCacheFor(sqInt thing);
sqInt quickPrimitiveConstantFor(sqInt aQuickPrimitiveIndex);
int (*quickPrimitiveGeneratorFor(sqInt aQuickPrimitiveIndex))(void) ;
sqInt quickPrimitiveInstVarIndexFor(sqInt primIndex);
sqInt rawHeaderOf(sqInt methodPointer);
void rawHeaderOfput(sqInt methodOop, sqInt cogMethodOrMethodHeader);
void reportMinimumUnusedHeadroom(void);
sqInt specialSelectorNumArgs(sqInt index);
usqInt stackLimitAddress(void);
usqInt stackPointerAddress(void);
sqInt startPCOfMethodHeader(sqInt aCompiledMethodHeader);
sqInt startPCOrNilOfLiteralin(sqInt lit, sqInt aMethodObj);
void updateStackZoneReferencesToCompiledCodePreCompaction(void);
char * whereIs(sqInt anOop);
usqInt primitiveFunctionPointerAddress(void);
char * cStringOrNullFor(sqInt oop);
sqInt failed(void);
void primitiveClosureValueNoContextSwitch(void);
sqInt primitiveFail(void);
sqInt primitiveFailFor(sqInt reasonCode);
sqInt primitiveFailureCode(void);
sqInt signalNoResume(sqInt aSemaphore);
usqInt sizeOfAlienData(sqInt oop);
void * startOfAlienData(sqInt oop);
sqInt ceClassAtIndex(sqInt classIndex);
void ceSheduleScavenge(void);
void ensureNoForwardedLiteralsIn(sqInt aMethodObj);
usqInt freeStartAddress(void);
usqInt getScavengeThreshold(void);
sqInt isForwardedClassIndex(sqInt maybeClassIndex);
sqInt isImmediateClass(sqInt classObj);
sqInt isReallyYoungObject(sqInt objOop);
sqInt methodHeaderOf(sqInt methodObj);
sqLong nullHeaderForMachineCodeMethod(void);
usqInt scavengeThresholdAddress(void);
sqInt smallIntegerTag(void);
sqInt withoutForwardingOnandwithsendToCogit(sqInt obj1, sqInt obj2, sqInt aBool, sqInt (*selector)(sqInt,sqInt,sqInt));
sqInt byteSwapped(sqInt w);
sqInt isIntegerValue(sqInt intValue);
sqInt isMarked(sqInt objOop);
sqInt numTagBits(void);
usqInt smallObjectBytesForSlots(sqInt numSlots);
sqInt tagMask(void);
sqInt remember(sqInt objOop);
sqInt addGCRoot(sqInt *varLoc);
sqInt addressCouldBeObj(sqInt address);
sqInt addressCouldBeOop(sqInt address);
sqInt allocatePinnedSlots(sqInt nSlots);
sqInt arrayFormat(void);
sqInt becomewith(sqInt array1, sqInt array2);
void beRootIfOld(sqInt oop);
sqInt byteSizeOf(sqInt oop);
sqInt characterObjectOf(sqInt characterCode);
sqInt characterTag(void);
sqInt characterValueOf(sqInt oop);
sqInt checkedLongAt(sqInt byteAddress);
sqInt checkOkayOop(usqInt oop);
sqInt checkOopHasOkayClass(usqInt obj);
sqInt checkTraversableSortedFreeList(void);
sqInt classAtIndex(sqInt classIndex);
sqInt classIndexMask(void);
sqInt classIndexOf(sqInt objOop);
sqInt classOrNilAtIndex(sqInt classIndex);
sqInt classSmallInteger(void);
sqInt classTableMajorIndexShift(void);
sqInt classTableMinorIndexMask(void);
sqInt classTablePageSize(void);
sqInt classTableRootObj(void);
sqInt classTagForClass(sqInt classObj);
sqInt compactClassIndexOf(sqInt objOop);
void countMarkedAndUnmarkdObjects(sqInt printFlags);
sqInt eeInstantiateClassIndexformatnumSlots(sqInt knownClassIndex, sqInt objFormat, sqInt numSlots);
sqInt fetchByteofObject(sqInt byteIndex, sqInt objOop);
void findStringBeginningWith(char *aCString);
void findString(char *aCString);
sqInt firstByteFormat(void);
sqInt firstCompiledMethodFormat(void);
sqInt firstLongFormat(void);
sqInt firstShortFormat(void);
sqInt fixedFieldsFieldWidth(void);
sqInt fixedFieldsOfClassFormatMask(void);
sqInt followForwardedObjectFieldstoDepth(sqInt objOop, sqInt depth);
sqInt followForwarded(sqInt objOop);
sqInt formatMask(void);
sqInt formatOfClass(sqInt classPointer);
sqInt formatShift(void);
sqInt freeObject(sqInt objOop);
usqLong headerForSlotsformatclassIndex(sqInt numSlots, sqInt formatField, sqInt classIndex);
sqInt identityHashHalfWordMask(void);
sqInt indexablePointersFormat(void);
void inOrderPrintFreeTreeprintList(sqInt freeChunk, sqInt printNextList);
sqInt instanceSizeOf(sqInt classObj);
sqInt isArrayNonImm(sqInt oop);
sqInt isCharacterObject(sqInt oop);
sqInt isCharacterValue(sqInt anInteger);
sqInt isCompiledMethod(sqInt objOop);
sqInt isForwardedObjectClassIndexPun(void);
sqInt isForwarded(sqInt objOop);
sqInt isImmediate(sqInt oop);
sqInt isNonImmediate(sqInt oop);
sqInt isOldObject(sqInt objOop);
sqInt isOopCompiledMethod(sqInt oop);
sqInt isOopForwarded(sqInt oop);
sqInt isOopImmutable(sqInt oop);
sqInt isOopMutable(sqInt oop);
sqInt isPinned(sqInt objOop);
sqInt isReallyYoung(sqInt oop);
sqInt isUnambiguouslyForwarder(sqInt objOop);
sqInt isYoungObject(sqInt objOop);
sqInt isYoung(sqInt oop);
sqInt lastPointerOfWhileSwizzling(sqInt objOop);
sqInt lastPointerOf(sqInt objOop);
sqInt leakCheckBecome(void);
sqInt leakCheckFullGC(void);
sqInt leakCheckImageSegments(void);
sqInt leakCheckIncrementalGC(void);
sqInt leakCheckNewSpaceGC(void);
sqInt lengthOf(sqInt objOop);
sqInt literalCountOfMethodHeader(sqInt header);
sqInt literalCountOf(sqInt methodPointer);
void longPrintInstancesOf(sqInt aClassOop);
void longPrintInstancesWithClassIndex(sqInt classIndex);
void longPrintReferencesTo(sqInt anOop);
void markAndTrace(sqInt objOop);
sqInt maybeSplObj(sqInt index);
sqInt nextInSortedFreeListLinkgiven(sqInt freeChunk, sqInt prevFree);
sqInt nonIndexablePointerFormat(void);
sqInt numBytesOf(sqInt objOop);
sqInt numPointerSlotsOf(sqInt objOop);
sqInt numSlotsHalfShift(void);
sqInt numSlotsMask(void);
usqInt numSlotsOf(sqInt objOop);
sqInt numStrongSlotsOfWeakling(sqInt objOop);
sqInt objectAfter(sqInt objOop);
sqInt objectBefore(sqInt objOop);
sqInt obsoleteDontUseThisFetchWordofObject(sqInt fieldIndex, sqInt oop);
sqInt popRemappableOop(void);
sqInt primitiveErrorTable(void);
void printForwarders(void);
void printFreeChunks(void);
void printFreeChunk(sqInt freeChunk);
void printFreeListHeads(void);
void printFreeList(sqInt chunkOrIndex);
void printFreeTree(void);
void printInstancesOf(sqInt aClassOop);
void printInstancesWithClassIndex(sqInt classIndex);
void printInvalidClassTableEntries(void);
void printMethodReferencesTo(sqInt anOop);
void printObjectsFromto(sqInt startAddress, sqInt endAddress);
void printObjStack(sqInt objStack);
void printOopsFromto(sqInt startAddress, sqInt endAddress);
void printReferencesTo(sqInt anOop);
void pushRemappableOop(sqInt oop);
sqInt remapObj(sqInt objOop);
sqInt rememberedBitShift(void);
sqInt removeGCRoot(sqInt *varLoc);
void shortPrintObjectsFromto(sqInt startAddress, sqInt endAddress);
sqInt shouldRemapObj(sqInt objOop);
sqInt shouldRemapOop(sqInt oop);
sqInt splObj(sqInt index);
usqInt storeCheckBoundary(void);
sqInt storePointerUncheckedofObjectwithValue(sqInt fieldIndex, sqInt objOop, sqInt valuePointer);
sqInt stringForCString(const char *aCString);
void tenuringIncrementalGC(void);
sqInt topRemappableOop(void);
sqInt vmEndianness(void);
sqInt weakArrayFormat(void);
sqInt activeProcess(void);
sqInt argumentCountOfClosure(sqInt closurePointer);
sqInt argumentCountOfMethodHeader(sqInt header);
sqInt argumentCountOf(sqInt methodPointer);
sqInt canContextSwitchIfActivatingheader(sqInt theMethod, sqInt methodHeader);
sqInt checkAllAccessibleObjectsOkay(void);
sqInt checkOkayInterpreterObjects(sqInt writeBack);
sqInt copiedValueCountOfClosure(sqInt closurePointer);
sqInt doSignalSemaphoreWithIndex(sqInt index);
sqInt followLiteralofMethod(sqInt offset, sqInt methodPointer);
void (*functionPointerForinClass(sqInt primIdx,sqInt theClass))(void) ;
usqLong getNextWakeupUsecs(void);
sqInt * getStackPointer(void);
sqInt headerIndicatesAlternateBytecodeSet(sqInt methodHeader);
sqInt highBit(usqInt anUnsignedValue);
sqInt isFloatObject(sqInt oop);
sqInt isQuickPrimitiveIndex(sqInt anInteger);
sqInt isReadMediatedContextInstVarIndex(sqInt index);
sqInt isWriteMediatedContextInstVarIndex(sqInt index);
sqInt isKindOfClass(sqInt oop, sqInt aClass);
sqInt literalofMethod(sqInt offset, sqInt methodPointer);
void longPrintOop(sqInt oop);
sqInt longStoreBytecodeForHeader(sqInt methodHeader);
sqInt lookupSelectorinClass(sqInt selector, sqInt class);
sqInt methodClassAssociationOf(sqInt methodPointer);
sqInt methodClassOf(sqInt methodPointer);
sqInt methodPrimitiveIndex(void);
sqInt methodUsesAlternateBytecodeSet(sqInt aMethodObj);
sqInt penultimateLiteralOf(sqInt aMethodOop);
sqInt popStack(void);
sqInt primitiveIndexOfMethodheader(sqInt theMethod, sqInt methodHeader);
sqInt primitiveIndexOf(sqInt methodPointer);
void printAllStacks(void);
sqInt printCallStackOf(sqInt aContextOrProcessOrFrame);
void printChar(sqInt aByte);
void printHex(sqInt n);
void printLikelyImplementorsOfSelector(sqInt selector);
void printMethodCache(void);
void printMethodDictionaryOf(sqInt behavior);
void printMethodDictionary(sqInt dictionary);
void printProcessStack(sqInt aProcess);
sqInt printProcsOnList(sqInt procList);
sqInt printStackCallStackOf(sqInt frameOrContext);
void print(char *s);
void setBreakMNUSelector(char *aString);
void setBreakSelector(char *aString);
void setNextWakeupUsecs(usqLong value);
void shortPrintFrameAndNCallers(char *theFP, sqInt n);
sqInt sizeOfCallPrimitiveBytecode(sqInt methodHeader);
sqInt sizeOfLongStoreTempBytecode(sqInt methodHeader);
sqInt specialSelector(sqInt index);
unsigned long stackPositiveMachineIntegerValue(sqInt offset);
long stackSignedMachineIntegerValue(sqInt offset);
sqInt stackTop(void);
sqInt stackValue(sqInt offset);
sqInt startPCOfMethod(sqInt aCompiledMethod);
sqInt tempCountOf(sqInt methodPointer);
sqInt temporaryCountOfMethodHeader(sqInt header);


/*** Global Variables ***/
char * breakSelector;
sqInt breakSelectorLength ;
sqInt checkAllocFiller;
sqInt checkForLeaks;
sqInt deferDisplayUpdates;
sqInt desiredCogCodeSize;
sqInt desiredEdenBytes;
sqInt desiredNumStackPages;
sqInt extraVMMemory;
usqInt heapBase;
sqInt inIOProcessEvents;
struct VirtualMachine* interpreterProxy;
sqInt maxLiteralCountForCompile ;
sqInt minBackwardJumpCountForCompile ;
volatile int sendTrace;
void * showSurfaceFn;
sqInt suppressHeartbeatFlag;


/*** Macros ***/
#define compilationBreakpointisMNUCase(sel, len, isMNU) do { \
	if ((len) == (isMNU ? -breakSelectorLength : breakSelectorLength) \
	 && !strncmp((char *)((sel) + BaseHeaderSize), breakSelector, (isMNU ? -breakSelectorLength : breakSelectorLength))) { \
		suppressHeartbeatFlag = 1; \
		compilationBreakpointFor(sel); \
	} \
} while (0)
#define mnuCompilationBreakpoint(sel, len) do { \
	if ((len) == -breakSelectorLength \
	 && !strncmp((char *)((sel) + BaseHeaderSize), breakSelector, -breakSelectorLength)) { \
		suppressHeartbeatFlag = 1; \
		compilationBreakpointFor(sel); \
	} \
} while (0)
#define startOfMemory() heapBase
#define shiftForWord() 2
#define wordSize() 4

