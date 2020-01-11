/*
 *  view.h
 *
 *  Copyright (C) 1997, 1998, 2006, 2020 Staf Wagemakers Belgie/Belgium
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

#ifndef __VIEW_H
#include "config.h"
#include "common.h"
#include "xstring.h"
#include "win.h"
#include "getline.h"
#define TAB 9
typedef struct 
{
ssize_t y,y_max,size;
ssize_t x,x_max;
char    mode;
char    **file;
char    *filename;
char    load,cmd;
char    *txt_f_open2,*txt_f_open1,*txt_f_load;
char    *txt_f_fseek;
char    *txt_f_freopen;
char    ***view_exec;
MENU    *m_ok;
WINDOW  *win;
chtype  *color;
int     cols,lines;
int     sx,sy;
} VIEW_PAR;
VIEW_PAR *view_par (VIEW_PAR *par);
void view_free();
void view_free_file();
int  view_load ();
char *view_getstr(unsigned long y);
unsigned view_strlen(char *c);
unsigned long view_gety();
unsigned long view_getx();
void view_setx(long x);
void view_sety(unsigned long y);
void view_addch(char *c);
void view_addstr (char *s);
void view_move (unsigned long y, long x);
void view_addline(int yp,unsigned long r);
void view_refresh();
void view_down();
void view_up();
void view_previous();
void view_right();
void view_left();
char * view_charstr(char *c);
unsigned view_charstr_len(char *c);
#endif
