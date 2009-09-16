SET (OS_TYPE \"unix\")

CONFIG_DEFINE (OS_TYPE)

STRING (REGEX REPLACE "(.*)\\.(.*)\\.(.*)-(.*)" "\\1" major   ${version})
STRING (REGEX REPLACE "(.*)\\.(.*)\\.(.*)-(.*)" "\\2" minor   ${version})
STRING (REGEX REPLACE "(.*)\\.(.*)\\.(.*)-(.*)" "\\3" patch   ${version})
STRING (REGEX REPLACE "(.*)\\.(.*)\\.(.*)-(.*)" "\\4" release ${version})

IF (APPLE)
  SET (DARWIN 1)
  STRING_APPEND (CMAKE_C_FLAGS -DMACOSX)		# ffi.h
  USE_FRAMEWORK (CoreFoundation)
  USE_FRAMEWORK (CoreServices)
ENDIF (APPLE)

CONFIG_DEFINE (DARWIN)

SET (interp interp)

INCLUDE (TestBigEndian)
INCLUDE (CheckIncludeFile)
INCLUDE (CheckLibraryExists)
INCLUDE (CheckTypeSize)
INCLUDE (CheckFunctionExists)
INCLUDE (CheckVariableExists)
INCLUDE (CheckStructHasMember)
INCLUDE (FindPkgConfig)

FIND_PACKAGE (X11)
IF (X11_FOUND)
  LIST (REMOVE_DUPLICATES X11_INCLUDE_DIR)
ENDIF ()

IF (NOT without-gl)
  FIND_PACKAGE (OpenGL)
  LIST (REMOVE_DUPLICATES OPENGL_INCLUDE_DIR)
ENDIF ()

IF (APPLE AND NOT without-gl)
  SET (AGL_FOUND	${OPENGL_FOUND})
  SET (AGL_INCLUDE_DIR	${OPENGL_INCLUDE_DIR})
  SET (AGL_LIBRARIES	${OPENGL_LIBRARIES})
  IF (X11_FOUND)
    FIND_PATH    (GLX_INCLUDE_DIR GL/gl.h /usr/X11/include /usr/X11R6/include)
    FIND_LIBRARY (GLX_LIBRARIES   GL      /usr/X11/lib /usr/X11R6/lib)
    IF (GLX_INCLUDE_DIR AND GLX_LIBRARIES)
      SET (OPENGL_INCLUDE_DIR	${GLX_INCLUDE_DIR})
      SET (OPENGL_LIBRARIES	${GLX_LIBRARIES})
    ELSE ()
      SET (OPENGL_FOUND)
    ENDIF ()
  ELSE (X11_FOUND)
    SET (OPENGL_FOUND)
  ENDIF (X11_FOUND)
ENDIF (APPLE AND NOT without-gl)

IF (NOT OPENGL_FOUND)
  SET (OPENGL_INCLUDE_DIR "")
ENDIF ()

# FOREACH (var AGL_FOUND AGL_INCLUDE_DIR AGL_LIBRARIES OPENGL_FOUND OPENGL_INCLUDE_DIR OPENGL_LIBRARIES)
#   MESSAGE ("${var} = ${${var}}")
# ENDFOREACH ()

TEST_BIG_ENDIAN (WORDS_BIGENDIAN)
IF (WORDS_BIGENDIAN)
  SET (MSB_FIRST 1)
ELSE ()
  SET (LSB_FIRST 1)
ENDIF ()

CONFIG_DEFINE (WORDS_BIGENDIAN)
CONFIG_DEFINE (LSB_FIRST)
CONFIG_DEFINE (MSB_FIRST)

CHECK_FUNCTION_EXISTS (atexit		HAVE_ATEXIT)
CHECK_FUNCTION_EXISTS (on_exit		HAVE_ON_EXIT)

IF (HAVE_ATEXIT)
  SET (AT_EXIT atexit)
ELSEIF (HAVE_ON_EXIT)
  SET (AT_EXIT on_exit)
ENDIF ()

CONFIG_DEFINE (AT_EXIT)

# sqMemoryAccess.h

CHECK_INCLUDE_FILE (interp.h HAVE_INTERP_H -I${src}/vm)

CONFIG_DEFINE (HAVE_INTERP_H)

CHECK_TYPE_SIZE (int SIZEOF_INT)
CHECK_TYPE_SIZE (long SIZEOF_LONG)
CHECK_TYPE_SIZE ("long long" SIZEOF_LONG_LONG)
CHECK_TYPE_SIZE ("void *" SIZEOF_VOID_P)

CONFIG_DEFINE (SIZEOF_INT)
CONFIG_DEFINE (SIZEOF_LONG)
CONFIG_DEFINE (SIZEOF_LONG_LONG)
CONFIG_DEFINE (SIZEOF_VOID_P)

TRY_RUN (DOUBLE_WORD_ALIGNMENT tmp ${bld} ${config}/testDoubleWordAlignment.c)
TRY_RUN (DOUBLE_WORD_ORDER tmp ${bld} ${config}/testDoubleWordOrder.c)

CONFIG_DEFINE (DOUBLE_WORD_ALIGNMENT)
CONFIG_DEFINE (DOUBLE_WORD_ORDER)

# sqPlatformSpecific.h

CHECK_INCLUDE_FILE (alloca.h HAVE_ALLOCA_H)

CONFIG_DEFINE (HAVE_ALLOCA_H)

IF (HAVE_ALLOCA_H)
  SET (HAVE_ALLOCA 1)
ELSE ()
  CHECK_FUNCTION_EXISTS (alloca HAVE_ALLOCA)
ENDIF ()

CONFIG_DEFINE (HAVE_ALLOCA)

# aio.c

CHECK_INCLUDE_FILE (sys/time.h HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILE (sys/filio.h HAVE_SYS_FILIO_H)

CONFIG_DEFINE (HAVE_SYS_TIME_H)
CONFIG_DEFINE (HAVE_SYS_FILIO_H)

CHECK_FUNCTION_EXISTS (nanosleep HAVE_NANOSLEEP)

CONFIG_DEFINE (HAVE_NANOSLEEP)

# sqUnixCharConv.c

CHECK_INCLUDE_FILE (iconv.h HAVE_ICONV_H)
CHECK_INCLUDE_FILE (langinfo.h HAVE_LANGINFO_H)

IF (HAVE_LANGINFO_H)
  TRY_COMPILE (HAVE_LANGINFO_CODESET ${bld} ${config}/testLanginfoCodeset.c)
ENDIF (HAVE_LANGINFO_H)

CONFIG_DEFINE (HAVE_ICONV_H)
CONFIG_DEFINE (HAVE_LANGINFO_CODESET)

# sqUnixExternalPrims.c

CHECK_INCLUDE_FILE (dlfcn.h HAVE_DLFCN_H)

CHECK_LIBRARY_EXISTS (dl dlopen "" HAVE_LIBDL)
IF (HAVE_LIBDL)
  USE_LIBRARY (dl)
ENDIF (HAVE_LIBDL)
IF (HAVE_LIBDL)
  SET (HAVE_DLOPEN 1)
ELSE ()
  CHECK_FUNCTION_EXISTS (dlopen HAVE_DLOPEN)
ENDIF (HAVE_LIBDL)

CHECK_FUNCTION_EXISTS (_dyld_present HAVE_DYLD)
CHECK_FUNCTION_EXISTS (snprintf HAVE_SNPRINTF)
CHECK_FUNCTION_EXISTS (__snprintf HAVE___SNPRINTF)

CONFIG_DEFINE (HAVE_DLFCN_H)
CONFIG_DEFINE (HAVE_DLOPEN)
CONFIG_DEFINE (HAVE_DYLD)
CONFIG_DEFINE (HAVE_SNPRINTF)
CONFIG_DEFINE (HAVE___SNPRINTF)

SET (VM_X11DIR \"${X11_LIBRARY_DIR}\")

CONFIG_DEFINE (VM_X11DIR)

SET (VM_MODULE_PREFIX \"${CMAKE_SHARED_MODULE_PREFIX}\")

CONFIG_DEFINE (VM_MODULE_PREFIX)

SET (CMAKE_SHARED_MODULE_PREFIX "so.")
SET (CMAKE_SHARED_MODULE_SUFFIX "")

SET (MODULE_PREFIX  \"${CMAKE_SHARED_MODULE_PREFIX}\")
SET (MODULE_SUFFIX  \"${CMAKE_SHARED_MODULE_SUFFIX}\")
SET (LIBRARY_PREFIX \"${CMAKE_SHARED_LIBRARY_PREFIX}\")
SET (LIBRARY_SUFFIX \"${CMAKE_SHARED_LIBRARY_SUFFIX}\")

CONFIG_DEFINE (MODULE_PREFIX)
CONFIG_DEFINE (MODULE_SUFFIX)
CONFIG_DEFINE (LIBRARY_PREFIX)
CONFIG_DEFINE (LIBRARY_SUFFIX)

# sqUnixMain.c

SET (VM_BUILD_STRING "\"Unix built on \"__DATE__ \" \"__TIME__\" Compiler: \"__VERSION__")

CONFIG_DEFINE (VM_BUILD_STRING)

SET (VM_LIBDIR "\"${prefix}/${plgdir}\"")

CONFIG_DEFINE (VM_LIBDIR)

SET (VM_HOST        \"${VM_HOST}\")
SET (VM_HOST_CPU    \"${VM_HOST_CPU}\")
SET (VM_HOST_VENDOR \"${VM_HOST_VENDOR}\")
SET (VM_HOST_OS     \"${VM_HOST_OS}\")

CONFIG_DEFINE (VM_HOST)
CONFIG_DEFINE (VM_HOST_CPU)
CONFIG_DEFINE (VM_HOST_VENDOR)
CONFIG_DEFINE (VM_HOST_OS)

SET (VM_VERSION \"${VM_VERSION}\")

CONFIG_DEFINE (VM_VERSION)
CONFIG_DEFINE (SQ_VERSION)

CHECK_FUNCTION_EXISTS (tzset	HAVE_TZSET)
CHECK_VARIABLE_EXISTS (timezone	HAVE_TIMEZONE)
CHECK_STRUCT_HAS_MEMBER ("struct tm" tm_gmtoff time.h HAVE_TM_GMTOFF)

CONFIG_DEFINE (HAVE_TZSET)
CONFIG_DEFINE (HAVE_TIMEZONE)
CONFIG_DEFINE (HAVE_TM_GMTOFF)

CONFIG_DEFINE (IMAGE_DUMP)

# sqUnixMemory.c

CHECK_FUNCTION_EXISTS (mmap HAVE_MMAP)

CONFIG_DEFINE (HAVE_MMAP)

# OLPC

IF (NOT without-SUGAR)
  SET (SUGAR 1)
ENDIF ()

CONFIG_DEFINE (SUGAR)
