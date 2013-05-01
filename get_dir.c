/*
 *  get_dir.c
 *
 *  Copyright (C) 1997, 1998, 2006, 2007  Staf Wagemakers Belgie/Belgium
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

#include "get_dir.h"
/* int compare(const char **c1,const char **c2)  */
int compare(const void *e1, const void *e2) 
{
  char **c1=(char **) e1, **c2=(char **) e2;
  return strcmp (*c1,*c2);
}
struct dir_data* get_dir(char *path)
{
 struct dirent *ep;
 struct stat stat_buf;
 struct dir_data *data;
 DIR    *dp;
 char   *s;
 dp=opendir(path);
 if (dp==NULL) return(NULL);
  data=(struct dir_data*) xmalloc(sizeof(struct dir_data));
  data->file=data->dir=NULL;
  data->files=data->dirs=0;
  while ((ep=readdir(dp))) {
       s=(char *) xmalloc(strlen(path)+strlen(ep->d_name)+1);
       strcpy(s,path);
       strcat(s,ep->d_name);
       if (stat(s,&stat_buf)==-1) {
             data->file=(char **) xrealloc(data->file,(data->files+1)*sizeof(char **)); 
             data->file[data->files]=(char *) xmalloc(strlen(ep->d_name)+1);
	     strcpy (data->file[data->files++],ep->d_name);
         }
	else { 
            if (S_ISDIR(stat_buf.st_mode)) { 
              data->dir=(char **) xrealloc(data->dir,(data->dirs+1)*sizeof(char **));
              data->dir[data->dirs]=(char *) xmalloc(strlen(ep->d_name)+1); 
	      strcpy (data->dir[data->dirs++],ep->d_name);
	      }
	      else {
                  data->file=(char **) xrealloc(data->file,(data->files+1)*sizeof(char *)); 
	          data->file[data->files]=(char *) xmalloc(strlen(ep->d_name)+1);
	          strcpy (data->file[data->files++],ep->d_name);
              }
	 }     
       xfree(s);
       };
   (void) closedir (dp);
   qsort (data->dir,data->dirs,sizeof(char **),compare);
   qsort (data->file,data->files,sizeof(char **),compare);
   return(data);
}
void get_dir_free(struct dir_data *data)
{
unsigned long l;
for (l=0;l<data->files;l++) xfree(data->file[l]);
for (l=0;l<data->dirs;l++) xfree(data->dir[l]);
xfree(data->file);
xfree(data->dir);
xfree(data);
}

