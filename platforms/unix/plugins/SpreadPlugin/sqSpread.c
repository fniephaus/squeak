/*
 * Spread Plugin for Squeak
 * by Ned Konz <ned@bike-nomad.com>
 * Mon Jul 15 21:45:46 PDT 2002
 * License: Squeak's
 *
 * Currently this is only Unix specific because it uses aio.h.
 * If the async file support were to be generalized it could
 * easily work cross-platform.
 */
#include "SpreadPlugin.h"
#include "aio.h"
#include "sqVirtualMachine.h"

#define _GNU_SOURCE		/* required for strndup */
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#if defined(DEBUG)
#	define dprintf(stuff) fprintf stuff
#else
#	define dprintf(stuff)
#endif

#define max(a,b) (((a) > (b)) ? (a) : (b))

/* lifted from sqUnixAsynchFile.c */
static int    sessionID = 0;
static fd_set fds;
static int    nfd = 0;	/* maximum file descriptor used */

static struct VirtualMachine *vm= 0;

#define isValid(f)	(f != NULL && f->sessionID == sessionID)
#define validate(f)	if (!isValid(f)) return vm->primitiveFail()

/* This is called when the file descriptor is ready to read.
 * It signals the semaphore.
 */
static void dataReadyCallback(int fd, void *clientData, int flag)
{
	int semaIndex = (int)clientData;	/* I know... */
	if (semaIndex > 0)
		signalSemaphoreWithIndex(semaIndex);
	dprintf((stderr, "dataReady on %d sem %d mask %d\n", fd, semaIndex, flag));
	dprintf((stderr, "Poll returns %d\n", SP_poll(fd)));

	/* aioHandle will be called again at the next receive. */
}

/* Close the file handle, and stop aio */
static void closeMbox(SpreadConnection * s)
{
	aioDisable(s->mbox);
	FD_CLR(s->mbox, &fds);
	s->sessionID = 0;
	s->semaIndex = 0;
}

/* Module initialization */
int sqSpreadInit(void)
{
	vm= sqGetInterpreterProxy();
	sessionID= clock() + time(0);
	FD_ZERO(&fds);
	nfd= 0;
    return 1;
}

/* Module de-initialization */
int sqSpreadShutdown(void)
{
	/* protect against calling stale aio handlers */
	int i;
	for (i= 0; i < nfd; ++i)
	if (FD_ISSET(i, &fds))
	  aioDisable(i);
	nfd= 0;
	FD_ZERO(&fds);
	sessionID= 0;
	return 1;
}

/* returns error code */
int sqSpreadConnect(
	SpreadConnection *s,	/*OUT*/
	const char *daemonName,
	int daemonNameSize,
	const char *privateName,
	int privateNameSize,
	int wantsGroupMsgs,
	char *groupBuf,			/*OUT*/
	int semaIndex)
{
	int retval;
	int mbox;
	char *privateNameCopy, *daemonNameCopy;

	if (s == NULL)
		return ILLEGAL_SESSION;

	privateNameCopy = strndup(privateName, privateNameSize);
	daemonNameCopy = strndup(daemonName, daemonNameSize);

	retval = SP_connect(daemonNameCopy, privateNameCopy, 0, wantsGroupMsgs,
			&mbox, groupBuf);

	dprintf((stderr,
		"sqSpreadConnect(%1$p, %2$d %3$.*2$s, %4$d %5$.*4$s, %6$d, %7$p)\n",
		(void*)s, daemonNameSize, daemonName,
		privateNameSize, privateName,
		wantsGroupMsgs, groupBuf));

	dprintf((stderr, "retval=%d mbox=%d sem=%d\n", retval, mbox, semaIndex));

	free(privateNameCopy);
	free(daemonNameCopy);

	if (retval == ACCEPT_SESSION)
	{
		s->semaIndex = semaIndex;
		s->mbox = mbox;
		s->sessionID = sessionID;

		FD_SET(mbox, &fds);
		nfd= max(nfd, mbox + 1);
		aioEnable(mbox, (void*)semaIndex, AIO_EXT);
		aioHandle(mbox, &dataReadyCallback, AIO_R);
	}

	return retval;
}

/* returns error code */
int sqSpreadDisconnect(SpreadConnection * s)
{
	validate(s);
	closeMbox(s);
	return SP_disconnect(s->mbox);
}

/* returns msg length (0..n) or error code (<0) */
int sqSpreadReceive(
	SpreadConnection const * s,
	int *serviceType,		/*OUT*/
	char *sender,			/*OUT*/
	int maxGroups,
	int *numberOfGroups,	/*OUT*/
	char *groups,			/*OUT*//* numberOfGroups*MAX_GROUP_NAME long */
	int16 *messageType,		/*OUT*/
	int *endianMismatch,	/*OUT*/
	int messageLength,
	char *message,			/*OUT*/
	int drop)
{
	int retval;

	validate(s);

	*serviceType = drop ? DROP_RECV : 0;
	retval = SP_receive( s->mbox, serviceType,
		    sender, maxGroups,
		    numberOfGroups, (groups_name*)groups,
		    messageType, endianMismatch,
		    messageLength, message);
	dprintf((stderr, "recv rval %d mbox %d stype %d sender %s maxG %d numG %d"
			" groups %s mtype %d endian %d mesLen %d mess %.*s\n", retval,
			s->mbox, *serviceType,
		    sender, maxGroups,
		    *numberOfGroups, (groups_name*)groups,
		    *messageType, *endianMismatch,
		    messageLength, messageLength, message));

	if (retval == CONNECTION_CLOSED)
		closeMbox(s);
	else
		/* make sure we get the next callback */
		aioHandle(s->mbox, &dataReadyCallback, AIO_R);

	return retval;
}

/* Print out the given Spread error. */
int sqSpreadError(int n)
{
	SP_error(n);
	return 0;
}

int sqSpreadMultigroupMulticast(
		SpreadConnection const *s,
		int service_type,
		int num_groups,
		char const *groups,
		int mess_type,
		int mess_len,
		const char* mess)
{
	int retval;
	validate(s);
	retval = SP_multigroup_multicast( s->mbox,
			service_type,
			num_groups,
			(cgroups_name*)groups,
			(int16)(mess_type & 0xffff),
			mess_len,
			mess); 
	if (retval == CONNECTION_CLOSED)
		closeMbox(s);
	return retval;
}

/* return error (<0), nothing (0), or bytes available (>0) */
int sqSpreadPoll(SpreadConnection const *s)
{
	validate(s);
	return SP_poll(s->mbox);
}

/* return 0 on success, or error (<0) */
int sqSpreadJoin(SpreadConnection const *s, char const* group, int groupLength)
{
	char *groupString;
	int retval;

	validate(s);
	groupString = strndup(group, groupLength);
	retval = SP_join(s->mbox, groupString);
	free(groupString);
	if (retval == CONNECTION_CLOSED)
		closeMbox(s);
	return retval;
}

/* return 0 on success, or error (<0) */
int sqSpreadLeave(SpreadConnection const *s, char const* group, int groupLength)
{
	char *groupString;
	int retval;

	validate(s);
	groupString = strndup(group, groupLength);
	retval = SP_leave(s->mbox, groupString);
	free(groupString);
	if (retval == CONNECTION_CLOSED)
		closeMbox(s);
	return retval;
}

/* Query for Squeak */
int sqSpreadIsValid(SpreadConnection const *s)
{
  return isValid(s);
}

/* vim: set ts=4 sw=4 */
