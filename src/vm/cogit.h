/* Automatically generated by
	CCodeGenerator * VMMaker.oscog-eem.112 uuid: efadaaf2-156f-49df-95df-ea610c02b6b2
 */


/*** Function Prototypes ***/
sqInt abortOffset(void);
void addCogMethodsToHeapMap(void);
sqInt bytecodePCForstartBcpcin(sqInt mcpc, sqInt startbcpc, CogBlockMethod *cogMethod);
sqInt canLinkToYoungClasses(void);
extern void (*ceCaptureCStackPointers)();
sqInt ceCPICMissreceiver(CogMethod *cPIC, sqInt receiver);
extern void (*ceEnter0ArgsPIC)();
extern void (*ceEnter1ArgsPIC)();
extern void (*ceEnter2ArgsPIC)();
extern void (*ceEnterCogCodePopReceiverAndClassRegs)();
extern void (*ceEnterCogCodePopReceiverArg0Regs)();
extern void (*ceEnterCogCodePopReceiverArg1Arg0Regs)();
extern void (*ceEnterCogCodePopReceiverReg)();
sqInt ceSICMiss(sqInt receiver);
void checkAssertsEnabledInCogit(void);
sqInt checkIntegrityOfObjectReferencesInCode(sqInt fullGCFlag);
char * codeEntryFor(char *address);
char * codeEntryNameFor(char *address);
sqInt cogCodeBase(void);
sqInt cogCodeConstituents(void);
void cogitPostGCAction(void);
CogMethod * cogselector(sqInt aMethodObj, sqInt aSelectorOop);
void compactCogCompiledCode(void);
void enterCogCodePopReceiver(void);
void enterCogCodePopReceiverAndClassRegs(void);
void enterCogCodePopReceiverArg0Regs(void);
void enterCogCodePopReceiverArg1Arg0Regs(void);
CogBlockMethod * findEnclosingMethodForinHomeMethod(sqInt mcpc, CogMethod *cogMethod);
CogBlockMethod * findMethodForStartBcpcinHomeMethod(sqInt startbcpc, CogMethod *cogMethod);
sqInt genQuickReturnConst(void);
sqInt genQuickReturnInstVar(void);
sqInt genQuickReturnSelf(void);
void initializeCodeZoneFromupTo(sqInt startAddress, sqInt endAddress);
sqInt interpretOffset(void);
sqInt isPCWithinMethodZone(char *address);
sqInt isSendReturnPC(sqInt retpc);
void linkSendAtintooffsetreceiver(sqInt callSiteReturnAddress, CogMethod *sendingMethod, CogMethod *targetMethod, sqInt entryOffset, sqInt receiver);
void mapObjectReferencesInMachineCode(sqInt inFullGC);
void markAndTraceObjectsOrFreeMachineCode(sqInt inFullGC);
void markMethodAndReferents(CogBlockMethod *aCogMethod);
usqInt maxCogMethodAddress(void);
sqInt mcPCForstartBcpcin(sqInt bcpc, sqInt startbcpc, CogBlockMethod *cogMethod);
CogMethod * methodFor(void *address);
sqInt minCogMethodAddress(void);
sqInt mnuOffset(void);
sqInt numMethodsOfType(sqInt cogMethodType);
sqInt patchToOpenPICFornumArgsreceiver(sqInt selector, sqInt numArgs, sqInt receiver);
sqInt pcisWithinMethod(char *address, CogMethod *cogMethod);
void printCogMethodFor(void *address);
void printCogMethods(void);
void printCogMethodsOfType(sqInt cmType);
void printCogMethodsWithSelector(sqInt selectorOop);
void printTrampolineTable(void);
void recordCallOffsetInof(CogMethod *cogMethod, void *callLabelArg);
sqInt recordPrimTraceFunc(void);
void rewritePrimInvocationInto(CogMethod *cogMethod, void (*primFunctionPointer)(void));
void setBreakMethod(sqInt anObj);
void setPostCompileHook(void (*aFunction)(CogMethod *, void *));
void setSelectorOfto(CogMethod *cogMethod, sqInt aSelectorOop);
sqInt stackPageHeadroomBytes(void);
sqInt traceLinkedSendOffset(void);
void unlinkAllSends(void);
void unlinkSendsOf(sqInt selector);
void unlinkSendsToFree(void);
void unlinkSendsTo(CogMethod *targetMethod);
void voidCogCompiledCode(void);


/*** Global Variables ***/
sqInt blockNoContextSwitchOffset;
sqInt breakPC;
void * CFramePointer;
void * CStackPointer;
sqInt ceBaseFrameReturnTrampoline;
sqInt ceCannotResumeTrampoline;
void (*ceCaptureCStackPointers)(void);
sqInt ceCheckForInterruptTrampoline;
void (*ceEnter0ArgsPIC)(void);
void (*ceEnter1ArgsPIC)(void);
void (*ceEnter2ArgsPIC)(void);
void (*ceEnterCogCodePopReceiverAndClassRegs)(void);
void (*ceEnterCogCodePopReceiverArg0Regs)(void);
void (*ceEnterCogCodePopReceiverArg1Arg0Regs)(void);
void (*ceEnterCogCodePopReceiverReg)(void);
unsigned long (*ceGetSP)(void);
sqInt ceReturnToInterpreterTrampoline;
unsigned long (*ceTryLockVMOwner)(void);
void (*ceUnlockVMOwner)(void);
sqInt cFramePointerInUse;
sqInt cmEntryOffset;
sqInt cmNoCheckEntryOffset;
unsigned long debugPrimCallStackOffset;
void (*realCEEnterCogCodePopReceiverAndClassRegs)(void);
void (*realCEEnterCogCodePopReceiverArg0Regs)(void);
void (*realCEEnterCogCodePopReceiverArg1Arg0Regs)(void);
void (*realCEEnterCogCodePopReceiverReg)(void);
int traceLinkedSends ;
sqInt traceStores;


/*** Macros ***/
#define breakOnImplicitReceiver() (traceLinkedSends & 16)
#define ceBaseFrameReturnPC() ceBaseFrameReturnTrampoline
#define ceCannotResumePC() ((usqInt)ceCannotResumeTrampoline)
#define ceReturnToInterpreterPC() ((usqInt)ceReturnToInterpreterTrampoline)
#define dynSuperEntryOffset() cmDynSuperEntryOffset
#define entryOffset() cmEntryOffset
#define getCFramePointer() CFramePointer
#define getCStackPointer() CStackPointer
#define noCheckEntryOffset() cmNoCheckEntryOffset
#define noContextSwitchBlockEntryOffset() blockNoContextSwitchOffset
#define numRegArgs() 1
#define printOnTrace() (traceLinkedSends & 8)
#define recordEventTrace() (traceLinkedSends & 4)
#define recordPrimTrace() (traceLinkedSends & 2)
#define recordSendTrace() (traceLinkedSends & 1)
#define setCFramePointer(theFP) (CFramePointer = (void *)(theFP))
#define setCStackPointer(theSP) (CStackPointer = (void *)(theSP))
#define tryLockVMOwner() (ceTryLockVMOwner() != 0)
#define unlockVMOwner() ceUnlockVMOwner()

