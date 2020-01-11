/*
 *  view.c
 *
 *  Copyright (C) 1997, 1998, 2000, 2003, 2004, 2006, 2007, 2019, 2020 Staf Wagemakers Belgium
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

#include "view.h"
static VIEW_PAR *p;
VIEW_PAR *view_par (VIEW_PAR *par)
{
if (par!=NULL) p=par;
  return (p);
}
unsigned view_strlen(char *c)
{
  unsigned u=0;

  while (*c) {

      u=u+1;
      c=c+view_charstr_len(c);

  }

  return u;
}
unsigned long view_gety()
{
if (p->mode) return(p->y+p->sy-p->lines+1);
   else return(p->y-p->lines+1);
}
unsigned long view_getx()
{
if (p->mode) return(p->x+p->sx+1);
   else return(p->x+1);
}
char *view_getstr(unsigned long y)
{
char *rc,*c,*s;
s=p->file[y];
c=rc=(char *) xmalloc(strlen(s)+1);
while (*s) 
  { if (*s!=8) { *c=*s;++c;}
      else if(c>rc) --c;
    ++s;
  }
*c=0;
rc=(char *) xrealloc(rc,strlen(rc)+1);
return rc;
}
/* -------------------------------------------- */
/* Wis een het bestand uit het geheugen ...     */
/* -------------------------------------------- */
void view_free_file()
{
unsigned long l;
for (l=0;l<=p->y_max;l++)  free(p->file[l]);
xfree(p->file);
}
void view_free()
{
  view_free_file();
  free_string_array(*p->view_exec);
}
int get_strlen (char *s)
{
int xmax=0;
while (*s) {
  if (*s!=8) ++xmax;
    else --xmax;
  s++;
  }
return xmax;
}
/* ---------------------------------------------------- */ 
/* Inladen van te bekijken bestand ...            */
/*                    */
/* aanroep  :           */
/*              */
/* char filename[] = te lezen bestand           */
/* char **file      = ptr naar geh. pl.           */
/*              */
/* antwoord :           */
/* 0 = ok                       */
/* 1 = nok                                        */
/*              */
/* ---------------------------------------------------- */ 
int view_load ()
{
FILE *fp=NULL;
char *cp,***ccc;
char *s;
int i;
size_t ii;
/* ----------------- vari voor het starten van gzip ---------------------- */
int  is_gz=0;
char *gzip=NULL;
FILE *popen(),*gz_fp; 
/* ----------------------------------------------------------------------- */
ccc=p->view_exec;
if(p->filename!=NULL) {
   while(*ccc) {
     if(strlen(p->filename)>strlen(ccc[0][0])) { 
       s=p->filename+strlen(p->filename)-strlen(ccc[0][0]);
     }
     else {
       s=p->filename;
     }
     if(!strcmp(s,ccc[0][0])){  
         gzip=(char *)xmalloc(strlen(ccc[0][1])+strlen(p->filename)+2);
         strcpy(gzip,ccc[0][1]);
         strcat(gzip," ");
         strcat(gzip,p->filename);
         break;
     }
     ++ccc;
   }
}

if(gzip) {
  int sys_err;
  
  if (p->cmd==1) {
    if (win_freopen("/dev/null","w",stderr,p->m_ok,p->win)) return(1);
  }
  if(freopen("/dev/null","w",stdout) ==NULL) {
    if(win_freopen("/dev/tty","w",stdout,p->m_ok,p->win)) return(1);
    char *txt[]={p->txt_f_freopen,NULL};
    open_okwin(6,40,p->m_ok,txt,p->win);
    return(1);
  }
  sys_err=system(gzip);
  if (sys_err==0) {
     gz_fp=popen(gzip,"r");
     if (gz_fp==NULL) pclose(fp);
        else  is_gz=1;
     } 
      
  if(win_freopen("/dev/tty","w",stdout,p->m_ok,p->win)) return(1);
  if (p->cmd==1) {
    if(win_freopen("/dev/tty","w",stderr,p->m_ok,p->win)) return(1);
    return(1);
  }
  xfree(gzip);
}
if (p->cmd<2) {
  if ((fp=fopen(p->filename,"r"))==NULL) {
    if (p->cmd==1) {
      fprintf(stderr,"%s %s %s",p->txt_f_open1,p->filename,p->txt_f_open2); 
      if (COLS) endwin();
      exit(1);
      }
    else {
      char *txt[]={p->txt_f_load,NULL};
      open_okwin(6,40,p->m_ok,txt,p->win);
      return(1);
      };
   };
};
if (p->load) view_free_file();
p->y=p->sy=0;p->sx=p->x=0;p->x_max=0;p->y_max=0;
if (p->cmd<2) {
  if (fseek(fp,0,SEEK_END)) {
  char *txt[]={p->txt_f_fseek,NULL};
  open_okwin(6,40,p->m_ok,txt,p->win);
  return(1);
  };
};
if (p->cmd<2) {
   p->size=ftell(fp);
   fseek(fp,0,SEEK_SET); }
  else p->size=0;
if (is_gz) {fclose(fp);fp=gz_fp;};
p->file=NULL;
ii=0;
do {
     int z;
     p->file=(char **) xrealloc(p->file,(p->y_max+2)*sizeof(char **));
     *(p->file+p->y_max)=NULL;
     if (p->cmd==2) p->x=getdelim(p->file+p->y_max,&ii,'\n',stdin);
       else p->x=getdelim(p->file+p->y_max,&ii,'\n',fp);
     if (p->x>0) {

        if (p->cmd==2) p->size+=p->x;

  if (p->x>1) {

  if (*(p->file[p->y_max]+p->x-2)==13) { 
    *(p->file[p->y_max]+p->x-2)=10;
    *(p->file[p->y_max]+p->x-1)=0;
    };
  }


  for (i=0;i<strlen(p->file[p->y_max]);i++) {
     if (*(p->file[p->y_max]+i)==TAB) {
       s=(char *) xmalloc(strlen(p->file[p->y_max])+(((i/8)+1)*8-i)+1);
       strcpy(s,p->file[p->y_max]);
       for (z=0;z<((i/8)+1)*8-i;z++) strcpy((s+i+z)," ");
       strcpy((s+i+z),(p->file[p->y_max]+i+1));
       cp=*(p->file+p->y_max);
       cp=(char *)xrealloc(cp,strlen(s)+1);
       *(p->file+p->y_max)=cp;
       strcpy(p->file[p->y_max],s);
       free(s);
      } 
   } 
       if ((get_strlen(p->file[p->y_max])>p->x_max)&&(p->x!=-1)) p->x_max=get_strlen(p->file[p->y_max]); 
       ++p->y_max;
       }
   } while (p->x!=-1);
if ((p->cmd<2)&&(!is_gz)) fclose(fp);
if (is_gz) pclose(fp);
p->load=1;
if (p->x_max>p->cols) p->x_max-=p->cols; 
  else p->x_max=0;
p->x=p->y=0;
return(0);
}
/* ----------------------------------------- */
/* Print een kar. op het scherm, filter voor */
/* '/0' en 0x0ad.          */
/* ----------------------------------------- */
void view_addch(char *c)
{
/* VIEW_PAR *p=view_par(NULL); */
if (*c==(char)0xad) waddch_fix(p->win,'-');
   else if (*(c)) waddch_fix(p->win,*c);
}
/* ----------------------------------------- */
/* Print een regel uit **bestand af ...      */
/*               */
/* int yp          = y-postie                */
/* unsigned long r = regel         */
/* ----------------------------------------- */
void view_addline (int yp,unsigned long r)
{
char *s;
unsigned t;
int lx=0;
if (r>=p->y_max) return;
if (strlen(p->file[r])>p->x) {
if (p->y<=p->y_max) {
  s=p->file[r];
  lx=strlen(s);
  if (p->x) {
  unsigned tt=0;
  for (t=0;t<=p->x-1;t++) { 
     if (*(s+1)==8) {s+=3;tt+=3;}
       else {++s;++tt;}
     if (tt>=lx) return;
     }
  } 
  wmove(p->win,yp,0);
  view_addstr(s);
}
}
}

unsigned view_charstr_len(char *c) {

  int   number_of_chars=0;

  switch (*c) {
    case 0x00 ... 0x7F:
      number_of_chars=1; if (*c != 0x8);
      break;
    case 0xffffffC0 ... 0xffffffDF:
      number_of_chars=2;
      break;
    case 0xffffffE2 ... 0xffffffEF:
      number_of_chars=3;
      break;
    case 0xffffffF0 ... 0xffffffF7:
      number_of_chars=4;
      break;
    default:
      number_of_chars=1;
      break;
  }

  return(number_of_chars);

}

char * view_charstr(char *c) {

  char *str;
  int   number_of_chars=view_charstr_len(c);

  str=xcalloc(number_of_chars + 1, sizeof(char));
  strncpy(str,c,number_of_chars);

  return(str);

}
void view_addstr(char *s)
{ 
  int lx=0;
  unsigned t;
  char *str;

  for (t=0;t<=strlen(s);t++) {

    if (((lx>p->cols-1)&&*(s+t)!=8)||(*(s+t)==0)) break; 
    if (*(s+t)==8) {
      lx--;
      if (*(s+t-1)==*(s+t+1)) {
        waddch_fix(p->win,*(s+t));
        wbkgdset(p->win,p->color[2]);
        view_addch(s+(++t));
        wbkgdset(p->win,p->color[1]);
      }
      else {
        if (*(s+t-1)=='_') {
          waddch_fix(p->win,*(s+t));
          wbkgdset(p->win,p->color[3]);
          view_addch(s+(++t));
          wbkgdset(p->win,p->color[1]);
        }
        else waddch_fix(p->win,'.');
      }
    }
    else {
        str=view_charstr(s+t);
        waddstr(p->win,str);
        t=t+strlen(str)-1;
        xfree(str);
/*
      if (*(s+t) == 0xffffffe2 ) {
        char str[5];
        strncpy(str,(s+t),4);
        str[4]=0;
        waddstr(p->win,str);
        t=t+4;
      } else {
        view_addch(s+t);
      }
*/
    }
    lx++;
  }
}

void view_sety(unsigned long y)
{
if (!p->mode) {
if (y > p->y_max) y=p->y_max;
   p->y=y;
   return;
   }
if (y > p->y_max-1) y=p->y_max-1;
if (y > (p->y_max-p->lines-1)) {
   p->y=p->y_max-p->lines;
   p->sy=y-p->y;
   }
   else  {  
   p->y=(y/p->lines)*p->lines;
   p->sy=y%p->lines;
   }
}
void view_setx(long x)
{
p->x=x/p->cols;
p->sx=x%p->cols;
p->x*=p->cols;
}
void view_move (unsigned long y, long x)
{
view_sety(y);
view_setx(x);
view_refresh();
}
void view_set_cursor(void)
{
wmove(p->win,p->sy,p->sx);
}
/* ----------------------------------------- */
/* Print een volledig scherm af ...          */
/* ----------------------------------------- */
void view_refresh()
{
int i;
if (!p->mode) {if (p->y>p->y_max-1) p->y=p->y_max-1;}
   else { if (p->y_max>p->lines)
      if (p->y+p->sy+p->lines>p->y_max-1) p->y=p->y_max-p->lines;
        }
  
werase(p->win);

for (i=0;i<p->lines;i++) view_addline(i,p->y++);
wrefresh(p->win);
if (p->mode) { 
   leaveok(p->win,FALSE);
   curs_set(1);
   view_set_cursor();
   }
}
/* ----------------------------------------------------- */
/* Scherm 1 regel naarboven + volgende regel printen ... */
/* ----------------------------------------------------- */
void view_down()
{
if (!p->mode) {
  if (p->y<(p->y_max+p->lines-1)) {
     scrollok(p->win,TRUE);
     wscrl(p->win,1);
     scrollok(p->win,FALSE);
     view_addline(p->lines-1,p->y++);
     wrefresh(p->win);
     }
  }
  else {
      if (p->y+p->sy-p->lines<p->y_max-1) {
         if (++p->sy>=p->lines) {
      p->sy=p->lines-1;
            scrollok(p->win,TRUE);
            wscrl(p->win,1);
            scrollok(p->win,FALSE);
            view_addline(p->lines-1,p->y++);
      wrefresh(p->win);
            }
         }
      view_set_cursor();
  }
}
/* ----------------------------------------------------- */
/* Scherm 1 regel naarbeneden + vorige regel printen ... */
/* ----------------------------------------------------- */
void view_up()
{
if (!p->mode) {
   if (p->y>p->lines) {
      scrollok(p->win,TRUE);
      wscrl(p->win,-1);
      scrollok(p->win,FALSE);
      p->y-=p->lines;
      view_addline(0,--p->y);
      p->y+=p->lines;
      wrefresh(p->win);
      }
   }
   else {
      if (--p->sy<0) {
         p->sy=0;
   if (p->y>p->lines) {
            scrollok(p->win,TRUE);
            wscrl(p->win,-1);
            scrollok(p->win,FALSE);
            p->y-=p->lines;
            view_addline(0,--p->y);
            p->y+=p->lines;
            wrefresh(p->win);
            }
       }
      view_set_cursor();
      }
}   
/* ---------------------------------------------------- */ 
/* Vorige pagina printen ...        */
/* ---------------------------------------------------- */ 
void view_previous()
{
if (p->y>=2*(p->lines)) p->y-=2*(p->lines);
   else p->y=0; 
view_refresh(); 
}
/* ---------------------------------------------------- */ 
/* Scherm naar rechts ...       */
/* ---------------------------------------------------- */ 
void view_right()
{
if (!p->mode) {
   if (p->x<p->x_max) {
      ++p->x;
      p->y-=p->lines;
      view_refresh();
      }
   }
   else {
        if (p->x+p->sx<view_strlen(p->file[p->sy+p->y-p->lines])-1) {
        if (++p->sx>=p->cols) {
        p->sx=p->cols-7;
              p->y-=p->lines;
              p->x+=7;
              view_refresh();
        }
        else  view_set_cursor();
  }
   }
}
/* ---------------------------------------------------- */ 
/* Scherm naar links ...        */
/* ---------------------------------------------------- */ 
void view_left()
{
if (!p->mode) {
   if (p->x>0) {
      --p->x;
      p->y-=p->lines;
      view_refresh();
      }
    }
  else {
      if(--p->sx<0) {
          if ((p->x+p->sx)>=0) {
       p->sx=6;
             p->y-=p->lines;
             p->x-=7;
       if (p->x<0) {
          p->sx+=p->x;
    p->x=0;
    }
             view_refresh();
    }
    else p->sx=0;
      }
      else view_set_cursor();
      } 
}


