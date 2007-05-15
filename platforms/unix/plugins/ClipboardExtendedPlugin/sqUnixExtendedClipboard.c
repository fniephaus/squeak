/*  sqUnixExtendedClipboard.c  -- support for clipboard with multiple types
 *
 * Copyright (C) 2007 by Viewpoints Research Institute and other
 *                             authors/contributors as listed.
 *   All rights reserved.
 *   
 *   This file is part of Unix Squeak.
 * 
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 * 
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * 
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
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
extern Display * stDisplay;

typedef struct _SelectionChunk
{
  unsigned char * data;
  size_t size;
  struct _SelectionChunk * next;
  struct _SelectionChunk * last;
} SelectionChunk;

SelectionChunk * newSelectionChunk();
void destroySelectionChunk(SelectionChunk * chunk);
void addSelectionChunk(SelectionChunk * chunk,
		       unsigned char * src, size_t size);
size_t sizeSelectionChunk(SelectionChunk * chunk);
void copySelectionChunk(SelectionChunk * chunk, char * dest);
void getSelectionChunk(SelectionChunk * chunk, Atom selection, Atom target);
char * getSelectionData(Atom selection, Atom target, size_t * bytes);

void updateInputTargets(Atom * newTargets, int targetSize);
void destroyInputTargets();
void finishDrop();

/* Convert string to Atom */
static Atom atom (char * name)
{
  return XInternAtom(stDisplay, name, False);
}

/* Get available media types in current clipboard */
static void getItemFravors()
{
  Atom * targets;
  size_t bytes= 0;

  targets= (Atom *) getSelectionData(
				     atom("CLIPBOARD"),
				     atom("TARGETS"),
				     &bytes);
  updateInputTargets(targets, bytes / sizeof(Atom));
  free(targets);
}

#ifndef CLIPBOARD_TEST

void sqPasteboardClear( sqInt inPasteboard )
{
  /* NOT IMPLEMENTED YET */
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
    
  itemFlavor= XGetAtomName(stDisplay, target);
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
  /* NOT YETT */

}

/* Return content in format at inputSelection. */
int sqPasteboardCopyItemFlavorDataformatformatLength (sqInt inPasteboard, char* format, int formatLength)
{
  char * formatString;
  size_t bytes;
  Atom target;
  int outData;

  formatString= (char *) malloc(formatLength + 1);
  memcpy(formatString, format, formatLength);
  formatString[formatLength]= 0;

  if (inputSelection == None)
    inputSelection= atom("CLIPBOARD");
  
  target= atom(formatString);
  free(formatString);

  SelectionChunk * chunk= newSelectionChunk();
  getSelectionChunk(chunk, inputSelection, target);
  bytes= sizeSelectionChunk(chunk);

  if (inputSelection == atom("XdndSelection"))
    {
      destroyInputTargets();
      inputSelection= None;
      finishDrop();
    }
  
  if (bytes == 0)
    {
      destroySelectionChunk(chunk);
      return interpreterProxy->nilObject();
    }

  outData = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classByteArray(), bytes);
  copySelectionChunk(chunk, interpreterProxy->firstIndexableField(outData));
  destroySelectionChunk(chunk);

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

