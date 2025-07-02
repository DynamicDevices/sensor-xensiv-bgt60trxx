AC_INIT([xensiv-bgt60trxx], [1.1.0], [support@dynamicdevices.co.uk], [xensiv-bgt60trxx], [https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_AR

# Check for required headers
AC_CHECK_HEADERS([stdint.h stdbool.h])

# Platform support options
AC_ARG_ENABLE([linux-support],
    AS_HELP_STRING([--enable-linux-support], [Enable Linux platform support (default: yes)]),
    [enable_linux_support=$enableval],
    [enable_linux_support=yes])

AC_ARG_ENABLE([examples],
    AS_HELP_STRING([--enable-examples], [Build example applications (default: yes)]),
    [enable_examples=$enableval],
    [enable_examples=yes])

AC_ARG_ENABLE([debug],
    AS_HELP_STRING([--enable-debug], [Enable debug build (default: no)]),
    [enable_debug=$enableval],
    [enable_debug=no])

# Set conditionals for Automake
AM_CONDITIONAL([ENABLE_LINUX_SUPPORT], [test "x$enable_linux_support" = "xyes"])
AM_CONDITIONAL([ENABLE_EXAMPLES], [test "x$enable_examples" = "xyes"])

# Set compiler flags
if test "x$enable_debug" = "xyes"; then
    CFLAGS="$CFLAGS -g -O0 -DDEBUG"
else
    CFLAGS="$CFLAGS -O2 -DNDEBUG"
fi

# Define preprocessor macros for enabled features
if test "x$enable_linux_support" = "xyes"; then
    AC_DEFINE([ENABLE_LINUX_SUPPORT], [1], [Enable Linux platform support])
fi

AC_CONFIG_FILES([
    Makefile
    examples/Makefile
    xensiv_bgt60trxx.pc
])

AC_OUTPUT

echo ""
echo "Configuration Summary:"
echo "  Linux support: $enable_linux_support"
echo "  Examples: $enable_examples"
echo "  Debug: $enable_debug"
echo ""