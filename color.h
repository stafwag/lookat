/*
 *
 * color.h
 * 
 *
 * Copyright (C) 2003, 2006 Staf Wagemakers Belgie/Belgium
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef __COLOR_H
#define __COLOR_H

#include "common.h"
#include "xstring.h"
#include "ncurses_h.h"
#include <stdio.h>
#include "configfile.h"

struct color {
       chtype  fg;
       chtype  bg;
       chtype  attr;
       chtype  ch;
       short   color_pair;
       char    *name;
};
struct color * new_color_table ( char ** color_vars );
void delete_color_table( struct color * color_table );
void read_config_colors(FILE *fp, struct color *color_table, int mode );
chtype get_color(char *name,struct color *color_table);
int set_color(char *name,struct color *color_table,chtype fg,chtype bg,chtype attr,int color_mode);
void save_colors(FILE *fp,struct color *colors,int color_mode);
int color_to_number(chtype color);
int attr_to_number(chtype color);
chtype number_to_color(int n);
chtype number_to_attr(int n);
void cp_color(struct color *color1,struct color *color2);
int color_array_members (struct color *color_array);
void cp_color_array(struct color *color1_array,struct color *color2_array);
void reinit_color_table (struct color *colors,int mode);

#endif


