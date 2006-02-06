/****************************************************************************
*   PROJECT: Mac menu bar logic
*   FILE:    sqMacUIMenuBar.c
*   CONTENT: 
*
*   AUTHOR:  John Maloney, John McIntosh, and others.
*   ADDRESS: 
*   EMAIL:   johnmci@smalltalkconsulting.com
*   RCSID:   $Id: sqMacUIMenuBar.c 1238 2005-08-16 12:36:18Z johnmci $
*
*   NOTES: 
*  Feb 22nd, 2002, JMM moved code into 10 other files, see sqMacMain.c for comments
*  Oct 18th, 2004, JMM changes for host menu support
*****************************************************************************/

#include "sqMacUIMenuBar.h"
#include "sqMacMain.h"
#include "sqMacWindow.h"
#include "sq.h"

MenuHandle	appleMenu = nil;
MenuHandle	editMenu = nil;
int		menuBarHeight = 20;
RgnHandle	menuBarRegion = nil;  /* if non-nil, then menu bar has been hidden */
MenuHandle	fileMenu = nil;
extern struct VirtualMachine* interpreterProxy;

 
#if TARGET_API_MAC_CARBON
    #define EnableMenuItemCarbon(m1,v1)  EnableMenuItem(m1,v1);
    #define DisableMenuItemCarbon(m1,v1)  DisableMenuItem(m1,v1);
#else
    #define EnableMenuItemCarbon(m1,v1)  EnableItem(m1,v1);
    #define DisableMenuItemCarbon(m1,v1)  DisableItem(m1,v1);
#endif

void MenuBarRestore() {
}

void SetUpMenus(void) {
#if TARGET_API_MAC_CARBON
	long decideOnQuitMenu;
#endif 
	
	InsertMenu(appleMenu = NewMenu(appleID, "\p\024"), 0);
	InsertMenu(fileMenu  = NewMenu(fileID,  "\pFile"), 0);
	InsertMenu(editMenu  = NewMenu(editID,  "\pEdit"), 0);
#if TARGET_API_MAC_CARBON
    Gestalt( gestaltMenuMgrAttr, &decideOnQuitMenu);
    if (!(decideOnQuitMenu & gestaltMenuMgrAquaLayoutMask) || true)	
        AppendMenu(fileMenu, "\pQuit do not save");
    if (RunningOnCarbonX()) {
        DisableMenuCommand(NULL,'quit');
	}
#else
	AppendResMenu(appleMenu, 'DRVR');
    AppendMenu(fileMenu, "\pQuit do not save");
#endif
 	AppendMenu(editMenu, "\pUndo/Z;(-;Cut/X;Copy/C;Paste/V;Clear");
	/* Disable items in the Edit menu */
	DisableMenuItemCarbon(editMenu, 1);
	DisableMenuItemCarbon(editMenu, 3);
	DisableMenuItemCarbon(editMenu, 4);
	DisableMenuItemCarbon(editMenu, 5);
	DisableMenuItemCarbon(editMenu, 6);
	DrawMenuBar();
}

 void AdjustMenus(void) {
	WindowRef		wp;
	int				isDeskAccessory;
	int				cutState;	
		
	cutState = IsMenuItemEnabled(editMenu, 1);

	wp = FrontWindow();
	if (wp != NULL) {
		isDeskAccessory = GetWindowKind(wp) < 0;
	} else {
		isDeskAccessory = false;
	}

	if (isDeskAccessory) {
		/* Enable items in the Edit menu */
		EnableMenuItemCarbon(editMenu, 1);
		EnableMenuItemCarbon(editMenu, 3);
		EnableMenuItemCarbon(editMenu, 4);
		EnableMenuItemCarbon(editMenu, 5);
		EnableMenuItemCarbon(editMenu, 6);
	} else {
		if (cutState) {
			/* Enable items in the Edit menu */
			EnableMenuItemCarbon(editMenu, 1);
			EnableMenuItemCarbon(editMenu, 3);
			EnableMenuItemCarbon(editMenu, 4);
			EnableMenuItemCarbon(editMenu, 5);
		} else {
			/* Disable items in the Edit menu */
			DisableMenuItemCarbon(editMenu, 1);
			DisableMenuItemCarbon(editMenu, 3);
			DisableMenuItemCarbon(editMenu, 4);
			DisableMenuItemCarbon(editMenu, 5);
			DisableMenuItemCarbon(editMenu, 6);
		}
	}
}

