AC_MSG_CHECKING([for Ogg Vorbis and Speex support])
AC_TRY_COMPILE([
  #include <vorbis/codec.h>
  #include <vorbis/vorbisenc.h>
  #include <speex/speex.h>
],[;],[
  AC_MSG_RESULT(yes)
],[
  AC_MSG_RESULT(no)
  AC_PLUGIN_DISABLE
])
