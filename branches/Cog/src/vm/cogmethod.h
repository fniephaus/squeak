/* Automatically generated by
	CCodeGenerator VMMaker.oscog-eem.148 uuid: bb53aa60-4173-48ed-874b-dc0ff5a48992
 */

typedef struct {
	unsigned short	homeOffset;
	unsigned short	startpc;
	unsigned		cmNumArgs : 8;
	unsigned		cmType : 3;
	unsigned		cmRefersToYoung : 1;
	unsigned		cpicHasMNUCase : 1;
	unsigned		cmUsageCount : 3;
	unsigned		stackCheckOffset : 16;
 } CogBlockMethod;

typedef struct {
	sqInt	objectHeader;
	unsigned		cmNumArgs : 8;
	unsigned		cmType : 3;
	unsigned		cmRefersToYoung : 1;
	unsigned		cpicHasMNUCase : 1;
	unsigned		cmUsageCount : 3;
	unsigned		stackCheckOffset : 16;
	unsigned short	blockSize;
	unsigned short	blockEntryOffset;
	sqInt	methodObject;
	sqInt	methodHeader;
	sqInt	selector;
 } CogMethod;
