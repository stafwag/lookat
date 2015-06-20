dnl Process this file with autoconf to produce a configure script.

AC_INIT(lookat_bekijk,1.4.4)
AC_CANONICAL_SYSTEM
AC_CONFIG_SRCDIR(bekijk.c)
AM_INIT_AUTOMAKE


AM_CONFIG_HEADER(config.h)

OS_NAME=$build_os

AC_PROG_CC
AC_CHECK_FUNCS([getcwd],,AC_MSG_ERROR(not found))
AC_CHECK_FUNCS(getdelim)
AC_CHECK_FUNCS(getline)
AC_CHECK_FUNCS([select],,AC_MSG_ERROR(not found))
AC_CHECK_FUNCS([strstr],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([sys/select.h],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([stddef.h],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([stdlib.h],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([string.h],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([sys/time.h],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([unistd.h],,AC_MSG_ERROR(not found))
AC_CHECK_HEADERS([ncurses.h],FOUND_NCURSES_H=1)
AC_CHECK_HEADERS([ncurses/ncurses.h],FOUND_NCURSES_H=1)

if test "$FOUND_NCURSES_H" != "1"; then

	AC_MSG_ERROR(ncurses_h not found)

fi

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
AC_HEADER_STDC
AC_HEADER_TIME
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S 
AC_TYPE_SIGNAL
AC_TYPE_SIZE_T
AC_CHECK_LIB(ncurses,initscr,,

dnl
dnl *BSD uses ncurses as their default curses lib
dnl

AC_MSG_CHECKING([*BSD system?])
if echo $OS_NAME | grep -i bsd > /dev/null 2>&1; then
   AC_MSG_RESULT(yes - assuming libcurses is ncurses)
   AC_CHECK_LIB(curses,initscr,,AC_MSG_ERROR(curses lib not found))
else
   AC_MSG_RESULT(no)
   AC_MSG_ERROR(no valid ncurses lib found)
fi
)

AC_OUTPUT(Makefile)