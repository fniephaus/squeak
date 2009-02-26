#include <stdio.h>
#include "sq.h"

/* Note: Primitives should be in correct format */

int win32JoystickDebugInfo _iarg();
int win32JoystickDebugPrintRawValues _iarg();
int win32JoystickDebugPrintAlternativeValues _iarg();
int win32DebugPrintSocketState _iarg();
int primitivePluginBrowserReady _iarg();
int primitivePluginRequestURLStream _iarg();
int primitivePluginRequestURL _iarg();
int primitivePluginPostURL _iarg();
int primitivePluginRequestFileHandle _iarg();
int primitivePluginDestroyRequest _iarg();
int primitivePluginRequestState _iarg();

extern void* stWindow;
extern void* firstMessageHook;
extern void* preMessageHook;
extern int fUseOpenGL;

void *os_exports[][3] = {
  {"","win32JoystickDebugInfo", win32JoystickDebugInfo},
  {"","win32JoystickDebugPrintRawValues", win32JoystickDebugPrintRawValues},
  {"","win32JoystickDebugPrintAlternativeValues", win32JoystickDebugPrintAlternativeValues},
  {"","win32DebugPrintSocketState", win32DebugPrintSocketState},
  {"","primitivePluginBrowserReady", primitivePluginBrowserReady},
  {"","primitivePluginRequestURLStream", primitivePluginRequestURLStream},
  {"","primitivePluginRequestURL", primitivePluginRequestURL},
  {"","primitivePluginPostURL", primitivePluginPostURL},
  {"","primitivePluginRequestFileHandle", primitivePluginRequestFileHandle},
  {"","primitivePluginDestroyRequest", primitivePluginDestroyRequest},
  {"","primitivePluginRequestState", primitivePluginRequestState},
  {"", "printf", printf},
  {"","stWindow", &stWindow},
  {"","firstMessageHook", &firstMessageHook},
  {"","preMessageHook", &preMessageHook},
  {"","fUseOpenGL", &fUseOpenGL},
  {NULL,NULL, NULL}
};

