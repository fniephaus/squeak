/* Spread Plugin common functions */

/* Spread header */
#include <sp.h>

struct SpreadConnection
{
	int mbox;
	int semaIndex;
	int sessionID;
};

typedef struct SpreadConnection SpreadConnection;

typedef char groups_name[MAX_GROUP_NAME];
typedef char const cgroups_name[MAX_GROUP_NAME];

int sqSpreadInit(void);
int sqSpreadShutdown(void);

/* all of the following functions return 0 on success, or error code on failure */

/* returns error code, fills in s and groupBuf */
int sqSpreadConnect(SpreadConnection *s,	/*OUT*/
		const char *daemonName,
		int daemonNameSize,
		const char *privateName,
		int privateNameSize,
		int wantsGroupMsgs,
		char groupBuf[MAX_GROUP_NAME],	/*OUT*/
		int semaIndex);

/* returns error code */
int sqSpreadDisconnect(SpreadConnection * s);

/* returns msg length (0..n) or error code (<0) */
int sqSpreadReceive(SpreadConnection * s,
		int *serviceType,	/*OUT*/
		char *sender,		/*OUT*/
		int maxGroups,
		int *numberOfGroups,	/*OUT*/
		char *groups,		/*OUT*//* numberOfGroups*MAX_GROUP_NAME long */
		int16 *messageType,	/*OUT*/
		int *endianMismatch,	/*OUT*/
		int messageLength,
		char *message,		/*OUT*/
		int drop);

int sqSpreadError(int n);

/* returns number of bytes sent or error code (<0) */
/* service_type is one of:
	UNRELIABLE_MESS 
	RELIABLE_MESS 
	FIFO_MESS 
	CAUSAL_MESS 
	AGREED_MESS 
	SAFE_MESS
   possibly or'd with SELF_DISCARD if desired.
*/
int sqSpreadMultigroupMulticast(SpreadConnection *s,
		int service_type,
		int num_groups,
		char const *groups,
		int mess_type,
		int mess_len,
		char const *mess);

/* return error (<0), nothing (0), or bytes available (>0) */
int sqSpreadPoll(SpreadConnection const *s);

/* return 0 on success, or error (<0) */
int sqSpreadJoin(SpreadConnection *s, char const* group, int groupLength);

/* return 0 on success, or error (<0) */
int sqSpreadLeave(SpreadConnection *s, char const* group, int groupLength);

/* return 1 if connection is still valid */
int sqSpreadIsValid(SpreadConnection const *s);
