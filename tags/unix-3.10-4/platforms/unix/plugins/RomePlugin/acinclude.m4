PKG_CHECK_MODULES(PANGOCAIRO,pangocairo,,AC_PLUGIN_DISABLE)
PKG_CHECK_MODULES(FREETYPE,freetype2,,AC_PLUGIN_DISABLE)
AC_PLUGIN_DEFINE_UNQUOTED(FREETYPE_CFLAGS,$FREETYPE_CFLAGS)
AC_PLUGIN_DEFINE_UNQUOTED(FREETYPE_LIBS,$FREETYPE_LIBS)
AC_PLUGIN_DEFINE_UNQUOTED(PANGOCAIRO_CFLAGS,$PANGOCAIRO_CFLAGS)
AC_PLUGIN_DEFINE_UNQUOTED(PANGOCAIRO_LIBS,$PANGOCAIRO_LIBS);