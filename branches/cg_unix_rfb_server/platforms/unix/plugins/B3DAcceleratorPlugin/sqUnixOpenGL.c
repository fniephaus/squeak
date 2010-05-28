/* sqUnixOpenGL.c -- support for accelerated 3D rendering
 *
 * Author: Bert Freudenberg <bert@isg.cs.uni-magdeburg.de>
 * 
 * Based on Andreas Raab's sqWin32OpenGL.c
 * 
 * Last edited: 19 May 2001 bert@faust
 */

#include <sys/types.h>
#include "sqVirtualMachine.h"
#include "sqConfig.h"
#include "sqPlatformSpecific.h"
#include "B3DAcceleratorPlugin.h"

#if defined (B3DX_GL)

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <stdio.h>

#include "sqOpenGLRenderer.h"

/* Plugin refs */
extern struct VirtualMachine *interpreterProxy;

static Display *stDisplay= NULL; /* Squeak's display */
static Window   stWindow=  0;    /* Squeak's main window */

static glRenderer *current= NULL;
static glRenderer allRenderer[MAX_RENDERER];


static int visualAttributes[]= {
  GLX_RGBA,                 /* no indexed colors */
  GLX_DOUBLEBUFFER,         /* will swap */
  GLX_LEVEL,            0,  /* frame buffer, not overlay */
  GLX_RED_SIZE,         1,  /* need rgb & alpha */
  GLX_GREEN_SIZE,       1,
  GLX_BLUE_SIZE,        1,
  GLX_ALPHA_SIZE,       1,
  GLX_DEPTH_SIZE,       16, /* decent depth */   
  GLX_STENCIL_SIZE,     0,  /* need no stencil */
  GLX_AUX_BUFFERS,      0,  /* no aux buffers */
  GLX_ACCUM_RED_SIZE,   0,  /* no accumulation */
  GLX_ACCUM_GREEN_SIZE, 0,
  GLX_ACCUM_BLUE_SIZE,  0,
  GLX_ACCUM_ALPHA_SIZE, 0,
  None
};

static float blackLight[4]= { 0.0f, 0.0f, 0.0f, 0.0f };

/* Verbose level for debugging purposes:
   0 - print NO information ever
   1 - print critical debug errors
   2 - print debug warnings
   3 - print extra information
   4 - print extra warnings
   5 - print information about primitive execution

   10 - print information about each vertex and face
*/
int verboseLevel= 1;


/*** create / destroy a renderer ***/

int glCreateRenderer(int allowSoftware, int allowHardware, int x, int y, int w, int h)
{
  glRenderer *renderer;
  XVisualInfo* visinfo= 0;
  int index= -1;

  /* find unused renderer */
  {
    int i;
    for (i= 0; i < MAX_RENDERER; i++)
      {
	if (!allRenderer[i].used)
	  {
	    index= i;
	    break;
	  }
      }
  }

  if (index == -1)
    {
      DPRINTF(1, (fp, "ERROR: Maximum number of renderers (%d) exceeded\n", MAX_RENDERER));
      return 0;
    }


  renderer= &allRenderer[index];
  renderer->used= True;
  renderer->window= 0;
  renderer->context= NULL;

  DPRINTF(3, (fp, "---- Creating new renderer ----\n\n"));

  /* sanity checks */

  if (w < 0 || h < 0)
    {
      DPRINTF(1, (fp, "Negative extent (%i@%i)!\n", w, h));
      goto FAILED;
    }

  /* choose visual and create context */

  {
    visinfo= glXChooseVisual(stDisplay, 
					  DefaultScreen(stDisplay), 
					  visualAttributes);
    if (!visinfo)
      {
	DPRINTF(1, (fp, "No OpenGL visual found!\n"));
	goto FAILED;
      }

    DPRINTF(3, (fp, "\n#### Selected GLX visual #%ld (depth: %d) ####\n", 
		visinfo->visualid, visinfo->depth));

    renderer->context= glXCreateContext(stDisplay, visinfo, 0, GL_TRUE);

    if (!renderer->context)
      {
	DPRINTF(1, (fp, "Creating GLX context failed!\n"));
	goto FAILED;
      }

    DPRINTF(3, (fp, "\n#### Created GLX context ####\n"  ));

    /* create window */
    {
      XSetWindowAttributes attributes;
      unsigned long valuemask= 0;

      attributes.colormap= XCreateColormap(stDisplay,
					   DefaultRootWindow(stDisplay),
					   visinfo->visual,
					   AllocNone);
      valuemask|= CWColormap;

      attributes.background_pixel= BlackPixel(stDisplay, DefaultScreen(stDisplay));
      valuemask|= CWBackPixel;

      renderer->window= XCreateWindow(stDisplay, stWindow, x, y, w, h, 0,
				    visinfo->depth, InputOutput, visinfo->visual, 
				    valuemask, &attributes);
      if (!renderer->window) 
	{
	  DPRINTF(1, (fp, "Failed to create client window\n"));
	  goto FAILED;
	}

      XMapWindow(stDisplay, renderer->window);

    }

    DPRINTF(3, (fp, "\n#### Created window ####\n"  ));

    XFree(visinfo);
    visinfo= 0;
  }

  /* Make the context current */
  if (!glXMakeCurrent(stDisplay, renderer->window, renderer->context))
    {
      DPRINTF(1, (fp, "Failed to make context current\n"));
      goto FAILED;
    }
   
  renderer->bufferRect[0]= x;
  renderer->bufferRect[1]= y;
  renderer->bufferRect[2]= w;
  renderer->bufferRect[3]= h;

  DPRINTF(3, (fp, "\n### Renderer created! ###\n"));

  /* setup user context */
  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glEnable(GL_DITHER);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glShadeModel(GL_SMOOTH);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, blackLight);
  ERROR_CHECK;
  return index;

FAILED:
  /* do necessary cleanup */
  DPRINTF(1, (fp, "OpenGL initialization failed\n"));

  if (visinfo)
    XFree(visinfo);
  if (renderer->context)
    glXDestroyContext(stDisplay, renderer->context);
  if (renderer->window)
    XDestroyWindow(stDisplay, renderer->window);

  return -1;
}


int glDestroyRenderer(int handle)
{
  glRenderer *renderer= glRendererFromHandle(handle);

  DPRINTF(3, (fp, "\n--- Destroying renderer ---\n"));

  if (!renderer)
    return 1; /* already destroyed */

  if (!glMakeCurrentRenderer(NULL))
    return 0;

  glXDestroyContext(stDisplay, renderer->context);
  XDestroyWindow(stDisplay, renderer->window);

  renderer->window= 0;
  renderer->context= NULL;
  renderer->used= False;

  return 1;
}



/*** helpers ***/


glRenderer *glRendererFromHandle(int handle)
{
  DPRINTF(7, (fp, "Looking for renderer id: %i\n", handle));

  if (handle < 0 || handle >= MAX_RENDERER) 
    return NULL;

  if (allRenderer[handle].used)
    return allRenderer+handle;

  return NULL;
}


int glIsOverlayRenderer(int handle)
{
  /* we always use overlay renderers */
  return 1;
}


int glSwapBuffers(glRenderer *renderer)
{
  if (!renderer || !renderer->used)
    return 0;

  glXSwapBuffers(stDisplay, renderer->window);

  return 1;
}


int glMakeCurrentRenderer(glRenderer *renderer)
{
  if (current == renderer)
    return 1;

  if (renderer && !renderer->used)
    return 0;

  if (renderer)
    {
      if (!glXMakeCurrent(stDisplay, renderer->window, renderer->context))
	{
	  DPRINTF(1, (fp, "Failed to make context current\n"));
	  return 0;
	}
    }
  else
    {
      glXMakeCurrent(stDisplay, 0, NULL);
    }
  
  current= renderer;
  return 1;
}


int glSetBufferRect(int handle, int x, int y, int w, int h)
{
  glRenderer *renderer= glRendererFromHandle(handle);

  if (!renderer || !glMakeCurrentRenderer(renderer))
    return 0;

  if (w < 1 || h < 1)
    return 0;


  XMoveResizeWindow(stDisplay, renderer->window, 
		    x, y, w, h);

  renderer->bufferRect[0]= x;
  renderer->bufferRect[1]= y;
  renderer->bufferRect[2]= w;
  renderer->bufferRect[3]= h;

  return 1;
}


int glSetVerboseLevel(int level)
{
  verboseLevel= level;
  return 1;
}


int glGetIntProperty(int handle, int prop)
{
  GLint v;
  glRenderer *renderer= glRendererFromHandle(handle);
  if (!renderer || !glMakeCurrentRenderer(renderer))
    return 0;

  switch(prop) {
  case 1: /* backface culling */
    if (!glIsEnabled(GL_CULL_FACE)) return 0;
    glGetIntegerv(GL_FRONT_FACE, &v);
    if (v == GL_CW) return 1;
    if (v == GL_CCW) return -1;
    return 0;
  case 2: /* polygon mode */
    glGetIntegerv(GL_POLYGON_MODE, &v);
    ERROR_CHECK;
    return v;
  case 3: /* point size */
    glGetIntegerv(GL_POINT_SIZE, &v);
    ERROR_CHECK;
    return v;
  case 4: /* line width */
    glGetIntegerv(GL_LINE_WIDTH, &v);
    ERROR_CHECK;
    return v;
  }
  return 0;
}


int glSetIntProperty(int handle, int prop, int value)
{
  glRenderer *renderer= glRendererFromHandle(handle);
  if (!renderer || !glMakeCurrentRenderer(renderer))
    return 0;

  switch(prop) {
  case 1: /* backface culling */
    if (!value) {
      glDisable(GL_CULL_FACE);
      ERROR_CHECK;
      return 1;
    }
    glEnable(GL_CULL_FACE);
    glFrontFace(value == 1 ? GL_CCW : GL_CW);
    ERROR_CHECK;
    return 1;
  case 2: /* polygon mode */
    if (value == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else if (value == 1) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else if (value == 2) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    else return 0;
    ERROR_CHECK;
    return 1;
  case 3: /* point size */
    glPointSize(value);
    ERROR_CHECK;
    return 1;
  case 4: /* line width */
    glLineWidth(value);
    ERROR_CHECK;
    return 1;
  }
  return 0;
}




/*** Module initializers ***/



int glInitialize(void)
{
  int i, p;

  p= interpreterProxy->ioLoadFunctionFrom("stDisplay", NULL);
  if (!p)
    {
      DPRINTF(1,(fp,"ERROR: Failed to look up stDisplay\n"));
      return 0;
    }
  stDisplay= *(Display**) p;

  p= interpreterProxy->ioLoadFunctionFrom("stWindow", NULL);
  if (!p)
    {
      DPRINTF(1,(fp,"ERROR: Failed to look up stWindow\n"));
      return 0;
    }
  stWindow= *(Window*) p;

  for (i= 0; i < MAX_RENDERER; i++)
    {
      allRenderer[i].used= False;
    }

  return 1;
}


int glShutdown(void)
{
  int i;

  for (i=0; i< MAX_RENDERER; i++)
    {
      if (allRenderer[i].used)
	{
	  glDestroyRenderer(i);
	}
    }

  return 1;
}

#endif /* defined(B3DX_GL) */