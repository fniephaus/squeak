#include <windows.h>

#include "B3DAcceleratorPlugin_imports.h"
#include "B3DAcceleratorPlugin.h"

#ifdef CROQUET
int glMode = 1; /* default to OpenGL */
#else
int glMode = 0; /* default to D3D */
#endif

int b3dxInitialize(void) {
  int *ptr;
  ptr =  (int*)vmFunction(loadFunctionfrom)("fUseOpenGL","");
  if(ptr) {
    glMode = *ptr;
  }
  if(glMode)
    return glInitialize();
  else 
    return d3dInitialize();
}

