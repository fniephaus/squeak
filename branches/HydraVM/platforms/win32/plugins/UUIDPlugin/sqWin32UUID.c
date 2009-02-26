/****************************************************************************
*   PROJECT: Squeak port for Win32 (NT / Win95)
*   FILE:    sqWin32UUID.c
*   CONTENT: UUID support
*
*   AUTHOR:  Andreas Raab (ar)
*   ADDRESS: 
*   EMAIL:   Andreas.Raab@gmx.de
*   RCSID:   $Id$
*
*   NOTES:
*****************************************************************************/
#include <windows.h>
#include <ole2.h>
#include "UUIDPlugin_imports.h"

int sqUUIDInit(void) {
  return 1;
}

int sqUUIDShutdown(void) {
  return 1;
}

int MakeUUID _iargs(char *location) {
  if(CoCreateGuid((GUID*)location) == S_OK) return 1;
  vmFunction(primitiveFail) _iparam();
  return 0;
}

