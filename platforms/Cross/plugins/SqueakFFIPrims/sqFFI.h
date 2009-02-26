/****************************************************************************
*   PROJECT: Squeak foreign function interface
*   FILE:    sqFFI.h
*   CONTENT: Declarations for the foreign function interface
*
*   AUTHOR:  Andreas Raab (ar)
*   ADDRESS: Walt Disney Imagineering, Glendale, CA
*   EMAIL:   andreasr@wdi.disney.com
*   RCSID:   $Id$
*
*   NOTES:
*
*****************************************************************************/
#ifndef SQ_FFI_H
#define SQ_FFI_H

/* Calling conventions */
#define FFICallTypeCDecl 0
#define FFICallTypeApi 1

/* Atomic types */
#define FFITypeVoid 0
#define FFITypeBool 1
#define FFITypeUnsignedByte 2
#define FFITypeSignedByte 3
#define FFITypeUnsignedShort 4
#define FFITypeSignedShort 5
#define FFITypeUnsignedInt 6
#define FFITypeSignedInt 7
#define FFITypeUnsignedLongLong 8
#define FFITypeSignedLongLong 9
#define FFITypeUnsignedChar 10
#define FFITypeSignedChar 11
#define FFITypeSingleFloat 12
#define FFITypeDoubleFloat 13

/* Shift and mask for atomic types */
#define FFIAtomicTypeShift 24
#define FFIAtomicTypeMask 251658240

/* Type flags */
#define FFIFlagPointer 131072
#define FFIFlagStructure 65536
#define FFIFlagAtomic 262144

/* Size mask */
#define FFIStructSizeMask 65535

/* error constants */
#define FFINoCalloutAvailable -1
#define FFIErrorGenericError 0
#define FFIErrorNotFunction 1
#define FFIErrorBadArgs 2
#define FFIErrorBadArg 3
#define FFIErrorIntAsPointer 4
#define FFIErrorBadAtomicType 5
#define FFIErrorCoercionFailed 6
#define FFIErrorWrongType 7
#define FFIErrorStructSize 8
#define FFIErrorCallType 9
#define FFIErrorBadReturn 10
#define FFIErrorBadAddress 11
#define FFIErrorNoModule 12
#define FFIErrorAddressNotFound 13
#define FFIErrorAttemptToPassVoid 14
#define FFIErrorModuleNotFound 15
#define FFIErrorBadExternalLibrary 16
#define FFIErrorBadExternalFunction 17
#define FFIErrorInvalidPointer 18

/* init FFI module */
int initialiseFFIModule(void);
int shutdownFFIModule(void);

/* Announce a coming FFI call */
int ffiInitialize _iarg();

/* cleanup */
int ffiCleanup _iarg();

/* Allocate/free external memory */
int ffiAlloc(int byteSize);
int ffiFree(int ptr);

/* general <=32bit integer loads */
int ffiPushSignedByte _iargs(int value);
int ffiPushUnsignedByte _iargs(int value);
int ffiPushSignedShort _iargs(int value);
int ffiPushUnsignedShort _iargs(int value);
int ffiPushSignedInt _iargs(int value);
int ffiPushUnsignedInt _iargs(int value);

/* 64bit integer loads */
int ffiPushSignedLongLong _iargs(int lowWord, int highWord);
int ffiPushUnsignedLongLong _iargs(int lowWord, int highWord);
/* 64bit integer returns */
int ffiLongLongResultLow _iarg();
int ffiLongLongResultHigh _iarg();

/* special <=32bit loads */
int ffiPushSignedChar _iargs(int value);
int ffiPushUnsignedChar _iargs(int value);

/* float loads */
int ffiPushSingleFloat _iargs(double value);
int ffiPushDoubleFloat _iargs(double value);

/* structure loads */
int ffiPushStructureOfLength _iargs(int pointer, int* structSpec, int specSize);

/* pointer loads */
int ffiPushPointer _iargs(int pointer);

/* string loads */
int ffiPushStringOfLength _iargs(int srcIndex, int length);

/* return true if calling convention is supported */
int ffiSupportsCallingConvention(int callType);

/* return true if these types can be returned */
int ffiCanReturn _iargs(int* structSpec, int specSize);

/* call the appropriate function w/ the given return type */
int ffiCallAddressOfWithPointerReturn _iargs(int fn, int callType);
int ffiCallAddressOfWithStructReturn _iargs(int fn, int callType, 
				     int* structSpec, int specSize);
int ffiCallAddressOfWithReturnType _iargs(int fn, int callType, int typeSpec);

/* store the structure result of a previous call */
int ffiStoreStructure _iargs(int address, int structSize);

/* return the float value from a previous call */
double ffiReturnFloatValue _iarg();


#endif /* SQ_FFI_H */
