/****************************************************************************
*   PROJECT: UUID support for the mac
*   FILE:    sqMacUUID.c
*   CONTENT: 
*
*   AUTHOR:  John McIntosh.
*   ADDRESS: 
*   EMAIL:   johnmci@smalltalkconsulting.com
*   RCSID:   $Id$
*
*   Feb 5 2002, JMM cleanup for windows port
*
*/

#include "UUIDPlugin_imports.h"

#include <CFUUID.h>
int sqUUIDInit(void);
int sqUUIDShutdown(void);

int MakeUUID(PLUGIN_IARG_COMMA sqUUID location) {
    CFUUIDRef theUUID;
    CFUUIDBytes theBytes;

    theUUID =  CFUUIDCreate (null);
    theBytes = CFUUIDGetUUIDBytes(theUUID);
    memcpy((char *) location,(char *) &theBytes,sizeof(sqUUID));
    CFRelease(theUUID);
	return 0;
}

int sqUUIDInit() {
    return 1;
}

int sqUUIDShutdown() {
    return 1;
}