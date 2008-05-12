/*
 *  sqMacEncoding.h
 *  SqueakVMForCarbon
 *
 *  Created by John M McIntosh on Mon Dec 01 2003.
 *
 */

#include <Carbon/Carbon.h>
#include "interp.h"

void getVMPathWithEncoding(char *target,UInt32 encoding);
void SetVMPathFromCFString(CFMutableStringRef strRef);
void getImageNameWithEncoding(INTERPRETER_ARG_COMMA char *target,UInt32 encoding);
void SetImageNameViaCFString(INTERPRETER_ARG_COMMA CFStringRef string);
void SetImageNameViaString(INTERPRETER_ARG_COMMA char *string,UInt32 encoding);
Boolean ImageNameIsEmpty(INTERPRETER_ARG);
char *getImageName(INTERPRETER_ARG);
void getShortImageNameWithEncoding(INTERPRETER_ARG_COMMA char *target,UInt32 encoding);
void SetShortImageNameViaString(INTERPRETER_ARG_COMMA char *string,UInt32 encoding);
Boolean ShortImageNameIsEmpty(INTERPRETER_ARG);
void getDocumentNameWithEncoding(char *target,UInt32 encoding);
void SetDocumentNameViaString(char *string,UInt32 encoding);

void setEncodingType (char *aType);
char* getEncodingType (UInt32 aType);
int IsImageName(char *name);