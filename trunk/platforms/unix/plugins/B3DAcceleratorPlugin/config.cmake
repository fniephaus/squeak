IF (OPENGL_FOUND OR AGL_FOUND)
  SET (USE_OPENGL 1)
  PLUGIN_INCLUDE_DIRECTORIES (${OPENGL_INCLUDE_DIR})
ENDIF ()

IF (OPENGL_FOUND)
  SET (USE_X11_GLX 1)
ENDIF ()

IF (AGL_FOUND)
  SET (USE_QUARTZ_CGL 1)
  SET (HAVE_OPENGL_GL_H 1)
ENDIF ()

CHECK_INCLUDE_FILE (GL/gl.h HAVE_GL_GL_H -I${OPENGL_INCLUDE_DIR})

CONFIG_DEFINE (USE_OPENGL)
CONFIG_DEFINE (USE_X11_GLX)
CONFIG_DEFINE (USE_QUARTZ_CGL)
CONFIG_DEFINE (HAVE_GL_GL_H)
CONFIG_DEFINE (HAVE_OPENGL_GL_H)
