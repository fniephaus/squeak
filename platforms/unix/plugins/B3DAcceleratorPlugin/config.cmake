IF (OPENGL_FOUND)
  SET (USE_OPENGL 1)
  SET (USE_X11_GLX 1)
  UNSET (tmp)
  FIND_PATH (tmp GL/gl.h)
  IF (tmp)
    SET (OPENGL_INCLUDE_DIR ${tmp})
  ENDIF (tmp)
  CHECK_INCLUDE_FILE (GL/gl.h HAVE_GL_GL_H -I${OPENGL_INCLUDE_DIR})
  CHECK_INCLUDE_FILE (OpenGL/gl.h HAVE_OPENGL_GL_H)
ENDIF (OPENGL_FOUND)

CONFIG_DEFINE (USE_OPENGL)
CONFIG_DEFINE (USE_X11_GLX)
CONFIG_DEFINE (USE_QUARTZ_CGL)
CONFIG_DEFINE (HAVE_GL_GL_H)
CONFIG_DEFINE (HAVE_OPENGL_GL_H)
