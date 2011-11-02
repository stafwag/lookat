/*
 *  menu.h
 *
 *  Copyright (C) 1997, 1998, 2006  Staf Wagemakers Belgie/Belgium
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __MENU_H
#define __MENU_H
#include "config.h"
#include "common.h"
#include "ncurses_h.h"
typedef struct m {
  WINDOW *w;            /* menu venster         */
  chtype color1;        /* normale kleur        */
  chtype color2;        /* kleur bij selektie   */
  chtype color3;        /* highlight kleur      */
  chtype color4;
  unsigned char used;
  unsigned sel;
  unsigned amount;
  unsigned *place;
  unsigned l;
  int      *key;
  int      *hkey;       /* highlight keys       */
  unsigned *hplace;     /* highlight place      */
  char     **txt;
  struct m **next;
  struct m *parent;
  void     (*call_open)(struct m *menu);
  void     (*call_close)(struct m *menu);
  } MENU;

void menu_print(MENU *m);
void menu_sel(MENU *m,unsigned s);
int menu_key (MENU *m);
void menu_pull (MENU *m,unsigned submenu);
int scroll_menu(MENU *m,unsigned g,void (*call_proc)(MENU *m),unsigned *u);
void menu_print_item(MENU *m,unsigned i,chtype hcolor);
void menu_hletter(MENU *m,unsigned i,chtype hcolor);
#endif


