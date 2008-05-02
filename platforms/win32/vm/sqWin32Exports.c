#include <stdio.h>
#include "sq.h"

/* Note: Primitives should be in correct format */

int win32JoystickDebugInfo(INTERPRETER_ARG);
int win32JoystickDebugPrintRawValues(INTERPRETER_ARG);
int win32JoystickDebugPrintAlternativeValues(INTERPRETER_ARG);
int win32DebugPrintSocketState(INTERPRETER_ARG);
int primitivePluginBrowserReady(INTERPRETER_ARG);
int primitivePluginRequestURLStream(INTERPRETER_ARG);
int primitivePluginRequestURL(INTERPRETER_ARG);
int primitivePluginPostURL(INTERPRETER_ARG);
int primitivePluginRequestFileHandle(INTERPRETER_ARG);
int primitivePluginDestroyRequest(INTERPRETER_ARG);
int primitivePluginRequestState(INTERPRETER_ARG);

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

