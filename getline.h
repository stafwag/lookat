/*  Copyright (C) 1995, 2000, 2001 Free Software Foundation, Inc.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
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

#include "config.h"
#ifndef GETLINE_H_
# define GETLINE_H_ 1
# define __USE_GNU 1

# include <stddef.h>
# include <stdio.h>

# ifndef PARAMS
#  if __STDC__ || defined __GNUC__ || defined __SUNPRO_C || defined __cplusplus || __PROTOTYPES
#   define PARAMS(args) args
#  else
#   define PARAMS(args) ()
#  endif
# endif

/* glibc2 has these functions declared in <stdio.h>.  Avoid redeclarations.  */
# if __GLIBC__ < 2

#ifndef HAVE_GETLINE 

int
getline PARAMS ((char **_lineptr, size_t *_n, FILE *_stream));

#endif

#ifndef HAVE_GETDELIM 

#if defined(__FreeBSD__)
#include <osreldate.h>
#if __FreeBSD_version <= 800066
int
getdelim PARAMS ((char **_lineptr, size_t *_n, int _delimiter, FILE *_stream));
#endif
#endif

#endif

# endif

#endif /* not GETLINE_H_ */


