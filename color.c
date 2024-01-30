/*
 * color.c
 *
 * Copyright (C) 2003, 2007, 2022, 2024 Staf Wagemakers Belgie/Belgium
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

#include "color.h"

chtype color_ch[]={ COLOR_BLACK,COLOR_RED,COLOR_GREEN,COLOR_YELLOW,
	            COLOR_BLUE,COLOR_MAGENTA,COLOR_CYAN,COLOR_WHITE };
char *color_names[] = {"black","red","green","yellow","blue",
	               "magenta","cyan", "white",NULL };
chtype attribute_ch[] = {A_NORMAL,A_BOLD,A_UNDERLINE,A_REVERSE,A_STANDOUT,A_BLINK};
char *attribute_names[] = {"normal","bold","underline","reverse","standout","blink",NULL};

struct color * new_color_table ( char **color_vars ) {

	struct color *ret;
	int t=0,members=20;
	char **ccp;
	ret=xcalloc(members,sizeof(struct color));
	ccp=color_vars;

	while (*ccp!=NULL) {

		if (t>members-1) {
			members+=20;
			ret=xrealloc(ret,members*sizeof(struct color));
		}

		ret[t].color_pair=t+1;
		ret[t].fg=COLOR_WHITE;
		ret[t].bg=COLOR_BLACK;
		ret[t].attr=A_NORMAL;
		init_pair(t,COLOR_WHITE,COLOR_BLACK);
		ret[t].ch=COLOR_PAIR(ret[t].color_pair)|ret[t].attr;
		ret[t].name=xmalloc(strlen(*ccp)+1);
		strcpy(ret[t].name,*ccp);
		++t;
		++ccp;

	}

	ret=xrealloc(ret,(t+1)*sizeof(struct color));
	ret[t].name=NULL;
	return(ret);

}

void delete_color_table( struct color * color_table ) {
	struct color *color_p=color_table;

	while (color_p->name!=NULL) {

		xfree(color_p->name);
		color_p++;

	}

	xfree(color_table);
}

chtype name_to_chtype(char *color_name,char **color_names, chtype *chtype_array) {

	char **ccp;
	int t=0;
	chtype ret=(chtype) -1;
	ccp=color_names;

	if (color_name==NULL) return(ret);

	while (*ccp!=NULL) {

		if (!strcmp(color_name,*ccp)) {

      ret=chtype_array[t];
		  break;

		}

		++t;
		++ccp;

 	}

	return(ret);

}

char * chtype_to_name(chtype color,char **color_names, chtype *chtype_array) {

  char **ccp;
  int t=0;
  char *ret=NULL;
  ccp=color_names;

  while (*ccp!=NULL) {

	  if (color==chtype_array[t]) {

      ret=xmalloc(strlen(color_names[t])+1);
		  strcpy(ret,color_names[t]);
      break;
    }

    ++t;
    ++ccp;

  }

  return(ret);

}

int chtype_to_number(chtype color,char **color_names, chtype *chtype_array) {

  char **ccp;
  int t=0;
  int ret=-1;
  ccp=color_names;

  while (*ccp!=NULL) {

    if (color==chtype_array[t]) {
      ret=t;
      break;
    }

    ++t;
    ++ccp;

  }

  return(ret);

}

int color_to_number(chtype color) {

	int ret=chtype_to_number(color,color_names,color_ch);
	return(ret);

}

chtype number_to_color(int n) {

	return(color_ch[n]);

}

chtype number_to_attr(int n) {

	return(attribute_ch[n]);

}


int attr_to_number(chtype color) {

	int ret=chtype_to_number(color,attribute_names,attribute_ch);
	return(ret);

}

void read_config_colors (FILE *fp, struct color *color_table, int mode ) {

	chtype fg,bg,attr;
	char txt_color[]="color";
	char txt_mono[]="mono";
	char **ccp,*color_type=txt_color;
	struct color *color_p;
	color_p=color_table;

	if (mode) {
		color_type=txt_mono;
	}

	while ((ccp=get_next_config_var_array(fp,color_type))!=NULL) {

	  if (ccp[0]==NULL) {
      free_string_array(ccp);
	    continue;
	  }

	  color_p=color_table;

	  while (color_p->name!=NULL) {

	    if (!strcmp(ccp[0],color_p->name)) {

	      if (!mode) {

          fg=name_to_chtype(ccp[1],color_names,color_ch);
	        bg=name_to_chtype(ccp[2],color_names,color_ch);
	        attr=name_to_chtype(ccp[3],attribute_names,attribute_ch);

	        if(fg!=-1) color_p->fg=fg;
	        if(bg!=-1) color_p->bg=bg;
	        if(attr!=-1) color_p->attr=attr;

	      }
	      else {

	        fg=name_to_chtype(ccp[1],attribute_names,attribute_ch);
	        bg=name_to_chtype(ccp[2],attribute_names,attribute_ch);

	        if(fg!=-1) color_p->fg=fg;
	          else color_p->fg=A_NORMAL;

	        if(bg!=-1) color_p->bg=bg;
	          else color_p->bg=A_NORMAL;

	        color_p->attr=color_p->fg|color_p->bg;
	      }

	      init_pair(color_p->color_pair,color_p->fg,color_p->bg);

	      if(!mode)
	        color_p->ch=COLOR_PAIR(color_p->color_pair)|color_p->attr;
	      else
	        color_p->ch=COLOR_PAIR(0)|color_p->attr;
	      break;

	    }

	    color_p++;
	  }

		free_string_array(ccp);

	}

}

chtype get_color(char *name,struct color *color_table) {

  struct color *color_p=color_table;
	chtype ret=COLOR_PAIR(0);

  while (color_p->name!=NULL) {

	  if (!strcmp(color_p->name,name)) {
	    ret=color_p->ch;
	    break;
	  }

    color_p++;

  }

	return(ret);

}

int set_color(char *name,struct color *color_table, chtype fg,chtype bg,chtype attr,int color_mode) {

  struct color *color_p=color_table;
  int ret=-1;

  while (color_p->name!=NULL) {

    if (!strcmp(color_p->name,name)) {

	    ret=0;
	    color_p->fg=fg;
	    color_p->bg=bg;
	    color_p->attr=attr;

	    if(!color_mode) {
	      init_pair(color_p->color_pair,color_p->fg,color_p->bg);
	      color_p->ch=COLOR_PAIR(color_p->color_pair)|color_p->attr;
	    }
	    else {
	      color_p->attr=color_p->fg|color_p->bg;
	      color_p->ch=COLOR_PAIR(0)|color_p->attr;
	    }

      break;

    }

    color_p++;
  }

  return(ret);

}
void save_colors(FILE *fp,struct color *colors,int color_mode) {

	char txt_color[]="color";
	char txt_mono[]="mono";
	char txt_null[]="\0";
	char *color_type=txt_color;
	struct color *color_p;
	char *line;
	char **cccp;
	char *fg_name=NULL;
	char *bg_name=NULL;
	char *attr_name=NULL;
	color_p=colors;
	cccp=color_names;

	while (color_p->name!=NULL) {

	  if (!color_mode) {

      fg_name=chtype_to_name(color_p->fg,color_names,color_ch);
		  if(fg_name==NULL) fg_name=txt_null;

		  bg_name=chtype_to_name(color_p->bg,color_names,color_ch);
		  if(bg_name==NULL) bg_name=txt_null;

		  attr_name=chtype_to_name(color_p->attr,attribute_names,attribute_ch);
		  if(attr_name==NULL) attr_name=txt_null;

	  }
	  else {

		  color_type=txt_mono;

		  fg_name=chtype_to_name(color_p->fg,attribute_names,attribute_ch);
		  if (fg_name==NULL) fg_name=txt_null;

		  bg_name=chtype_to_name(color_p->bg,attribute_names,attribute_ch);
		  if(bg_name==NULL) bg_name=txt_null;

		  attr_name=txt_null;

	  }
		 
	  line=xmalloc(strlen(color_type)+strlen(" ")+strlen(color_p->name)+strlen("\t")+strlen(fg_name)+strlen("\t")+strlen(bg_name)+strlen("\t")+strlen(attr_name)+1);
	  strcpy(line,color_type);
	  strcat(line," ");
	  strcat(line,color_p->name);
	  strcat(line,"\t");
	  strcat(line,fg_name);
	  strcat(line,"\t");
	  strcat(line,bg_name);
	  strcat(line,"\t");
	  strcat(line,attr_name);

	  fputs(line,fp);
	  fputs("\n",fp);

	  xfree(line);

	  if(fg_name!=txt_null) xfree(fg_name);
	  if(bg_name!=txt_null) xfree(bg_name);
	  if(attr_name!=txt_null) xfree(attr_name);

	  ++color_p;
	}
}

void cp_color(struct color *color1,struct color *color2) {

	color1->fg=color2->fg;
	color1->bg=color2->bg;
	color1->attr=color2->attr;
	color1->ch=color2->ch;
	color1->color_pair=color2->color_pair;
	color1->name=color2->name;

}

int color_array_members (struct color *color_array) {

	struct color *color_p;
	int ret=1;

	if (color_array==NULL) return(-1);

	color_p=color_array;

	while(color_p->name!=NULL)  {

		++ret;
		++color_p;

	}

	return(ret);

}

void cp_color_array(struct color *color1_array,struct color *color2_array) {

	unsigned u;

	for (u=0;color2_array[u].name!=NULL;u++) {

		cp_color(&color1_array[u],&color2_array[u]);

	}

	cp_color(&color1_array[u],&color2_array[u]); 
}

void reinit_color_table (struct color *colors,int mode) {

	struct color *color_p=colors;

	if (color_p==NULL) return;

	while (color_p->name!=NULL) {
		set_color(color_p->name,colors,color_p->fg,color_p->bg,color_p->attr,mode);
		color_p++;
	}

}
