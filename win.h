/*
 *  win.h
 *
 *  Copyright (C) 1997, 1998, 2001, 2006  Staf Wagemakers Belgie/Belgium
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef __VENSTER_H
#define __VENSTER_H
#include "common.h"
#include "ncurses_h.h"
#include "xstring.h"
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#if TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#include "menu.h"
#include "get_dir.h"
#include "input.h"
void waddch_fix (WINDOW *w,char c);
void win_hline(WINDOW *w,int h);
void win_vline(WINDOW *w,int v);
void win_box(WINDOW *w,int y,int x,int yp,int xp);
WINDOW * open_win(int yy,int xx,int y,int x);
WINDOW * open_cwin(int yy,int xx);
void open_okwin(int yy,int xx,MENU *m,char **txt,WINDOW *win1);
void open_animwin(int yy,int xx,MENU *m,char **txt,char *animtxt,WINDOW *win1,char mode);
void m_open(MENU *m,WINDOW *win1);
char *open_filewin(WINDOW *win2,char **txt,chtype *kleur,WINDOW *win1,int (*ends)());
int open_inputwin (int yy,int xx,MENU *m,chtype kleur,INPUT_STRING *read_str,char **txt,char mode,WINDOW *win1);
void wexit(int i);
int open_ynwin (int yy,int xx,MENU *m,char **txt,WINDOW *win1);
#endif


