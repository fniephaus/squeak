/*
 *  sqMacExtendedClipboard.c
 *  SqueakClipboardExtendedxcodeproj
 *
 *  Created by John Sterling Mcintosh on 4/21/06.
 *  Copyright 2006 Corporate Smalltalk Consulting ltd. All rights reserved.
 *  Licenced under the squeak-l NSImage
 *
 */

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<X11/Xlib.h>
#include<X11/Xatom.h>

#ifdef CLIPBOARD_TEST
  #include<stdio.h>
  typedef int sqInt;
#else
  #include "sqVirtualMachine.h"
  extern struct VirtualMachine* interpreterProxy;
#endif /* ifdef CLIPBOARD_TEST */

extern	Atom	 inputSelection;
extern	Atom	 *inputTargets;
char * getSelectionData(Atom selection, Atom target, size_t * bytes, XEvent * event);

Display * ioGetDisplay();
void updateInputTargets(Atom * newTargets, int targetSize);
void destroyInputTargets();
void finishDrop (XSelectionEvent * evt);

/* Convert string to Atom */
static Atom atom (char * name)
{
  return XInternAtom(ioGetDisplay(), name, False);
}

/* Get available media types in current clipboard */
static void getItemFravors()
{
  Atom * targets;
  size_t bytes= 0;

  targets= (Atom *) getSelectionData(
				     atom("CLIPBOARD"),
				     atom("TARGETS"),
				     &bytes, NULL);
  updateInputTargets(targets, bytes / sizeof(Atom));
}

#ifndef CLIPBOARD_TEST

void sqPasteboardClear( sqInt inPasteboard )
{
  /*	PasteboardClear( inPasteboard ); */
}

/* Return a number of types.
 * Update it only if the selection is CLIPBOARD
 */
int sqPasteboardGetItemCount(sqInt inPasteboard)
{
  if (inputSelection != atom("XdndSelection"))
    {
      getItemFravors();
    }
  if (inputTargets == NULL)
    return 0;

  int count= 0;
  while(inputTargets[count++] != None);
  return count;
}

/* Answer a type name in the position. */
int sqPasteboardCopyItemFlavorsitemNumber (sqInt inPasteboard, int formatNumber)
{
  int i;
  char * itemFlavor;
  size_t length;
  Atom target;
  int outData;
  char * dest;

  if (formatNumber < 1)
    return interpreterProxy->nilObject();

  if (inputTargets == NULL)
    return interpreterProxy->nilObject();
  
  for (i= 0; i < formatNumber; i++) {
    if (inputTargets[i] == None)
      return interpreterProxy->nilObject();
    target= inputTargets[i];
  }
    
  itemFlavor= XGetAtomName(ioGetDisplay(), target);
  length= strlen(itemFlavor);
  outData = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classString(), length);

  dest = ((char *) (interpreterProxy->firstIndexableField(outData)));
  memcpy(dest, itemFlavor, length);

  return outData;
}

/* In X11 clipboard is global in a display, so it just return 1 */
sqInt sqCreateClipboard( void )
{
  return 1;
}

void sqPasteboardPutItemFlavordatalengthformatTypeformatLength ( sqInt inPasteboard, char* inData, int dataLength, char* format, int formatLength)
{	
/* 	OSStatus err; */

/* 	CFStringRef formatType = CFStringCreateWithBytes ( kCFAllocatorDefault, format, formatLength,  */
/* 														kCFStringEncodingMacRoman, false); */
/* 	CFDataRef convertedData = CFDataCreate(NULL, inData, dataLength); */
	
/* 	err = PasteboardPutItemFlavor ( inPasteboard, 1, formatType, convertedData, kPasteboardFlavorNoFlags); */

/* 	CFRelease (convertedData); */
/* 	CFRelease (formatType); */

/* 	if (err) { */
/* 		interpreterProxy->success(0); */
/* 	} */
}

/* Return content in format at inputSelection. */
int sqPasteboardCopyItemFlavorDataformatformatLength ( sqInt inPasteboard, char* format, int formatLength)
{
  char * formatString;
  char * buffer;
  size_t bytes;
  Atom target;
  int outData;
  char * dest;
  XEvent ev;

  formatString= (char *) malloc(formatLength + 1);
  memcpy(formatString, format, formatLength);
  formatString[formatLength]= 0;

  if (inputSelection == None) {
    inputSelection= atom("CLIPBOARD");
  }

  printf("selection=%s\n", XGetAtomName(ioGetDisplay(), inputSelection));

  target= atom(formatString);
  buffer= (char *) getSelectionData(inputSelection, target, &bytes, &ev);

  if (inputSelection == atom("XdndSelection"))
    {
      destroyInputTargets();
      inputSelection= None;
      printf("From %s\n", __FILE__);
      finishDrop(&ev.xselection);
    }
  
  if (buffer == NULL)
    return interpreterProxy->nilObject();

  outData = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classByteArray(), bytes);
  dest = interpreterProxy->firstIndexableField(outData);
  memcpy(dest, buffer, bytes);
  free(buffer);
  free(formatString);

  return outData;
}

#endif /* #ifndef CLIPBOARD_TEST */

#ifdef CLIPBOARD_TEST

static Display * display;

Display * ioGetDisplay()
{
  return display;
}

char * getSelectionData(Atom selection, Atom target, size_t * bytes, XEvent * event)
{
  char * answer= "Hello world!";
  *bytes= strlen(answer) + 1;
  char * result= calloc(*bytes, 1);
  memcpy(result, answer, *bytes);
  return result;
}

void updateInputTargets(Atom * newTargets, int targetSize)
{
}

int main () {
  Window window;
  int i;
  size_t nitems;
  
  display= XOpenDisplay(NULL);
  if(display == NULL) {
    printf("Cannot open display\n");
    return 1;
  }
  window= XCreateSimpleWindow(display, DefaultRootWindow(display),
				 10, 10, 100, 100, 1, 0, 0);
  getItemFravors();
  getItemFravors();

  printf("\n");
  char * text= (char *) getSelectionData(
					 atom("CLIPBOARD"),
					 atom("STRING"), &nitems);
  printf("String contents: %s\n", text);
  return 0;
}

#endif /* CLIPBOARD_TEST */

