AC_MSG_CHECKING([for Cairo library])
AC_CHECK_HEADERS([cairo.h],AC_PLUGIN_CHECK_LIB(cairo,cairo_create),[
  AC_MSG_RESULT([******* not adding Freetype2 support to Cairo plugin])
  AC_PLUGIN_DISABLE])

AC_MSG_CHECKING([for Cairo library Freetype and Fontconfig support])
AC_CHECK_HEADERS([cairo.h cairo-ft.h ft2build.h fontconfig.h],
  [AC_CHECK_LIB(cairo,cairo_ft_font_create,
    [AC_MSG_RESULT([******* found Freetype2 support for Cairo plugin])
	plibs="${plibs} freetype fontconfig"],
    [AC_MSG_RESULT([******* not adding Freetype2 support to Cairo plugin])],
	[-lfreetype -lfontconfig])])
