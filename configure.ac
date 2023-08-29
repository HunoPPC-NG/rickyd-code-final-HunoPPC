dnl Process this file with autoconf to produce a configure script.

# Save the original $CFLAGS so we can distinguish whether the user set those
# in the environment, or whether autoconf added -O and -g options:
ORIGINAL_CFLAGS="$CFLAGS"

dnl ---------------------------------------------------------------------------
dnl CF_STRIP_MWINDOWS_OPT version: 3 updated: 2002/12/21 19:25:52
dnl --------------
dnl	Remove "-mwindows" option from the compiler options
AC_DEFUN([CF_STRIP_MWINDOWS_OPT],
[$1=`echo ${$1} | sed -e 's%-mwindows %%' -e 's%-mwindows$%%'`])

dnl --------------------------------------------------------------------------
dnl Package definitions and initialization
dnl --------------------------------------------------------------------------
m4_define([rickyd_major], [0])
m4_define([rickyd_minor], [2])
m4_define([rickyd_micro], [0])
m4_define([rickyd_version],
          [rickyd_major.rickyd_minor.rickyd_micro])

AC_INIT([rickyd],[rickyd_version],[http://rickyd.sourceforge.net/])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

dnl Compute the canonical target-system type variable (target_cpu, target_vendor, target_os)
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([gnu])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS(config.h)
AM_MAINTAINER_MODE(disable)
#AM_MAINTAINER_MODE


# autoconf 2.5x defaults to no cache file; we need the cache file's information
# for building the config page.  But start with it empty to avoid confusion by
# people who don't do a "make distclean" after applying patches.
cache_file=config.cache
rm -f config.cache; touch config.cache

AC_PREFIX_DEFAULT(/usr/local)

dnl -----------------------------------------------
dnl Checks for programs.
dnl -----------------------------------------------

AC_PROG_CC
AC_PROG_CXX

AM_SANITY_CHECK

AC_LANG_CPLUSPLUS

AC_PREFIX_DEFAULT(/usr/local)

AC_SUBST(CFLAGS)

dnl---------------------------------------------------------------------------
dnl Check host
dnl---------------------------------------------------------------------------

HOST_WIN32='no'
HOST_MINGW='no'
HOST_CYGWIN='no'
HOST_UNIX='no'

echo "checking target... $target"

case "$target" in
  *-cygwin*)
    HOST_WIN32='yes'
    HOST_CYGWIN='yes'
    AC_DEFINE([__WIN32],[1],[Define if is target windows])
    AC_DEFINE([__CYGWIN],[1],[Define if is target CYGWIN])
    ;;
  *-mingw*)
    HOST_WIN32='yes'
    HOST_MINGW='yes'
    AC_DEFINE([__WIN32],[1],[Define if is target windows])
    AC_DEFINE([__MINGW],[1],[Define if is target MINGW])
    ;;
  *)
    HOST_UNIX='yes'
    AC_DEFINE([__UNIX],[1],[Define if is target a unix system])
    ;;
esac

if test x$HOST_WIN32 = xyes; then
  AC_CHECK_TOOL(WINDRES, windres, :)
else
  WINDRES=":"
fi

AM_CONDITIONAL(HAVE_WINDRES, test "x$WINDRES" != "x:")
AC_SUBST(WINDRES)

AM_CONDITIONAL(__WIN32, test x$HOST_WIN32 = xyes)
AM_CONDITIONAL(__CYGWIN, test x$HOST_CIGWIN = xyes)
AM_CONDITIONAL(__MINGW, test x$HOST_MINGW = xyes)
AM_CONDITIONAL(__UNIX, test x$HOST_UNIX = xyes)

dnl --------------------------------------------------------------------------
dnl check for dependencies
dnl --------------------------------------------------------------------------

dnl --------------------------------------------------------------------------
dnl check for dependencies
dnl --------------------------------------------------------------------------
PKG_CHECK_MODULES(DEPS, [sdl2 SDL2_image SDL2_mixer SDL2_ttf])
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

if test x$HOST_MINGW = xyes; then

AC_MSG_CHECKING(if you want to enable console mode)
AC_ARG_ENABLE(mingw-console, AS_HELP_STRING([--enable-mingw-console],
		[enable console mode (default: disabled)]),
	[enable_console=yes],
	[enable_console=no])

AC_MSG_RESULT($enable_console)

if test x$enable_console = xyes; then
CF_STRIP_MWINDOWS_OPT(DEPS_LIBS)
fi

fi

if test x$HOST_WIN32 = xyes; then

	enable_debian_menu=no
	enable_desktop=no

else

dnl --------------------------------------------------------------------------
dnl set/unset debian menu
dnl --------------------------------------------------------------------------
AC_MSG_CHECKING(if you want to enable debian menu)
AC_ARG_ENABLE(debian-menu, AS_HELP_STRING([--disable-debian-menu],
		[disable debian menu (default: enabled)]),
	[enable_debian_menu=no],
	[enable_debian_menu=yes])

AC_MSG_RESULT($enable_debian_menu)

dnl --------------------------------------------------------------------------
dnl set/unset desktop file
dnl --------------------------------------------------------------------------
AC_MSG_CHECKING(if you want to enable desktop file)
AC_ARG_ENABLE(desktop, AS_HELP_STRING([--disable-desktop],
		[disable desktop file (default: enabled)]),
	[enable_desktop_file=no],
	[enable_desktop_file=yes])

AC_MSG_RESULT($enable_desktop_file)

fi

AM_CONDITIONAL(DEBIAN_MENU, test "$enable_debian_menu" = yes)
AM_CONDITIONAL(DESKTOP_FILE, test "$enable_desktop_file" = yes)


dnl --------------------------------------------------------------------------
dnl Check if the processor stores words with the most significant byte first
dnl (like Motorola and SPARC, unlike Intel and VAX).
dnl --------------------------------------------------------------------------

AC_C_BIGENDIAN

dnl---------------------------------------------------------------------------
dnl Create the Makefiles
dnl---------------------------------------------------------------------------
AC_CONFIG_FILES([
 Makefile
 sources/Makefile
 data/Makefile
 data/ricky.desktop.in
 data/ricky.in
 data/Maps/Makefile
 data/Resources/Makefile
 data/Resources/sounds/Makefile
 data/Resources/tilesets/Makefile
 data/Resources/fonts/Makefile
])
AC_OUTPUT

dnl Output the results
AC_MSG_NOTICE([

  rickyd $VERSION
  ----------------------
  Target           : ${target}

  Prefix           : ${prefix}

  Desktop File     : ${enable_desktop_file}
  Debian Menu      : ${enable_debian_menu}

])

if test x$HOST_MINGW = xyes; then
echo "  Console mode     : ${enable_console}"
fi
