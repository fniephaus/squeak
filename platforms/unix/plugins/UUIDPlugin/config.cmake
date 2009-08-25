CHECK_INCLUDE_FILE (uuid.h HAVE_UUID_H)
CHECK_INCLUDE_FILE (uuid/uuid.h HAVE_UUID_UUID_H)

IF (NOT HAVE_UUID_H AND NOT HAVE_UUID_UUID_H)
  PLUGIN_DISABLE ()
ELSE ()
  CHECK_LIBRARY_EXISTS (uuid uuid_generate "" HAVE_LIBUUID)
  IF (HAVE_LIBUUID)
    PLUGIN_LINK_LIBRARIES (uuid)
  ENDIF ()
ENDIF ()

CONFIG_DEFINE (HAVE_UUID_H)
CONFIG_DEFINE (HAVE_UUID_UUID_H)
