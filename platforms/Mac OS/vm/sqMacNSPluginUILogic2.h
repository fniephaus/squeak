/*
 *  sqMacNSPluginUILogic2.h
 *  SqueakVMUNIXPATHS
 *
 */
#include "interp.h"

void setupPipes(void);
int plugInTimeToReturn(void);
int plugInNotifyUser(char *msg);
int primitivePluginBrowserReady(void);
void browserProcessCommand(void);
int MouseModifierStateFromBrowser(EventRecord*);
int recordKeyboardEvent(INTERPRETER_ARG_COMMA EventRecord *, int );
void recordMouseEvent(EventRecord *);
void recordMouseEventCarbon(INTERPRETER_ARG_COMMA EventRef ,UInt32 ,Boolean noPointConversion);
Boolean inline browserActiveAndDrawingContextOk(void);
void browserSetCursor(Cursor *macCursor);
int primitivePluginRequestState(void);
int primitivePluginDestroyRequest(void);
int primitivePluginRequestFileHandle(void);
int primitivePluginPostURL(void);
int primitivePluginRequestURLStream(void);
int primitivePluginRequestURL(void);
int browserGetWindowSize(void);
Boolean inline browserActiveAndDrawingContextOkAndNOTInFullScreenMode(void);
Boolean inline browserActiveAndDrawingContextOkAndInFullScreenMode(void);

typedef struct SqueakSharedMemoryBlock {
	int		written;
	int		top;
	int		right;
	int		bottom;
	int		left;
	char	screenBits[];
} SqueakSharedMemoryBlock;