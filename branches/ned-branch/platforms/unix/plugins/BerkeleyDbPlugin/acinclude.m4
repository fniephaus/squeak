# AC_PLUGIN_CHECK_LIB(db4,db_create)
AC_CHECK_HEADERS(db.h)
AC_TRY_LINK(#include "db.h", void *xx = (void*)(&db_create);,
    [plibs="${plibs} db"],
    AC_MSG_RESULT([******** disabling BerkeleyDbPlugin due to missing libraries])
    disabled_plugins="${disabled_plugins} BerkeleyDbPlugin")
