#define	gprCount		( 0)
#define	fprCount		( 4)
#define stackIndex		( 8)
#define structReturnValue	(12)
#define longReturnValue		(16)
#define floatReturnValue	(24)
#define gprs			(32)
#define fprs			(32 + 4*GPR_MAX)
#define stack			(32 + 4*GPR_MAX + 8*FPR_MAX)
