/*
 *  input.h
 *
 *  Copyright (C) 1997, 1998, 2006, 2016  Staf Wagemakers Belgie/Belgium
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

#ifndef __INPUT_H
#define __INPUT_H
#include "config.h"
#include "common.h"
#define NCURSES_INTERNALS 1
#include "ncurses_h.h"
typedef struct {
	WINDOW *w;	/* input vensters				 */
	int  yp,xp;	/* y,x positie in het venster w			 */
        char *c;	/* De in te lezen string			 */
	unsigned n,m;	/* aantal kar's op scherm, max. toegelaten kar's */
	unsigned count;	/* huidige positie				 */
	unsigned ox;	/* xoffset string				 */
	bool insert;    /* Insert mode					 */
	char mode;	/* Input mode 0 -> normale string		 */
			/*            1 -> dec. string			 */
			/*            2 -> hex. string			 */
	int  *key;	/* pointer naar ekstra toetsen			 */
} INPUT_STRING;
int input (INPUT_STRING *p);
void input_show_string (INPUT_STRING *p);
#endif


