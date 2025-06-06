/*
 *  lookat_configfile.c
 *
 *  Copyright (C) 2003  Staf Wagemakers Belgie/Belgium
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

#include "xmalloc.h"
#include "configfile.h"
#include <stdio.h>
#include <errno.h>

char * lookat_get_config(FILE *fp, char *var_name) {
	char **cc=NULL;
	char *cp=NULL;
	char *ret=NULL;

	cc=get_config_array(fp,var_name);
	if(cc==NULL) return(NULL);
	if(!strcmp(cc[0],"=")) {
	  if(cc[1]!=NULL) cp=cc[1];
	}
	else {
		cp=cc[0];
	}

	if(cp!=NULL) {
		ret = xmalloc(strlen(cp)+1);
		ret = strcpy(ret,cp);
	}

	free_string_array(cc);
	return(ret);
}

char ** lookat_get_next_config (FILE *fp) {
	char **cc=NULL;
	char *var_name=NULL,*var_value=NULL;
	char *cp=NULL;
	char **ret=NULL;

	cc=get_next_config_array(fp);
	if(cc==NULL) return(NULL);
	
	if(cc[0]!=NULL) {
		var_name=xmalloc(strlen(cc[0])+1);
		strcpy(var_name,cc[0]);
	}
	
	if(cc[1]!=NULL) {
	  if(!strcmp(cc[1],"=")) cp=cc[2];
	    else cp=cc[1];

	  if(cp!=NULL) {
		var_value=xmalloc(strlen(cp)+1);
		strcpy(var_value,cp);
	  }
	}

	if((var_name!=NULL)&&(var_value!=NULL)) {
		ret=xcalloc(3,sizeof(char *));
		ret[0]=var_name;
		ret[1]=var_value;
	}

	free_string_array(cc);

	return(ret);
}
