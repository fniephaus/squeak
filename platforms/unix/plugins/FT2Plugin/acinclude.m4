if test x$freetype_exec_prefix != x ; then
     freetype_args="$freetype_args --exec-prefix=$freetype_exec_prefix"
     if test x${FREETYPE_CONFIG+set} != xset ; then
        FREETYPE_CONFIG=$freetype_exec_prefix/bin/freetype-config
     fi
fi
if test x$freetype_prefix != x ; then
     freetype_args="$freetype_args --prefix=$freetype_prefix"
     if test x${FREETYPE_CONFIG+set} != xset ; then
        FREETYPE_CONFIG=$freetype_prefix/bin/freetype-config
     fi
fi

AC_PATH_PROG(FREETYPE_CONFIG, freetype-config, no)
no_freetype=""
if test "$FREETYPE_CONFIG" = "no" ; then
    AC_MSG_ERROR([
*** Unable to find FreeType2 library (http://www.freetype.org/)
])
fi

# This is equivalent to Freetype2 2.17, I think
# AC_CHECK_FT2([9.5.3],[],[AC_MSG_ERROR([ *** Unable to find FreeType2 library (http://www.freetype.org/) ])])

AC_PLUGIN_CHECK_LIB(freetype,FT_Library_Version)
AC_PLUGIN_DEFINE_UNQUOTED(freetype_config,$FREETYPE_CONFIG)

