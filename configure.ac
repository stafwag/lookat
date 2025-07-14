dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([lookat_bekijk],[2.1.0rc2],[staf@wagemakers.be])
AC_CANONICAL_TARGET
AC_CONFIG_SRCDIR(bekijk.c)
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE([disable])


AM_CONFIG_HEADER(config.h)

AC_USE_SYSTEM_EXTENSIONS

OS_NAME=$build_os

AC_PROG_CC
AC_CHECK_FUNCS([getcwd],,AC_MSG_ERROR(not found))
AC_CHECK_FUNCS(getdelim)
AC_CHECK_FUNCS(getline)
AC_CHECK_FUNCS([select],,AC_MSG_ERROR(not found))
AC_CHECK_FUNCS([strstr],,AC_MSG_ERROR(not found))
AC_CHECK_FUNCS([strcasestr])
AC_CHECK_HEADERS([sys/select.h],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([stddef.h],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([stdlib.h],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([string.h],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([sys/time.h],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([unistd.h],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([ncurses.h],FOUND_NCURSES_H=1)
AC_CHECK_HEADERS([ncurses/ncurses.h],FOUND_NCURSES_H=1)
AC_CHECK_HEADERS([ncursesw/ncurses.h],FOUND_NCURSES_H=1)

if test "$FOUND_NCURSES_H" != "1"; then

	AC_MSG_ERROR(ncurses_h not found)

fi

AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([strcasecmp])
AC_CHECK_FUNCS([strncasecmp])
AC_CHECK_FUNCS([strrchr])
AC_TYPE_SSIZE_T

AC_CHECK_HEADERS([ctype.h])
AC_CHECK_HEADERS([malloc.h])
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_STAT
AC_C_CONST
AC_HEADER_STDBOOL
AC_HEADER_DIRENT
m4_warn([obsolete],
[The preprocessor macro 'STDC_HEADERS' is obsolete.
  Except in unusual embedded environments, you can safely include all
  C89 headers unconditionally.])dnl
# Autoupdate added the next two lines to ensure that your configure
# script's behavior did not change.  They are probably safe to remove.
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

m4_warn([obsolete],
[Update your code to rely only on HAVE_SYS_TIME_H,
then remove this warning and the obsolete code below it.
All current systems provide time.h; it need not be checked for.
Not all systems provide sys/time.h, but those that do, all allow
you to include it and time.h simultaneously.])dnl
AC_CHECK_HEADERS_ONCE([sys/time.h])
# Obsolete code to be removed.
if test $ac_cv_header_sys_time_h = yes; then
  AC_DEFINE([TIME_WITH_SYS_TIME],[1],[Define to 1 if you can safely include both <sys/time.h>
	     and <time.h>.  This macro is obsolete.])
fi
# End of obsolete code.

AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
m4_warn([obsolete],
[your code may safely assume C89 semantics that RETSIGTYPE is void.
Remove this warning and the 'AC_CACHE_CHECK' when you adjust the code.])dnl
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[AC_COMPILE_IFELSE(
[AC_LANG_PROGRAM([#include <sys/types.h>
#include <signal.h>
],
		 [return *(signal (0, 0)) (0) == 1;])],
		   [ac_cv_type_signal=int],
		   [ac_cv_type_signal=void])])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[$ac_cv_type_signal],[Define as the return type of signal handlers
		    ('int' or 'void').])

AC_TYPE_SIZE_T
AC_CHECK_LIB(ncursesw,initscr,,

dnl
dnl OpenBSD uses ncurses as their default curses lib
dnl

AC_MSG_CHECKING([OpenBSD system?])
if echo $OS_NAME | grep -i openbsd > /dev/null 2>&1; then
   AC_MSG_RESULT(yes - assuming libcurses is ncurses)
   AC_CHECK_LIB(curses,initscr,,AC_MSG_ERROR(curses lib not found))
else
   AC_MSG_RESULT(no)
   AC_CHECK_LIB(ncurses,initscr,,

   dnl
   dnl OpenBSD uses ncurses as their default curses lib
   dnl

   AC_MSG_CHECKING([OpenBSD system?])
   if echo $OS_NAME | grep -i openbsd > /dev/null 2>&1; then
      AC_MSG_RESULT(yes - assuming libcurses is ncurses)
      AC_CHECK_LIB(curses,initscr,,AC_MSG_ERROR(curses lib not found))
   else
      AC_MSG_RESULT(no)
      AC_MSG_ERROR(no valid ncurses lib found)
   fi
  )
fi
)


AC_CONFIG_FILES([Makefile])
AC_OUTPUT
