AC_CHECK_HEADER(ffi.h,
  AC_CHECK_LIB(ffi, ffi_call,
    AC_PLUGIN_USE_LIB(ffi),
    AC_PLUGIN_DISABLE),
  AC_PLUGIN_DISABLE)
