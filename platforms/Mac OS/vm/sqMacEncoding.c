/*
 *  sqMacEncoding.c
 *  SqueakVMForCarbon
 *
 *  Created by John M McIntosh on Mon Dec 01 2003.
 *	May 24th, 2005, JMM. bug in SetImageNameViaCFString
 *
 3.8.11b1 Mar 4th, 2006 JMM refactor, cleanup and add headless support
 */
#include "sq.h"

#include "sqMacEncoding.h"
#include "sqMacUIConstants.h" 
#include "sqMacFileLogic.h"	


    CFStringEncoding gCurrentVMEncoding=kCFStringEncodingMacRoman;


/*** Variables -- image and path names ***/
//HYRDRA char imageName[IMAGE_NAME_SIZE+1];

static CFStringRef vmPathString=NULL;
static CFStringRef documentNameString=NULL;

void getVMPathWithEncoding(char *target,UInt32 encoding) {
    CFStringGetCString (vmPathString, target, VMPATH_SIZE, encoding);
}

void SetVMPathFromCFString(CFMutableStringRef strRef) {    
    if (vmPathString != NULL)  
        CFRelease(vmPathString);
	vmPathString = strRef;
	CFRetain(vmPathString);
}

void getImageNameWithEncoding(INTERPRETER_ARG_COMMA char *target,UInt32 encoding) {
	DECL_MAC_STATE();
    CFStringGetCString (MAC_STATE(imageName), target, IMAGE_NAME_SIZE, encoding);
}

char *getImageName(INTERPRETER_ARG) {
	DECL_MAC_STATE();
    getImageNameWithEncoding(INTERPRETER_PARAM_COMMA MAC_STATE(workArea),gCurrentVMEncoding);
    return MAC_STATE(workArea);
}
    
void SetImageNameViaCFString(INTERPRETER_ARG_COMMA CFStringRef string) {
    char *ignore;
	DECL_MAC_STATE();
	// normalization because we get here from looking for file name in resource folder directly at startup time. 
	// HFS+ imposes Unicode2.1 decomposed UTF-8 encoding on all path elements
	CFMutableStringRef mutableStr= CFStringCreateMutableCopy(NULL, 0, string);
	if (gCurrentVMEncoding == kCFStringEncodingUTF8) 
		CFStringNormalize(mutableStr, kCFStringNormalizationFormKC); // pre-combined
    CFRetain(mutableStr);
	if (MAC_STATE(imageName) != NULL)
        CFRelease(MAC_STATE(imageName));
    MAC_STATE(imageName) = mutableStr;
    ignore = getImageName(INTERPRETER_PARAM);
}

void SetImageNameViaString(INTERPRETER_ARG_COMMA char *string,UInt32 encoding) {
	CFStringRef path;
	DECL_MAC_STATE();
	
	if (MAC_STATE(imageName) != NULL)
        CFRelease(MAC_STATE(imageName));
	path = CFStringCreateWithCString(NULL, string, encoding);
    SetImageNameViaCFString(INTERPRETER_PARAM_COMMA path);
	CFRelease(path);
}


Boolean ImageNameIsEmpty(INTERPRETER_ARG) {
	DECL_MAC_STATE();
    if (MAC_STATE(imageName) == NULL) 
        return true;
    return getImageName(INTERPRETER_PARAM) == NULL;
}

void getDocumentNameWithEncoding(char *target,UInt32 encoding) {
    if (documentNameString == NULL) {
        *target = 0x00;
        return;
    }
    CFStringGetCString (documentNameString, target, DOCUMENT_NAME_SIZE, encoding);
}

void SetDocumentNameViaString(char *string,UInt32 encoding) {
    if (documentNameString != NULL)
        CFRelease(documentNameString);
    documentNameString = CFStringCreateWithCString(NULL, string, encoding);
}

void getShortImageNameWithEncoding(INTERPRETER_ARG_COMMA char *target,UInt32 encoding) {
	DECL_MAC_STATE();
    if (MAC_STATE(shortImageName) == NULL) {
        *target = 0x00;
        return;
    }
    CFStringGetCString (MAC_STATE(shortImageName), target, SHORTIMAGE_NAME_SIZE, encoding);
}

void SetShortImageNameViaString(INTERPRETER_ARG_COMMA char *string,UInt32 encoding) {
	DECL_MAC_STATE();
    if (MAC_STATE(shortImageName) != NULL)
        CFRelease(MAC_STATE(shortImageName));
    MAC_STATE(shortImageName) = CFStringCreateWithCString(NULL, string, encoding);
}

Boolean ShortImageNameIsEmpty(INTERPRETER_ARG) {
	DECL_MAC_STATE();
    return MAC_STATE(shortImageName) == NULL;
}

void setEncodingType (char * string) {
      gCurrentVMEncoding = kCFStringEncodingMacRoman;
      if (strcmp("UTF-8",string) == 0)
          gCurrentVMEncoding = kCFStringEncodingUTF8;
      if (strcmp("ShiftJIS",string) == 0)
          gCurrentVMEncoding = kCFStringEncodingShiftJIS;
      if (strcmp("Latin1",string) == 0)
          gCurrentVMEncoding = kCFStringEncodingISOLatin1;
      if (strcmp("iso-8859-1",string) == 0)
          gCurrentVMEncoding = kCFStringEncodingISOLatin1;
  }
  
  char  *getEncodingType(UInt32 aType) {
      if (aType == kCFStringEncodingMacRoman) 
          return (char *)&"macintosh";
      if (aType == kCFStringEncodingUTF8) 
          return (char *)&"UTF-8";
      if (aType == kCFStringEncodingShiftJIS) 
          return (char *)&"ShiftJIS";
      if (aType == kCFStringEncodingISOLatin1) 
          return (char *)&"Latin1";
      return (char *)&"macintosh";
  }
