/****************************************************************************
*   PROJECT: Squeak foreign function interface
*   FILE:    sqWin32FFI.c
*   CONTENT: Win32 support for the foreign function interface
*
*   AUTHOR:  Andreas Raab (ar)
*   ADDRESS: Walt Disney Imagineering, Glendale, CA
*   EMAIL:   andreasr@wdi.disney.com
*   RCSID:   $Id$
*
*   NOTES:
*
*****************************************************************************/

#include <assert.h>
#include "SqueakFFIPrims_imports.h"
#include "sqFFI.h"

#define primitiveFail() vmFunction(primitiveFail)(PLUGIN_IPARAM);

#ifdef _MSC_VER
#define LONGLONG __int64
#endif
#ifdef __GNUC__
#define LONGLONG long long int
#endif

/* Max stack size */
#define FFI_MAX_ARGS 128

typedef struct FFI_State {
	int   ffiArgIndex;				// + 0
	volatile int      intReturnValue;  // + 4
	volatile double   floatReturnValue; // + 8
	volatile int      intReturnValue2; // + 16

	int   ffiArgs[FFI_MAX_ARGS]; // + 20
	/* The stack pointer while filling the stack */
	/* The area for temporarily allocated strings */
	char *ffiTempStrings[FFI_MAX_ARGS];
	/* The number of temporarily allocated strings */
	int   ffiTempStringCount;

	/* The return values for calls */
	void * structReturnValue;
} FFI_State;

static int ffiStateId;

#define FFI_STATE(name) (pstate->name)
#define DECLARE_FFI_STATE() FFI_State * pstate = vmFunction(getAttachedStateBuffer)(PLUGIN_IPARAM_COMMA ffiStateId)


#define ARG_CHECK() if(FFI_STATE(ffiArgIndex) >= FFI_MAX_ARGS) return primitiveFail();
#define ARG_PUSH(value) { ARG_CHECK(); FFI_STATE(ffiArgs)[FFI_STATE(ffiArgIndex)] = value; FFI_STATE(ffiArgIndex)++; }

/*****************************************************************************/
/*****************************************************************************/

int initialiseFFIModule(void)
{
	int success;
	FFI_State pstate;
	ffiStateId = vmFunction(attachStateBufferinitializeFnfinalizeFn)(sizeof(FFI_State),0,0);
	if (!ffiStateId) return 0;

	success = ( (void*)&pstate.ffiArgIndex == (void*)&pstate )
		&& ( ( (unsigned long)&pstate.intReturnValue - (unsigned long)&pstate ) == 4 )
		&& ( ( (unsigned long)&pstate.intReturnValue2 - (unsigned long)&pstate ) == 16 )
		&& ( ( (unsigned long)&pstate.floatReturnValue - (unsigned long)&pstate ) == 8 )
		&& ( ( (unsigned long)&pstate.ffiArgs[0] - (unsigned long)&pstate ) == 20 );

	return success;
}

int shutdownFFIModule(void)
{
	if (ffiStateId)
	{
		vmFunction(releaseAttachedState)(ffiStateId);
		ffiStateId = 0;
	}
}

/*  ffiInitialize:
	Announce that the VM is about to do an external function call. */
int ffiInitialize(PLUGIN_IARG)
{
	DECLARE_FFI_STATE();
	FFI_STATE(ffiArgIndex) = 0;
	FFI_STATE(ffiTempStringCount) = 0;

	return 1;
}


/*  ffiSupportsCallingConvention:
	Return true if the support code supports the given calling convention. */
int ffiSupportsCallingConvention(int callType)
{
	if(callType == FFICallTypeCDecl) return 1;
	if(callType == FFICallTypeApi) return 1;
	return 0;
}

int ffiAlloc(int byteSize)
{
	return (int) malloc(byteSize);
}

int ffiFree(int ptr)
{
	if(ptr) free((void*)ptr);
	return 1;
}

/*****************************************************************************/
/*****************************************************************************/

int ffiPushSignedChar(PLUGIN_IARG_COMMA int value) 
{ 
	DECLARE_FFI_STATE();
	ARG_PUSH(value); 
	return 1; 
}

int ffiPushUnsignedChar(PLUGIN_IARG_COMMA int value) 
{ 
	DECLARE_FFI_STATE();
	ARG_PUSH(value); 
	return 1; 
}

int ffiPushSignedByte(PLUGIN_IARG_COMMA int value) 
{ 
	DECLARE_FFI_STATE();
	ARG_PUSH(value); 
	return 1; 
}

int ffiPushUnsignedByte(PLUGIN_IARG_COMMA int value) 
{ 
	DECLARE_FFI_STATE();
	ARG_PUSH(value); 
	return 1; 
}

int ffiPushSignedShort(PLUGIN_IARG_COMMA int value) 
{ 
	DECLARE_FFI_STATE();
	ARG_PUSH(value); 
	return 1; 
}

int ffiPushUnsignedShort(PLUGIN_IARG_COMMA int value) 
{ 
	DECLARE_FFI_STATE();
	ARG_PUSH(value); 
	return 1; 
}

int ffiPushSignedInt(PLUGIN_IARG_COMMA int value) 
{ 
	DECLARE_FFI_STATE();
	ARG_PUSH(value); 
	return 1; 
}

int ffiPushUnsignedInt(PLUGIN_IARG_COMMA int value) 
{ 
	DECLARE_FFI_STATE();
	ARG_PUSH(value); 
	return 1; 
}

int ffiPushSignedLongLong(PLUGIN_IARG_COMMA int lowWord, int highWord) 
{ 
	DECLARE_FFI_STATE();
	ARG_PUSH(lowWord); 
	ARG_PUSH(highWord); 
	return 1; 
}

int ffiPushUnsignedLongLong(PLUGIN_IARG_COMMA int lowWord, int highWord) 
{ 
	DECLARE_FFI_STATE();
	ARG_PUSH(lowWord); 
	ARG_PUSH(highWord); 
	return 1; 
}

int ffiPushSingleFloat(PLUGIN_IARG_COMMA double value)
{
	DECLARE_FFI_STATE();
	float floatValue;
	floatValue = (float) value;
	ARG_PUSH(*(int*)(&floatValue));
	return 1;
}

int ffiPushDoubleFloat(PLUGIN_IARG_COMMA double value)
{
	DECLARE_FFI_STATE();
	ARG_PUSH(((int*)(&value))[0]);
	ARG_PUSH(((int*)(&value))[1]);
	return 1;
}

int ffiPushStructureOfLength(PLUGIN_IARG_COMMA int pointer, int* structSpec, int structSize)
{
	DECLARE_FFI_STATE();
	int nItems, i;
	nItems = ((*structSpec & FFIStructSizeMask) + 3) / 4;
	if(pointer == 0) 
		return primitiveFail();
	for(i=0; i < nItems;i++)
		ARG_PUSH(((int*)pointer)[i]);
	return 1;
}

int ffiPushPointer(PLUGIN_IARG_COMMA int pointer)
{
	DECLARE_FFI_STATE();
	ARG_PUSH(pointer);
	return 1;
}

int ffiPushStringOfLength(PLUGIN_IARG_COMMA int srcIndex, int length)
{
	DECLARE_FFI_STATE();
	char *ptr;
	ARG_CHECK(); /* fail before allocating */
	ptr = (char*) malloc(length+1);
	if(!ptr) return primitiveFail();
	memcpy(ptr, (void*)srcIndex, length);
	ptr[length] = 0;
	FFI_STATE(ffiTempStrings)[FFI_STATE(ffiTempStringCount)++] = ptr;
	ARG_PUSH((int)ptr);
	return 1;
}

/*****************************************************************************/
/*****************************************************************************/

/*  ffiCanReturn:
	Return true if the support code can return the given type. */
int ffiCanReturn(PLUGIN_IARG_COMMA int *structSpec, int specSize)
{
	DECLARE_FFI_STATE();
	int header = *structSpec;
	if(header & FFIFlagPointer) return 1;
	if(header & FFIFlagStructure) {
		int structSize = header & FFIStructSizeMask;
		if(structSize > 8) {
			FFI_STATE(structReturnValue) = malloc(structSize);
			if(!FFI_STATE(structReturnValue)) return 0;
			ARG_PUSH((int)FFI_STATE(structReturnValue));
		}
	}
	return 1;
}

/*  ffiReturnFloatValue:
	Return the value from a previous ffi call with float return type. */
double ffiReturnFloatValue(PLUGIN_IARG)
{
	DECLARE_FFI_STATE();
	return FFI_STATE(floatReturnValue);
}

/*  ffiLongLongResultLow:
	Return the low 32bit from the 64bit result of a call to an external function */
int ffiLongLongResultLow(PLUGIN_IARG)
{
	DECLARE_FFI_STATE();
	return FFI_STATE(intReturnValue);
}

/*  ffiLongLongResultHigh:
	Return the high 32bit from the 64bit result of a call to an external function */
int ffiLongLongResultHigh(PLUGIN_IARG)
{
	DECLARE_FFI_STATE();
	return FFI_STATE(intReturnValue2);
}

/*  ffiStoreStructure:
	Store the structure result of a previous ffi call into the given address.
	Note: Since the ST allocator always allocates multiples of 32bit we can
	use the atomic types for storing <= 64bit result structures. */
int ffiStoreStructure(PLUGIN_IARG_COMMA int address, int structSize)
{
	DECLARE_FFI_STATE();
	if(structSize <= 4) {
		*(int*)address = FFI_STATE(intReturnValue);
		return 1;
	}
	if(structSize <= 8) {
		*(int*)address = FFI_STATE(intReturnValue);
		*(int*)(address+4) = FFI_STATE(intReturnValue2);
		return 1;
	}
	/* assume pointer to hidden structure */
	memcpy((void*)address, (void*) FFI_STATE(structReturnValue), structSize);
	return 1;
}

/*  ffiCleanup:
	Cleanup after a foreign function call has completed. */
int ffiCleanup(PLUGIN_IARG)
{
	DECLARE_FFI_STATE();
	int i;
	for(i=0; i<FFI_STATE(ffiTempStringCount); i++)
		free(FFI_STATE(ffiTempStrings)[i]);
	FFI_STATE(ffiTempStringCount) = 0;
	if(FFI_STATE(structReturnValue)) {
		free(FFI_STATE(structReturnValue));
		FFI_STATE(structReturnValue) = NULL;
	}
	return 1;
}

/*****************************************************************************/
/*****************************************************************************/

/*  ffiCallAddress:
	Perform the actual function call. */
int ffiCallAddress(FFI_State * state, int fn)
{
#ifdef __GNUC__
	asm("\
		pushl %%ebx; \
		pushl %%edx; \
		pushl %%ecx; \
		pushl %%edi; \
		pushl %%esi; \
		pushl %%ebp; \
		movl %%esp, %%ebp; \
		movl (%%ecx), %%eax; /* ffiArgIndex */ \
		shll $2, %%eax; \
		subl %%eax, %%esp; \
		movl %%esp, %%edi; \
		leal 20(%%ecx), %%esi; /* ffiArgs */ \
		shrl $2, %%eax; \
		movl %%eax, %%ecx; \
		cld; \
		rep movsl; \
		call *%%ebx; \
		movl %%ebp, %%esp; \
		popl %%ebp; \
		popl %%esi; \
		popl %%edi; \
		popl %%ecx; \
		movl %%eax, 4(%%ecx); /* intReturnValue */ \
		movl %%edx, 16(%%ecx); /* intReturnValue2 */ \
		fstpl 8(%%ecx); /* floatReturnValue */ \
		popl %%edx; \
		popl %%ebx; \
		": /* no outputs */ : "ebx" (fn), "ecx" (state) : "eax" /* clobbered registers */);
		/* done */
#else
#error "Compiler not supported"
#endif
	return state->intReturnValue;
}

int ffiCallAddressOfWithPointerReturn(PLUGIN_IARG_COMMA int fn, int callType)
{
	DECLARE_FFI_STATE();
	return ffiCallAddress(pstate,fn);
}
int ffiCallAddressOfWithStructReturn(PLUGIN_IARG_COMMA int fn, int callType, int* structSpec, int specSize)
{
	DECLARE_FFI_STATE();
	return ffiCallAddress(pstate,fn);
}

int ffiCallAddressOfWithReturnType(PLUGIN_IARG_COMMA int fn, int callType, int typeSpec)
{
	DECLARE_FFI_STATE();
	return ffiCallAddress(pstate,fn);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/************ Test functions for the foreign function interface **************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifndef NO_FFI_TEST
typedef struct ffiTestPoint2 {
	int x;
	int y;
} ffiTestPoint2;

typedef struct ffiTestPoint4 {
	int x;
	int y;
	int z;
	int w;
} ffiTestPoint4;

#pragma export on
EXPORT(char) ffiTestChars(char c1, char c2, char c3, char c4);
EXPORT(short) ffiTestShorts(short c1, short c2, short c3, short c4);
EXPORT(int) ffiTestInts(int c1, int c2, int c3, int c4);
EXPORT(float) ffiTestFloats(float f1, float f2);
EXPORT(double) ffiTestDoubles(double d1, double d2);
EXPORT(char *) ffiPrintString(char *string);
EXPORT(ffiTestPoint2) ffiTestStruct64(ffiTestPoint2 pt1, ffiTestPoint2 pt2);
EXPORT(ffiTestPoint4) ffiTestStructBig(ffiTestPoint4 pt1, ffiTestPoint4 pt2);
EXPORT(ffiTestPoint4*) ffiTestPointers(ffiTestPoint4 *pt1, ffiTestPoint4 *pt2);
EXPORT(LONGLONG) ffiTestLongLong(LONGLONG i1, LONGLONG i2);
#pragma export off


/* test passing characters */
EXPORT(char) ffiTestChars(char c1, char c2, char c3, char c4) {
	printf("4 characters came in as\nc1 = %c (%x)\nc2 = %c (%x)\nc3 = %c (%x)\nc4 = %c (%x)\n", c1, c1, c2, c2, c3, c3, c4, c4);
	return c1+c2;
}

/* test passing shorts */
EXPORT(short) ffiTestShorts(short c1, short c2, short c3, short c4) {
	printf("4 shorts came in as\ns1 = %d (%x)\ns2 = %d (%x)\ns3 = %d (%x)\ns4 = %d (%x)\n", c1, c1, c2, c2, c3, c3, c4, c4);
	return c1+c2;
}

/* test passing ints */
EXPORT(int) ffiTestInts(int c1, int c2, int c3, int c4) {
	printf("4 ints came in as\ni1 = %d (%x)\ni2 = %d (%x)\ni3 = %d (%x)\ni4 = %d (%x)\n", c1, c1, c2, c2, c3, c3, c4, c4);
	return c1+c2;
}

/* test passing and returning floats */
EXPORT(float) ffiTestFloats(float f1, float f2) {
	printf("The two floats are %f and %f\n", f1, f2);
	return (float) (f1 + f2);
}

/* test passing and returning doubles */
EXPORT(double) ffiTestDoubles(double d1, double d2) {
	printf("The two floats are %f and %f\n", (float)d1, (float)d2);
	return d1+d2;
}

/* test passing and returning strings */
EXPORT(char*) ffiPrintString(char *string) {
	printf("%s\n", string);
	return string;
}

/* test passing and returning 64bit structures */
EXPORT(ffiTestPoint2) ffiTestStruct64(ffiTestPoint2 pt1, ffiTestPoint2 pt2) {
	ffiTestPoint2 result;
	printf("pt1.x = %d\npt1.y = %d\npt2.x = %d\npt2.y = %d\n",
			pt1.x, pt1.y, pt2.x, pt2.y);
	result.x = pt1.x + pt2.x;
	result.y = pt1.y + pt2.y;
	return result;
}

/* test passing and returning large structures */
EXPORT(ffiTestPoint4) ffiTestStructBig(ffiTestPoint4 pt1, ffiTestPoint4 pt2) {
	ffiTestPoint4 result;
	printf("pt1.x = %d\npt1.y = %d\npt1.z = %d\npt1.w = %d\n",
			pt1.x, pt1.y, pt1.z, pt1.w);
	printf("pt2.x = %d\npt2.y = %d\npt2.z = %d\npt2.w = %d\n",
			pt2.x, pt2.y, pt2.z, pt2.w);
	result.x = pt1.x + pt2.x;
	result.y = pt1.y + pt2.y;
	result.z = pt1.z + pt2.z;
	result.w = pt1.w + pt2.w;
	return result;
}

/* test passing and returning pointers */
EXPORT(ffiTestPoint4*) ffiTestPointers(ffiTestPoint4 *pt1, ffiTestPoint4 *pt2) {
	ffiTestPoint4 *result;
	printf("pt1.x = %d\npt1.y = %d\npt1.z = %d\npt1.w = %d\n",
			pt1->x, pt1->y, pt1->z, pt1->w);
	printf("pt2.x = %d\npt2.y = %d\npt2.z = %d\npt2.w = %d\n",
			pt2->x, pt2->y, pt2->z, pt2->w);
	result = (ffiTestPoint4*) malloc(sizeof(ffiTestPoint4));
	result->x = pt1->x + pt2->x;
	result->y = pt1->y + pt2->y;
	result->z = pt1->z + pt2->z;
	result->w = pt1->w + pt2->w;
	return result;
}

/* test passing and returning longlongs */
EXPORT(LONGLONG) ffiTestLongLong(LONGLONG i1, LONGLONG i2) {
	return i1 + i2;
}

#endif /* NO_FFI_TEST */
