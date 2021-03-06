PLUGIN_FIND_INCLUDE (UUID uuid.h)
CONFIG_DEFINE (HAVE_UUID_H)

PLUGIN_FIND_INCLUDE (SYS_UUID sys/uuid.h)
CONFIG_DEFINE (HAVE_SYS_UUID_H)

PLUGIN_FIND_INCLUDE (UUID_UUID uuid/uuid.h)
CONFIG_DEFINE (HAVE_UUID_UUID_H)

PLUGIN_FIND_LIBRARY (UUID uuid)

IF (HAVE_UUID_H OR HAVE_SYS_UUID_H OR HAVE_UUID_UUID_H)
  IF (HAVE_LIBUUID)
    SET (CMAKE_REQUIRED_LIBRARIES uuid)
  ENDIF (HAVE_LIBUUID)
ELSE ()
  PLUGIN_DISABLE ()
ENDIF (HAVE_UUID_H OR HAVE_SYS_UUID_H OR HAVE_UUID_UUID_H)

CHECK_FUNCTION_EXISTS (uuidgen HAVE_UUIDGEN)
CONFIG_DEFINE (HAVE_UUIDGEN)

CHECK_FUNCTION_EXISTS (uuid_generate HAVE_UUID_GENERATE)
CONFIG_DEFINE (HAVE_UUID_GENERATE)

CHECK_FUNCTION_EXISTS (uuid_create HAVE_UUID_CREATE)
CONFIG_DEFINE (HAVE_UUID_CREATE)

IF (NOT HAVE_UUIDGEN AND NOT HAVE_UUID_GENERATE AND NOT HAVE_UUID_CREATE)
  PLUGIN_DISABLE ()
ENDIF (NOT HAVE_UUIDGEN AND NOT HAVE_UUID_GENERATE AND NOT HAVE_UUID_CREATE)

# IF (UUID_UUID_GENERATE)
#   SET (HAVE_UUID_GENERATE 1)
#   PLUGIN_LINK_LIBRARIES (uuid)
# ENDIF (UUID_UUID_GENERATE)
