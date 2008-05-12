/****************************************************************************
*   PROJECT: Squeak Headers
*   FILE:    sqMacUIEvents.c
*   CONTENT: 
*
*   AUTHOR:  John Maloney, John McIntosh, and others.
*   ADDRESS: 
*   EMAIL:   johnmci@smalltalkconsulting.com
*   RCSID:   $Id$
*
*   NOTES: 
*  Feb 22nd, 2002, JMM moved code into 10 other files, see sqMacMain.c for comments
*  Feb 27th, 2002, JMM changed for carbon event logic.
*  Mar 8th,  2002, JMM external prims that make dialog windows must do this on main thread
*   3.7.0bx Nov 24th, 2003 JMM gCurrentVMEncoding, plus multiple keystrokes for input
****************************************************************************/
 
    #include <Carbon/Carbon.h>

typedef int (*eventMessageHook)(EventRecord* event);

int recordDragDropEvent(INTERPRETER_ARG_COMMA EventRecord *theEvent, int numberOfItems, int dragType);
int recordKeyboardEvent(INTERPRETER_ARG_COMMA EventRecord *theEvent, int keyType);
void recordWindowEvent(INTERPRETER_ARG_COMMA int windowType,int left, int top, int right, int bottom);
void recordMenu(int menuID,UInt32 menuItem);
int MouseModifierState(EventRecord *theEvent);
WindowPtr getSTWindow(void);
void setMessageHook(eventMessageHook theHook);
void setPostMessageHook(eventMessageHook theHook);
void ignoreLastEvent(INTERPRETER_ARG);
void DoZoomWindow (EventRecord* theEvent, WindowPtr theWindow, short zoomDir, short hMax, short vMax);
int getUIToLock(sqInt *);
void SetUpCarbonEvent();
void SetUpCarbonEventForWindowIndex(INTERPRETER_ARG_COMMA int index);
void RunApplicationEventLoopWithSqueak(void);
