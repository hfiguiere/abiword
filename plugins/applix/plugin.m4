
APPLIX_CFLAGS=
APPLIX_LIBS=

if test "$enable_applix" == "yes"; then

APPLIX_CFLAGS="$APPLIX_CFLAGS "'${PLUGIN_CFLAGS}'
APPLIX_LIBS="$APPLIX_LIBS "'${PLUGIN_LIBS}'

if test "$enable_applix_builtin" == "yes"; then
	APPLIX_CFLAGS="$APPLIX_CFLAGS -DABI_PLUGIN_BUILTIN"
fi

fi

AC_SUBST([APPLIX_CFLAGS])
AC_SUBST([APPLIX_LIBS])

