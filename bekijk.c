/*
 * bekijk.c
 *
 * Copyright (C) 1997, 1998, 2000, 2001, 2002, 2003, 2006, 2007, 2015, 2020, 2024  Staf Wagemakers Belgium
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

#define TAB 9
#define TYPE_WIDTH 59
#include "common.h"
#include "color.h"
#include"ncurses_h.h"
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <libgen.h>

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

#include "input.h"
#include "win.h"
#include "view.h"
#include "configfile.h"
#include "lookat_configfile.h"
#include "outofmemory.h"
#include "msg.h"
#include "color.h"
#ifdef _NED
#include "ned.h"
#endif
#ifdef _ENG
#include "eng.h"
#endif
WINDOW *win1,*win2,*w,*hm_win,*setcolors_w;          /* Midden scherm, Onderscherm + alg. venster*/
MENU *hm_menup;
INPUT_STRING zoek_str;
char s[10]={'\0'};    /* String voor de te-zoeken-tekst     */
char *ss=NULL;
chtype ansi_colors[ANSI_SGR_PARAMS+1];    /* Array with the ansi colors */
int  color_mode;
struct color *colors;
struct color *color_array;
struct color *mono_array;
char txt_view[]="view";
char txt_view_found[]="view_found";
char txt_view_bold[]="view_bold";
char txt_view_italic[]="view_italic";
char txt_view_underline[]="view_underline";
char txt_view_blink[]="view_blink";
char txt_view_reverse[]="view_reverse";
char txt_main_menu[]="main_menu";
char txt_main_menuhot[]="main_menuhot";
char txt_main_menusel[]="main_menusel";
char txt_main_menuhotsel[]="main_menuhotsel";
char txt_status_bar[]="status_bar";
char txt_win1[]="win1";
char txt_win1_edit[]="win1_edit";
char txt_win1_menu[]="win1_menu";
char txt_win1_menusel[]="win1_menusel";
char txt_win1_menuhot[]="win1_menuhot";
char txt_win1_menuhotsel[]="win1_menuhotsel";
char txt_win2[]="win2";
char txt_win2_edit[]="win2_edit";
char txt_win2_menu[]="win2_menu";
char txt_win2_menusel[]="win2_menusel";
char txt_win2_menuhot[]="win2_menuhot";
char txt_win2_menuhotsel[]="win2_menuhotsel";

char *color_vars[] = { 
  txt_view,             /* 0 */
  txt_view_bold,        /* 1 */
  txt_view_italic,      /* 2 */
  txt_view_underline,   /* 3 */
  txt_view_blink,       /* 4 */
  txt_view_reverse,     /* 4 */
  txt_main_menu,        /* 5 */
  txt_main_menusel,     /* 6 */
  txt_main_menuhot,     /* 7 */
  txt_main_menuhotsel,  /* 8 */
  txt_status_bar,       /* 9 */
  txt_win1,             /* 10 */
  txt_win1_edit,        /* 11 */
  txt_win1_menu,        /* 12 */
  txt_win1_menusel,     /* 13 */
  txt_win1_menuhot,     /* 14 */
  txt_win1_menuhotsel,  /* 15 */
  txt_win2,             /* 16 */
  txt_win2_edit,        /* 17 */
  txt_win2_menu,        /* 18 */
  txt_win2_menusel,     /* 19 */
  txt_win2_menuhot,     /* 20 */
  txt_win2_menuhotsel,  /* 21 */
  txt_view_found,       /* 22 */
  NULL
};

char free_vb=0;
unsigned brol,zoek_offset=0;
unsigned long zoek_y;
int  zoek_mode;     /* Ondersheid tussen G & k ?????      */
MENU m_ok,m_jn;
VIEW_PAR bv;
int display_raw=0;

/* ------------------------------------------- 
 * Afsluiten van het prg
 * ------------------------------------------- */
static void terminate (int sig)
{
   endwin();

   if (freopen("/dev/tty","w",stderr)==NULL) {

    fprintf(stdout,"\n %s %d %s\n",txt_err_freopen,errno,strerror(errno));

   };

   fprintf(stderr,"\n %s %d\n",txt_term,sig);
   exit (0);
}

/* ------------------------------------------- */
/* Procedure voor het sluiten van menu venster */
/* ------------------------------------------- */ 
void m_close(MENU *m)
{
   delwin(m->w);
   touchwin(win1);
   wrefresh(win1);
}

void cm_close(MENU *m) 
{
    delwin(m->w);
    touchwin(win1);
    wrefresh(win1);
    touchwin((m->parent)->w);
    touchwin(setcolors_w);
    wrefresh(setcolors_w);
    wrefresh((m->parent)->w);
}

/* --------------------------------------------------- */
/* Procedure voor het openen van het bestandsmenu .... */
/* --------------------------------------------------- */
void open_bm (MENU *m)
{
   m->w=open_win(7,m->l+2,1,1); 
   m_open(m,win1);   
   mvwaddch(m->w,2,0,ACS_LTEE);
   whline(m->w,ACS_HLINE,m->l);
   mvwaddch(m->w,2,m->l+1,ACS_RTEE);
   mvwaddch(m->w,4,0,ACS_LTEE);
   whline(m->w,ACS_HLINE,m->l);
   mvwaddch(m->w,4,m->l+1,ACS_RTEE);
}

/* ----------------------------------------- */
/* Procedure voor het openen van het ga-menu */
/* ----------------------------------------- */
void open_gm (MENU *m)
{
   m->w=open_win(7,m->l+2,1,(m->parent)->place[3]);
   m_open(m,win1);   
}

/* ------------------------------------------- */
/* Procedure voor het openen van het zoek-menu */
/* ------------------------------------------- */
void open_zm (MENU *m)
{
   m->w=open_win(5,m->l+2,1,(m->parent)->place[5]);
   m_open(m,win1);   
}

/* ------------------------------------------- */
/* Procedure voor het openen van het optie-menu*/
/* ------------------------------------------- */
void open_om (MENU *m)
{
   m->w=open_win(9,m->l+2,1,(m->parent)->place[7]);
   m_open(m,win1);   
   mvwaddch(m->w,3,0,ACS_LTEE);
   whline(m->w,ACS_HLINE,m->l);
   mvwaddch(m->w,3,m->l+1,ACS_RTEE);
   mvwaddch(m->w,6,0,ACS_LTEE);
   whline(m->w,ACS_HLINE,m->l);
   mvwaddch(m->w,6,m->l+1,ACS_RTEE);
}

/* ------------------------------------------- */
/* Procedure voor het openen van het help-menu */
/* ------------------------------------------- */
void open_hlpm (MENU *m)
{
   m->w=open_win(5,m->l+2,1,COLS-m->l-2);
   m_open(m,win1);
}

/* ----------------------------------------- */
/* Procedure voor het openen van het fg-menu */
/* ----------------------------------------- */
void open_fg (MENU *m)
{
   /* m->w=open_win(m->amount+2,m->l+2,2*(m->parent)->sel+9,37); */
   m->w=open_win(m->amount+2,m->l+2,2*(m->parent)->sel+LINES/2-3,COLS/2-3);
   m_open(m,setcolors_w);
}

/* ----------------------------------------- */
/* give_notice function          */
/* ----------------------------------------- */
int exit_lookat(WINDOW *rw) {
   if (txt_om2[1]==' ') return 1;
   return(open_ynwin(6,43,&m_jn,txt_stop,rw));
}


/*
 * displays an error msg for view_load
 */
void view_load_failed() {
    refresh();
    open_okwin(9,43,&m_ok,txt_view_load_error,win1);
}


/*
 * view_load wrapper with error handling
 * An error window will be displayed if view_load fails
 */
int bekijk_view_load() {
  int ret=0;
  if(view_load() ) {
    ret=open_ynwin(9,60,&m_jn,txt_view_load_abort,win1);
  }

  view_set_raw(display_raw);
  return ret;
}

/* 
 * freopen wrapper with error handling.
 * An error window will be displayed if freopen fails
 */
int bekijk_freopen(const char *pathname, const char *mode, FILE *stream, WINDOW *win1)
{
 int ret=0;

 if ( win_freopen(pathname,mode,stream,&m_ok,win1) ) {
    ret=open_ynwin(9,60,&m_jn,txt_freopen_abort,win1);
 }

 return ret;
}

/* ----------------------------------------- */
/* Open van best. selektie menu              */
/*                                           */
/* int ends():  exit function                */
/*                                           */
/* Geeft terug: 0 -> ok 1 -> ESC 2-> error   */
/* ----------------------------------------- */
int open_best(int mode)
{
   char *vb;
   int (*ends)(WINDOW *)=NULL;
   chtype kl[4]={
     get_color(txt_win1,colors),
     get_color(txt_win1_edit,colors),
     get_color(txt_win1_menu,colors),
     get_color(txt_win1_menusel,colors)
   };
   vb=bv.filename;

   if(mode) ends=&exit_lookat;

   bv.filename=open_filewin(win2,txt_bo,kl,win1,ends);

   if (bv.filename) {

      if (view_load()) {

        xfree(bv.filename); 
        bv.filename=vb;
        return(2);

      }
      else {

        if (free_vb) xfree(vb);

        bv.y=0;bv.x=0;
        view_refresh();

      };

      free_vb=1; 
   }
   else {

     bv.filename=vb;return(1);

   }
   return(0);
}

/*
 * displays an error msg for view_load
 */
void open_best_failed() {

    refresh();
    open_okwin(9,43,&m_ok,txt_open_best_error,win1);

}

/*
 * open_best wrapper with error handling
 * An error window will be displayed if open_best fails
 *
 * returns:
 *          0 -> ok ( or ignore error)
 *          1 -> ESC
 *          2 -> err ( and exit requested ) 
 */
int bekijk_open_best(int mode) {

  int ret=0;

  if(open_best(mode) > 1 ) {

    if (open_ynwin(9,60,&m_jn,txt_open_best_abort,win1)) ret=2;

  }

  return ret;
}

/* -------------------------------------------- */
/* Openen van het zoek venster                  */
/*                                              */
/* char     *s = pointer naar string            */
/* char *titel = pointer naar venster-titel     */
/* char *tekst = pointer naar info-tekst        */
/*                                              */
/* Geeft terug 0 -> geen onderscheid G & k      */
/*             1 -> ondersheid G & k            */
/*             2 -> break                       */
/*             bit 1 -> set onderscheid G&K     */
/*             bit 2 -> set backwards           */
/*             3     -> break                   */ 
/* -------------------------------------------- */
int open_zoek_venster (char *titel,char *tekst)
{
   MENU m;
   int brol;
   int ret=0;
   int toetsen[]={27,9,0};
   unsigned pl_ok[]={4,7,4,26,6,15,6,35};
   int key_m[]={0,9,'\n',' ','\n','\n'};
   w=open_cwin(8,60);
   m.w=w;
   m.amount=4;
   m.l=0;
   m.txt=txt_m_zoek;
   m.place=pl_ok;
   m.color1=get_color(txt_win2,colors);
   m.color2=get_color(txt_win2_menusel,colors);
   m.color3=get_color(txt_win2_menuhot,colors);
   m.color4=get_color(txt_win2_menuhotsel,colors);
   m.sel=m.used=0;
   m.key=key_m;
   m.hkey=hkey_zoek;
   m.hplace=hplace_zoek;
   zoek_str.w=w;
   zoek_str.yp=2;
   zoek_str.xp=22;
   zoek_str.key=toetsen;
   wbkgd(w,get_color(txt_win2,colors));
   werase(w);
   box(w,0,0);
   echo();
   leaveok(w,FALSE);
   curs_set(1);
   mvwaddstr(w,0,2,titel);
   mvwaddstr(w,2,2,tekst);
   mvwaddstr(w,4,6,txt_ondersheid_g);
   mvwaddstr(w,4,25,txt_search_back);
   wmove (w,2,22);
   wbkgdset(w,get_color(txt_win2_edit,colors));
   for (brol=0;brol<=32;brol++) waddch(w,' ');
   m.sel=0;
   do {
     menu_print(&m);
     wbkgdset(w,get_color(txt_win2_edit,colors));

     if (brol!=1) brol=input(&zoek_str);
       else brol=2;

     curs_set(0);
     leaveok(w,TRUE);

     if (!brol) {
        if (m.txt[0]==txt_X) ret=ret | 1;
        if (m.txt[1]==txt_X) ret=ret | 2;
        return(ret);
     };

     if (brol!=2) return(3);

     do { 
       brol=menu_key(&m);
       if ((brol==2)&&(m.sel==0)) break;
     } while(brol<3);

     if ((m.sel<2)&&(brol>2)) {
        if (m.txt[m.sel]==txt_spacie) m.txt[m.sel]=txt_X;
           else m.txt[m.sel]=txt_spacie;
        brol=1;
        };

   } while(brol<3);

   if (m.sel==2) {

     if (m.txt[0]==txt_X) ret=ret | 1;
     if (m.txt[1]==txt_X) ret=ret | 2;
     return(ret);

   };
   return (3);
}

/* -------------------------------------------- */
/* Konverteert een string naar grootte letters  */
/* -------------------------------------------- */
void str_toupper (char *c) {

   int i=0;

   if ((c!=NULL) && (*c!=0)) {

      do {c[i]=toupper(c[i]);++i;} while (c[i]!=0);

   }

}

/* -------------------------------------------- */
/* Print de gevonden tekst in de juiste kleur */
/* -------------------------------------------- */
void print_zoek (char *c,char *cp,char *sp,char *rp,unsigned i)
{
  int brol;
  brol=cp-c;
  cp=rp+brol;
  strncpy(sp,cp,strlen(zoek_str.c));
  zoek_y=i;
  zoek_offset=cp-rp;
  view_setx(zoek_offset);

  if ((bv.sx+strlen(sp))>bv.cols) { 

     bv.x+=bv.sx+strlen(sp)-bv.cols;
     bv.sx-=bv.sx+strlen(sp)-bv.cols;

  }

  if (((bv.x+bv.cols)>strlen(c))&&(strlen(c)>bv.cols)) {

     bv.x=strlen(c)-bv.cols;
     bv.sx=zoek_offset-bv.x;
  } 

  view_sety(zoek_y);
  view_refresh();
  zoek_offset++;
  wmove (bv.win,bv.sy,bv.sx);
  wbkgdset(bv.win,get_color(txt_view_found,colors));
  view_addstr(sp);
  wbkgdset(bv.win,get_color(txt_view,colors));
  wmove (bv.win,bv.sy,bv.sx);
  wrefresh(bv.win);
  xfree(rp);
}

/* -------------------------------------------- */
/* Funktie voor het zoeken van tekst ...  */  
/*            */
/* unsigned long start = bestand te-zoeken-van  */
/* int mode bit 1 = 0 -> geen ondersheid G & k  */
/*                = 1 -> ondersheid G & k */
/*          bit 2 = backwards                   */
/* -------------------------------------------- */
void zoek(unsigned long start,int mode) {

   char *cp,*rp;
   char *c;
   char *sp;
   int add,e=0;
   int i;
   unsigned l;
   int end;

   do {

      l=0;
      sp=NULL;
      c=NULL;

      if (zoek_y!=start) zoek_offset=0;

      if (view_par(NULL)->mode) {

          getyx(bv.win,bv.sy,bv.sx);
          zoek_offset=view_getx();
      };

      if (txt_m_zoek[1]==txt_spacie) { 

          end=bv.y_max;
          add=1;

          if ((start==0)&&(view_getx()==1)) e=1; 

       }
       else {

         if ((start==0)&&(view_getx()==1)) e=0; 

         end=-1;
         add=-1;

       }

       for (i=start;i!=end;i+=add) {

          if (i!=start) zoek_offset=0;
  
          if (add==-1) if(i<0) { 

            cp=NULL;
            i=0;
            break;

          }
   
          rp=view_getstr(i);
          c=xrealloc(c,strlen(rp)+1);
          sp=xrealloc(sp,strlen(zoek_str.c)+1);
          strcpy(c,rp);
          strcpy(sp,zoek_str.c);
  
          if (txt_m_zoek[0]==txt_spacie) {

            str_toupper(sp);
            str_toupper(c);

          };

          wrefresh(win2);
  
          if (add==-1) {

            char *cz,*cp2=NULL;
            cp=NULL;

            if(zoek_offset) c[zoek_offset-1]='\0';

            cz=c;

            for (;;) {

              cp2=strstr(cz,sp);

              if (cp2!=NULL) { 

                cz+=strlen(sp);
                cp=cp2;

              } else break;

             }; 

            } else {

              cp=strstr(c+zoek_offset+l,sp);

            }

  
          if (cp!=NULL) break;

            zoek_offset=0;
            xfree(rp);

        } /* for */

        if (c!=NULL) {   

          if (cp!=NULL) {

            print_zoek(c,cp,sp,rp,i);
            e=0; 

          } else {

              if (!e) {

                char **verder;

                if (add==-1) verder=txt_verder_zoeken_begin;
                  else verder=txt_verder_zoeken_einde;

                e=open_ynwin(6,55,&m_jn,verder,win1);

               if (e) {

                  if (add==-1) {

                      bv.x=bv.sx=0;
                      start=bv.y_max-1;
                      wmove(bv.win,bv.sy,bv.sx);

                    } else {

                      bv.x=bv.sx=start=0;wmove(bv.win,bv.sy,bv.sx);

                     }

                }
              }
              else { 

                 e=0;
                 open_okwin(6,40,&m_ok,txt_t_nt_gevonden,win1);
              }  
          }

       } else {

            e=0;
            mvwprintw(win2,0,0,"   Internal Error!!!!!! Press a key to continue... ");
            wrefresh(win2);
            getchar();

       }

       xfree(c);
       xfree(sp);
       touchwin(win1);
       wrefresh(win1);

   } while (e);
}

/* -------------------------------------------- */
/* Funktie voor het inlezen van te-zoeken-tekst */
/* char  mode :  0 -> huidige positie   */
/*               1 -> van begin     */
/* -------------------------------------------- */
void zoek_tekst (char mode)
{
   zoek_mode=open_zoek_venster(txt_z_v_tekst,txt_geef_de_tekst);
   delwin(w);
   touchwin(win1);
   wrefresh(win1);

   if (zoek_mode<4) {
      if (mode) zoek(0,zoek_mode);
        else zoek(view_gety()-1,zoek_mode);
   }
   else zoek_mode=1;
   ss=(char *) xrealloc(ss,strlen(zoek_str.c)+1);
   strcpy(ss,zoek_str.c);
}

/* -------------------------------------------- */
/* Funktie om naar de lijnnr. te gaan ...       */
/* -------------------------------------------- */
void ga_lijn ()
{
   MENU m;
   int brol;
   unsigned long y;
   char *txt[]={txt_ga_lijn,txt_geef_lijnnr};
   INPUT_STRING s;
   m.txt=txt_ok_anu;
   m.hkey=hkey_ok_anu;
   m.hplace=hplace_ok_anu;
   m.color1=get_color(txt_win2,colors);
   m.color2=get_color(txt_win2_menusel,colors);
   m.color3=get_color(txt_win2_menuhot,colors);
   m.color4=get_color(txt_win2_menuhotsel,colors);
   brol=open_inputwin(7,60,&m,get_color(txt_win2_edit,colors),&s,txt,1,win1);
   curs_set(0);
   if (strlen(s.c)&&!brol) {
      sscanf(s.c,"%lu",&y);
      if (y) --y;
        else bv.y=0;
      view_sety(y);
      view_refresh();
   }
   xfree(s.c);
}

/* -------------------------------------------- */
/* char     *s = pointer naar string    */
/* char *titel = pointer naar venster-titel */
/* char *tekst = pointer naar info-tekst  */
/* n         = plaats       */
/* mode        = 0 -> bijvoegen     */
/*     1 -> aanpassen     */
/* -------------------------------------------- */
void open_type_venster (char *titel,char *txt_typename,char *txt_typeval,int n,int mode)
{
   WINDOW *w;
   MENU m;
   INPUT_STRING ip_name,ip_val;

   if(n<0) n=0;

   char ***ccc;
   int brol;
   int toetsen[]={27,9,0};
   unsigned pl_ok[]={7,11,7,31};
   int key_m[]={0,9,'\n',' ','\n',27};
   w=open_cwin(10,50);
   ccc=bv.view_exec;
   m.w=w;
   m.amount=2;
   m.l=0;
   m.txt=txt_m_type;
   m.place=pl_ok;
   m.color1=get_color(txt_win2_menu,colors);
   m.color2=get_color(txt_win2_menusel,colors);
   m.color3=get_color(txt_win2_menuhot,colors);
   m.color4=get_color(txt_win2_menuhotsel,colors);
   m.sel=m.used=0;
   m.key=key_m;
   m.hkey=hkey_type;
   m.hplace=hplace_type;
   ip_val.w=ip_name.w=w;
   ip_name.yp=2;ip_val.yp=4;
   ip_val.xp=ip_name.xp=17;

   if (mode!=0) { 
     ip_name.c=(char *) xmalloc(strlen(ccc[n][0]));
     strcpy(ip_name.c,ccc[n][0]+1);
     ip_val.c=(char *) xmalloc(strlen(ccc[n][1])+1);
     strcpy(ip_val.c,ccc[n][1]);
   }
   else ip_name.c=ip_val.c=NULL;

   ip_val.n=ip_name.n=29;
   ip_val.m=ip_name.m=0;
   ip_val.count=ip_name.count=0;
   ip_val.mode=ip_name.mode=0;
   ip_val.insert=ip_name.insert=1;
   ip_val.ox=ip_name.ox=0;
   ip_val.key=ip_name.key=toetsen;

   wbkgd(w,get_color(txt_win2,colors));
   werase(w);
   box(w,0,0);
   echo();
   leaveok(w,FALSE);
   curs_set(1);
   mvwaddstr(w,0,2,titel);
   mvwaddstr(w,2,2,txt_typename);
   mvwaddstr(w,4,2,txt_typeval);
   wmove (w,2,16);
   waddch(w,'.');
   wbkgdset(w,get_color(txt_win2_edit,colors));

   for (brol=0;brol<=ip_name.n;brol++) waddch(w,' ');
   wmove (w,4,17);
   for (brol=0;brol<=ip_val.n;brol++) waddch(w,' ');
   input_show_string(&ip_val);
   touchwin(w);
   wrefresh(w);

   do {

     menu_print(&m);
     m.sel=0;
     wbkgdset(w,get_color(txt_win2_edit,colors));
     touchwin(w);
     wrefresh(w);

     if (brol!=1) brol=input(&ip_name);
       else brol=2;

     leaveok(w,TRUE);
     curs_set(0);

     if (brol==1) break;

     brol=input(&ip_val);
     leaveok(w,TRUE);
     curs_set(0);

     if (brol==1) break;

     do { 

         brol=menu_key(&m);
         if ((brol==2)&&(m.sel==0)) break;

     } while(brol<3);

   } while(brol<3);


   if ((m.sel==0)&&(brol==3)) {

      int t=n;
      char *c;

      if (mode!=0) { 

         t=n;
         ccc[t][0]=xrealloc(ccc[t][0],strlen(ip_name.c)+2);
         strcpy(ccc[t][0],".");
         strcat(ccc[t][0],ip_name.c);
         ccc[t][1]=xrealloc(ccc[t][1],strlen(ip_val.c)+1);
         strcpy(ccc[t][1],ip_val.c);

      }
      else { 

       c=xmalloc(strlen(ip_name.c)+2);
       strcpy(c,".");
       strcat(c,ip_name.c);
       view_add_view_exec(t,c,ip_val.c);
       xfree(c);

      }
   }
   xfree(ip_name.c);
   xfree(ip_val.c); 
   delwin(w); 
}

char **colors_2_str (struct color *colors,int *a) {

  struct color *color_p=colors;
  char ** ret=NULL;
  int i=0;

  *a=20;
  ret=xcalloc(*a,sizeof(char *));

  while(color_p->name!=NULL) {

    if(i>*a-1) {
      *a=*a+20;
      ret=xrealloc(ret,(*a)*sizeof(char *));
    }

    ret[i]=xmalloc(strlen(color_p->name)+1);
    strcpy(ret[i],color_p->name);
    ++i;
    ++color_p;

  }

  *a=i;
  ret=xrealloc(ret,i*sizeof(char *));

  return(ret);
}

/*
 * Converts bv.view_exec to strings array
 * a let to set to the number of strings
 */
char **view_exec_2_strs(int *a)
{

  char **ret=view_view_exec_2_trimmed_string_array(TYPE_WIDTH);
  *a=number_of_strings(ret);

  return(ret);

}
/*
 * Manage the types...
 */
void set_types ()
{

MENU m,mm;
WINDOW *w;
int key_m[]={KEY_UP,KEY_DOWN,'\n',KEY_PPAGE,KEY_NPAGE,27,9};
int key_mm[]={0,9,'\n',' ',27,'\n'};
unsigned pl_m[]={2,5,3,5,4,5,5,5,6,5,7,5,8,5,9,5,10,5,11,5,12,5,13,5,14,5};
unsigned pl_mm[]={15,1,15,1,15,1,15,1,15,1,2,5};
int m_selected=0;
int s,i,numberOfTypes=0;
char **cc=NULL;
unsigned selectedType=0;

i=(TYPE_WIDTH+11-strlen(txt_typem[0])-strlen(txt_typem[1])-strlen(txt_typem[2])-strlen(txt_typem[3])-strlen(txt_typem[4]))/6;
pl_mm[1]=1+i;
pl_mm[3]=pl_mm[1]+i+strlen(txt_typem[0]);
pl_mm[5]=pl_mm[3]+i+strlen(txt_typem[1]);
pl_mm[7]=pl_mm[5]+i+strlen(txt_typem[2]);
pl_mm[9]=pl_mm[7]+i+strlen(txt_typem[3]);

m.color1=get_color(txt_win1_menu,colors);
m.color2=get_color(txt_win1_menusel,colors);
m.color3=get_color(txt_win1_menuhot,colors);
m.place=pl_m;
m.key=key_m;
m.l=TYPE_WIDTH;
m.hkey=NULL;
m.hplace=NULL;
w=open_cwin(18,TYPE_WIDTH+10);
m.w=w;
m.used=m.sel=0;
mm.w=w;
mm.l=0;
mm.place=pl_mm;
mm.sel=mm.used=0;
mm.txt=txt_typem;
mm.color1=get_color(txt_win1_menu,colors);
mm.color2=get_color(txt_win1_menusel,colors);
mm.color3=get_color(txt_win1_menuhot,colors);
mm.color4=get_color(txt_win1_menuhotsel,colors);
mm.hkey=hkey_typem;
mm.hplace=hplace_typem;
mm.amount=6;
mm.key=key_mm;
leaveok(w,TRUE);
curs_set(0);
keypad(w,TRUE);
nodelay(w,FALSE);
wbkgdset(w,get_color(txt_win1,colors));
werase(w);
box(w,0,0);
win_box(w,12,TYPE_WIDTH+3,1,3);

cc=view_exec_2_strs(&numberOfTypes);
m.txt=cc;

/*
 * get the string array of the types.
 */
do { 

  if (numberOfTypes<11) {

    m.amount=numberOfTypes;
    wmove(w,numberOfTypes+2,5);
    wbkgdset(w,get_color(txt_win1,colors));
    for(i=0;i<TYPE_WIDTH;i++) waddch(w,' ');
    wrefresh(w);
  }
  else m.amount=11;

  /* 
   * show the main menu
   */

  menu_print(&mm);
  wrefresh(w);

  /*
   * selectedType boundaries
   */
  if(numberOfTypes<=1) {
    selectedType=0;
  } else {
    if(selectedType>numberOfTypes-1) selectedType=numberOfTypes-1;
  }

  /*
   * show the types
   */

  menu_print(&m);

  if (numberOfTypes) i=scroll_menu(&m,numberOfTypes,NULL,&selectedType);
    else i=2;

  mm.sel=0;

  switch (i) {

    case 0 :
              /*
               * type is selected open edit window
               */
              open_type_venster(txt_c_type,txt_typename,txt_typeval,m.sel,1);
              touchwin(w);
              wrefresh(w);
              break;
    case 1 :
              /*
              * break (ESC pressed)
              * set main menu selection to [ Done ]
              */
              mm.sel=4;
              break;
    case 2 :
              /*
               * extra key pressed [ Cancel ]
               */
              do {

                s=menu_key(&mm);
                if ((s==2)&&(mm.sel==5)) break; 

              } while(s<3);

              if (s==5) mm.sel=4;
              break; 
    default:  break;

  }

  if(i) {

    switch(mm.sel) {
      case 0 :
              /*
               * [ Insert ]
               */
               open_type_venster(txt_n_type,txt_typename,txt_typeval,m.sel,0);
               break;
      case 1 :
               /*
                * [ Add ]
                */
               if (m.amount) open_type_venster(txt_n_type,txt_typename,txt_typeval,m.sel+1,0);
                  else open_type_venster(txt_n_type,txt_typename,txt_typeval,m.sel,0);
               break;
      case 2 :
               /*
                * [ Modify ]
                */
               if (numberOfTypes) open_type_venster(txt_c_type,txt_typename,txt_typeval,m.sel,1);
               break;
      case 3 :
               /*
                * [ Delete ]
                */
               if (numberOfTypes) {
                  view_rm_view_exec(m.sel);
               }
               break;
      case 4 :
               /*
                * [ Done ]
                */
               break;
      }

  }

  touchwin(w);
  wrefresh(w);

  m_selected=m.txt-cc;

  /*
   * dump old array, and get the new one from execType
   */

  free_string_array(cc);
  cc=view_exec_2_strs(&numberOfTypes);
  m.txt=cc;

  if (m_selected>numberOfTypes) m_selected=numberOfTypes;
  m.txt=cc+m_selected;

  if(m.txt>cc) {
    if ((m_selected+m.amount)>numberOfTypes) { --m.sel;--m.txt; };
  }

} while ((mm.sel<4)||(mm.sel==5)); 
delwin(w);
free_string_array(cc);
}

void init_pullmenu_colors (MENU *m,chtype color1,chtype color2, chtype color3, chtype color4) {

  MENU **mp;
  int i,n;

  if(m==NULL) return;
  m->color1=color1;
  m->color2=color2;
  m->color3=color3;
  m->color4=color4;

  n=m->amount;
  mp=m->next;

  if(mp==NULL) return;

  for(i=0;i<n;i++) {
    if(mp[i]!=NULL) {
      mp[i]->color1=color1;
      mp[i]->color2=color2;
      mp[i]->color3=color3;
      mp[i]->color4=color4; 
    }
  }

}

void init_colors (struct color *colors) {

  int i=0;

  /* Initialize the ansi color table, use the normal color by default */

  for(i=0;i<=ANSI_SGR_PARAMS;i++) ansi_colors[i]=get_color(txt_view,colors);

  /*
   * Implement the ansi colors see https://en.wikipedia.org/wiki/ANSI_escape_code
   * Only the basic colors (bold, italic, underline, etc. Are supported for now
   */

  ansi_colors[0]=get_color(txt_view,colors);
  ansi_colors[1]=get_color(txt_view_bold,colors);
  ansi_colors[2]=get_color(txt_view_bold,colors);
  ansi_colors[3]=get_color(txt_view_italic,colors);
  ansi_colors[4]=get_color(txt_view_underline,colors);
  ansi_colors[5]=get_color(txt_view_blink,colors);
  ansi_colors[6]=get_color(txt_view_blink,colors);
  ansi_colors[7]=get_color(txt_view_reverse,colors);

  bv.ansi_colors=ansi_colors;

  /* Initialize menu colors */

  m_jn.color1=get_color(txt_win2,colors);
  m_jn.color2=get_color(txt_win2_menusel,colors);
  m_jn.color3=get_color(txt_win2_menuhot,colors);
  m_jn.color4=get_color(txt_win2_menuhotsel,colors);

  m_ok.color1=get_color(txt_win2,colors);
  m_ok.color2=get_color(txt_win2_menusel,colors);
  m_ok.color3=get_color(txt_win2_menuhot,colors);
  m_ok.color4=get_color(txt_win2_menuhotsel,colors);

}

void set_main_colors() {

  wbkgdset(bv.win,get_color(txt_view_bold,colors));
  wbkgdset(win2,get_color(txt_status_bar,colors));
  wbkgdset(hm_win,get_color(txt_main_menu,colors));
  init_pullmenu_colors(hm_menup,get_color(txt_main_menu,colors),get_color(txt_main_menusel,colors),get_color(txt_main_menuhot,colors),get_color(txt_main_menuhotsel,colors));
  menu_print(hm_menup);
  touchwin(win2);
  touchwin(hm_menup->w);
  wrefresh(win2);
  wrefresh(hm_menup->w);
  bv.y-=(LINES-2);
  wbkgdset(bv.win,get_color(txt_view,colors));
  view_refresh();

}

void refresh_setcolors_win(WINDOW *w) {

  leaveok(w,TRUE);
  curs_set(0);
  keypad(w,TRUE);
  nodelay(w,FALSE);
  wbkgdset(w,get_color(txt_win1,colors));
  werase(w);
  box(w,0,0);
  win_box(w,12,20,1,3);

}

void open_color_venster(MENU *setcolors_m, struct color *colors,int p) {

  unsigned pl_cm_color[]={2,5,4,5,6,5,8,5,8,19,8,33};
  unsigned pl_cm_mono[]={2,5,4,5,8,5,8,19,8,33};
  unsigned pl_sc[]={1,1,2,1,3,1,4,1,5,1,6,1,7,1,8,1};
  int key_cm[]={0,9,'\n',0,0,27};
  int key_sub[]={KEY_UP,KEY_DOWN,'\n',0,0,27};
  int x,y;
  int cancel=0;
  int try=0;
  int ok=0;
  int last_color=0;
  MENU *sub_NULL[]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
  MENU cm,fg_m,bg_m,attr_m;
  MENU *sub_cm_color[]={&fg_m,&bg_m,&attr_m,NULL,NULL,NULL};
  MENU *sub_cm_mono[]={&fg_m,&bg_m,NULL,NULL,NULL};
  struct color backup_color;

  cp_color(&backup_color,&colors[p]);

  /* cm.w init */
 
  cm.w=open_cwin(10,48);
  leaveok(cm.w,TRUE);
  curs_set(0);
  keypad(cm.w,TRUE);
  nodelay(cm.w,FALSE);
  wbkgdset(cm.w,get_color(txt_win2,colors));
  werase(cm.w);
  box(cm.w,0,0);
  wrefresh(cm.w);

  /* cm menu init */

  cm.sel=0;
  cm.used=0;

  if(!color_mode) {

     cm.txt=txt_cm_color;
     cm.place=pl_cm_color;
     cm.hkey=hkey_cv_color;
     cm.hplace=hplace_cv_color;
     cm.next=sub_cm_color;
     last_color=2;

   } else {

     cm.txt=txt_cm_mono;
     cm.place=pl_cm_mono;
     cm.hkey=hkey_cv_mono;
     cm.hplace=hplace_cv_mono;
     cm.next=sub_cm_mono;
     last_color=1;

    }
  
    cm.amount=number_of_strings(cm.txt);
    cancel=cm.amount-1;
    try=cancel-1;
    ok=try-1;
    
    cm.key=key_cm;
    cm.parent=NULL;
    cm.call_open=NULL;
    cm.call_close=NULL;
    cm.l=0;
    cm.call_close=NULL;
        
    if (color_mode==0) {

      fg_m.sel=color_to_number(colors[p].fg);
      bg_m.sel=color_to_number(colors[p].bg);
      attr_m.sel=attr_to_number(colors[p].attr);
      bg_m.txt=txt_kleuren;
      fg_m.txt=txt_kleuren;
      attr_m.txt=txt_attributen;

    } else {

      fg_m.sel=attr_to_number(colors[p].fg);
      bg_m.sel=attr_to_number(colors[p].bg);
      bg_m.txt=txt_attributen;
      fg_m.txt=txt_attributen;
      attr_m.txt=NULL;
  
    }

    bg_m.amount=number_of_strings(bg_m.txt);
    bg_m.place=pl_sc;
    bg_m.key=key_sub;
    bg_m.hkey=NULL;
    bg_m.hplace=NULL;
    bg_m.parent=&cm;
    bg_m.next=sub_NULL;
    bg_m.call_open=open_fg;
    bg_m.call_close=cm_close;
    bg_m.l=10;

    fg_m.amount=number_of_strings(fg_m.txt);
    fg_m.place=pl_sc;
    fg_m.key=key_sub;
    fg_m.hkey=NULL;
    fg_m.hplace=NULL;
    fg_m.parent=&cm;
    fg_m.next=sub_NULL;
    fg_m.call_open=open_fg;
    fg_m.call_close=cm_close;
    fg_m.l=10;

    attr_m.amount=number_of_strings(attr_m.txt);
    attr_m.place=pl_sc;
    attr_m.key=key_sub;
    attr_m.hkey=NULL;
    attr_m.hplace=NULL;
    attr_m.parent=&cm;
    attr_m.next=sub_NULL;
    attr_m.call_open=open_fg;
    attr_m.call_close=cm_close;
    attr_m.l=10;

    init_pullmenu_colors(&cm,get_color(txt_win2,colors),get_color(txt_win2_menusel,colors),get_color(txt_win2_menuhot,colors),get_color(txt_win2_menuhotsel,colors));

    for(;;) {

      char kar='A';
      wbkgdset(cm.w,get_color(txt_win2,colors));
      werase(cm.w);
      box(cm.w,0,0);

      win_box(cm.w,2,27,1,4);
      win_box(cm.w,4,27,1,4);

      if (!color_mode) win_box(cm.w,6,27,1,4);

      win_box(cm.w,6,10,1,33);
        
      wbkgdset(cm.w,get_color(txt_win2,colors));
      mvwaddstr(cm.w,2,17,">");
      mvwaddstr(cm.w,2,20,fg_m.txt[fg_m.sel]);

      mvwaddstr(cm.w,1,34," Example "); 
      wbkgdset(cm.w,colors[p].ch);

      for (y=1;y<6;y++) {

        for(x=1;x<10;x++) 
          mvwaddch(cm.w,y+1,x+33,kar++);
      }

      wbkgdset(cm.w,get_color(txt_win2,colors));
      mvwaddstr(cm.w,4,17,">");
      mvwaddstr(cm.w,4,20,bg_m.txt[bg_m.sel]);

      if (!color_mode) { 

        mvwaddstr(cm.w,6,17,">");
        mvwaddstr(cm.w,6,20,txt_attributen[attr_m.sel]);

      }

      wrefresh(cm.w);
      menu_pull(&cm,0);
  
      if (cm.used==2) break; 
      if (cm.sel>last_color) {

      if (cm.sel==cancel) {

        set_color(color_vars[p],
        colors,
        backup_color.fg,
        backup_color.bg,
        backup_color.attr,
        color_mode);

      } else {

        if(!color_mode) {

          set_color(color_vars[p],
          colors,number_to_color(fg_m.sel),
          number_to_color(bg_m.sel),
          number_to_attr(attr_m.sel),
          color_mode);

        } else {

          set_color(color_vars[p],
          colors,number_to_attr(fg_m.sel),
          number_to_attr(bg_m.sel), 0, color_mode);

        }

      }
  
      init_colors(colors);
      set_main_colors();
      init_pullmenu_colors(&cm,get_color(txt_win2,colors),get_color(txt_win2_menusel,colors),get_color(txt_win2_menuhot,colors),get_color(txt_win2_menuhotsel,colors));
      refresh_setcolors_win(setcolors_m->w);
      setcolors_m->next[0]->color1=setcolors_m->color1=get_color(txt_win1_menu,colors);
      setcolors_m->next[0]->color2=setcolors_m->color2=get_color(txt_win1_menusel,colors);
      setcolors_m->next[0]->color3=setcolors_m->color3=get_color(txt_win1_menuhot,colors);
      setcolors_m->next[0]->color4=setcolors_m->color4=get_color(txt_win1_menuhotsel,colors);
      menu_print(&cm);
      menu_print(setcolors_m);
      menu_print(setcolors_m->next[0]);
      touchwin(setcolors_m->w);
      touchwin(cm.w);
      wrefresh(setcolors_m->w);

      if ((cm.sel==ok)||(cm.sel==cancel)) break;

    }

  }

}

/* -------------------------------------------- */
/* Funktie voor de kleuren...                   */
/* -------------------------------------------- */
void set_colors ()
{

   MENU m,mm;
   MENU *m_sub[]={&mm};
   WINDOW *w;
   int key_m[]={KEY_UP,KEY_DOWN,'\n',KEY_PPAGE,KEY_NPAGE,27,9};
   int key_mm[]={0,9,'\n',' ',27,'\n'};
   unsigned pl_m[]={2,5,3,5,4,5,5,5,6,5,7,5,8,5,9,5,10,5,11,5,12,5,13,5,14,5};
   unsigned pl_mm[]={2,27,4,27,6,27,2,5};
   int s,i,brol=0;
   char **cc=NULL;
   unsigned u=0;
   struct color *backup_colors;

   backup_colors=xcalloc(color_array_members(colors),sizeof(struct color));
   cp_color_array(backup_colors,colors);
   cc=colors_2_str(colors,&brol);

   m.color1=get_color(txt_win1_menu,colors);
   m.color2=get_color(txt_win1_menusel,colors);
   m.color3=get_color(txt_win1_menuhot,colors);
   m.place=pl_m;
   m.key=key_m;
   m.l=18;
   m.hkey=NULL;
   m.hplace=NULL;

   w=open_cwin(15,42);
   setcolors_w=m.w=w;
   m.used=m.sel=0;
   m.next=m_sub;

   mm.w=w;
   mm.l=0;
   mm.place=pl_mm;
   mm.sel=mm.used=0;
   mm.amount=number_of_strings(txt_colorm);
   mm.txt=xcalloc(mm.amount+2,sizeof(char *));

   copy_string_array_pointers(mm.txt,txt_colorm);

   mm.color1=get_color(txt_win1_menu,colors);
   mm.color2=get_color(txt_win1_menusel,colors);
   mm.color3=get_color(txt_win1_menuhot,colors);
   mm.color4=get_color(txt_win1_menuhotsel,colors);
   mm.hkey=hkey_colorm;
   mm.hplace=hplace_colorm;
   mm.key=key_mm;

   refresh_setcolors_win(w);

   m.txt=cc; 
   m.sel=u=0;

   do { 

      if (brol<11) {

        m.amount=brol;
        wmove(w,brol+2,5);
        wbkgdset(w,get_color(txt_win1,colors));

        for(i=0;i<TYPE_WIDTH;i++) waddch(w,' ');

        wrefresh(w);
      }
      else m.amount=11;

      menu_print(&mm);
      wrefresh(w);

      if (u>brol-1) u=brol-1;

      menu_print(&m);

      if (brol) i=scroll_menu(&m,brol,NULL,&u);
      else i=2;

      mm.sel=0;

      switch (i) {

        case 0 :  

            open_color_venster(&m,colors,m.sel);
            touchwin(bv.win);
            wrefresh(bv.win);
            touchwin(w);
            wrefresh(w);

            break;

        case 1 :  

            mm.sel=4;
            break;
   
        case 2 :

            do {

              s=menu_key(&mm);
              if ( (s==2) && (mm.sel==mm.amount-1) ) break; 

            } while(s<3);
       
            if (s==4) mm.sel=mm.amount-1;

            break; 

        default:  

            break;

      } /* switch */

      if (i) {

         switch(mm.sel) {

          case 0 :  

              open_color_venster(&m,colors,m.sel);
              touchwin(bv.win);
              wrefresh(bv.win);
              touchwin(w);
              wrefresh(w);

              break;

          case 1 :

              break;

          case 2 : 

              cp_color_array(colors,backup_colors); 
              init_colors(colors);
              reinit_color_table(colors,color_mode);
              set_main_colors();

              break;

          default:  

              break;

          }
      }

      touchwin(w);
      wrefresh(w);

   } while ((mm.sel<1)||(mm.sel==mm.amount-1)); 

   delwin(w);
   xfree(backup_colors);
   xfree(mm.txt);

}

void cp2stdout(FILE *fp) {

  int c;

  while(1) {

    if ((c=getc(fp))==EOF) break;

    fputc(c,stdout);

  }

  fclose(fp);
  fclose(stdout);

}

FILE * get_config_file (const char *opentype) {

  FILE *fp;
  char *c,*file;

  if ((c=getenv("HOME"))==NULL) return(NULL); 
    else {

      file=(char *) xmalloc (strlen(c)+strlen("/.lookat")+1);
      strcpy(file,c);
      strcat(file,"/.lookat");
      fp=fopen(file,opentype); 
      xfree(file);

  }

  if (fp==NULL) fp=fopen(CONFIGFILE,opentype); 

  return (fp);

}

/*
 * usage function
 */
void usage() {

  if (COLS) endwin();

  /*
   * create txt_help
   */

   char *txt_prgname_version=combine_strings( (char *[]){txt_help_prgname," ",PACKAGE_VERSION,NULL});
   char *txt_help_line=str_nchars(help_width,'-');
   char *txt_help_copyright=combine_strings( (char *[]) {txt_gpl_short," ",txt_prg_dates," ",txt_belgie,NULL });
   char *txt_help_1st_line=right_align_2_strings(txt_prgname_version,txt_help_copyright,help_width);

   char *txt_help_2nd_line=right_align_2_strings("",txt_author,help_width);
   char *txt_help_3rd_line=right_align_2_strings("",txt_author_email,help_width);

   char *txt_help_head[] = {
      txt_help_line,
      txt_help_1st_line,
      txt_help_2nd_line,
      txt_help_3rd_line,
      NULL
   };

   char *txt_help_footer[] = {
      txt_help_line,
      NULL
   };

   char **txt_help_body_footer=combine_string_array_pointers(txt_help_body,txt_help_footer);
   char **txt_help=combine_string_array_pointers(txt_help_head,txt_help_body_footer);

   print_strs(stderr,txt_help);

   xfree(txt_prgname_version);
   xfree(txt_help_line);
   xfree(txt_help_copyright);
   xfree(txt_help_1st_line);
   xfree(txt_help_2nd_line);
   xfree(txt_help_3rd_line);
   xfree(txt_help_body_footer);
   xfree(txt_help);

}
/* ---------------------------------------------------- */ 
/* Hoofdprg ...           */
/* ---------------------------------------------------- */ 
int main (int argn,char **argv)
{

int i,c;
int prev_lines=0;
int prev_cols=0;

WINDOW *sub_win;
char **cp,*sp;
unsigned  pl_hm[]={0,1,0,9,0,12,0,15,0,17};
unsigned  pl_bm[]={1,1,3,1,5,1};
unsigned  pl_gm[]={1,1,2,1,3,1,4,1,5,1,};
unsigned  pl_zm[]={1,1,2,1,3,1,};
unsigned  pl_om[]={1,1,2,1,4,1,5,1,7,1};
unsigned  pl_hlpm[]={1,1,2,1,3,1,};
int key_sub[]={KEY_UP,KEY_DOWN,'\n',KEY_LEFT,KEY_RIGHT,27};
int key_hm[]={KEY_LEFT,KEY_RIGHT,'\n',KEY_UP,KEY_DOWN,27};
MENU hm,bm,gm,zm,om,hlpm;
MENU *sub_hm[]={&bm,&gm,&zm,&om,&hlpm};
MENU *sub_NULL[]={NULL,NULL,NULL,NULL,NULL,NULL};
FILE *fp;
char ***ccc;
s[0]=0;

char *prgname=NULL;
char *arg_filename=NULL;
int hlpflag=0;
int rawflag=0;
int arg_errors=0;
int verboseflag=0;
/* flag to open the file window, don't open it by default */
int open_filewin_flag=0;

/*
 * Based on CGIpaf changepass.c code
 *
 * argument handling, not using get long opts as this not supported on older Un!x systems
 * We parse the arguments first as it might be not required to do
 * anything-else in case of the error or when the user provide the help argument
 */

 prgname=basename(argv[0]);    /* set prgname to the real program name */

if (argn>1) {                  /* we've arguments */

	 int i;
	 char *longargs[]={"help","raw","verbose",NULL};

	 /*
	  * convert long opts to shorts opt
	  */

	 for (i=1;i<argn;i++) {

	   char *shortarg=NULL;

		 if (strncmp("--",argv[i],2)==0) {

			 char **ccp;
			 int found=0;

			 for (ccp=longargs;*ccp!=NULL;++ccp) {

					if (!strcmp(argv[i]+2,*ccp)) {
			   	
						shortarg=xmalloc(3*sizeof(char));
						shortarg[0]=0;
						strncat(shortarg,(argv[i]+1),2);
						argv[i]=shortarg;
						found=1;
						break;

					}

				}

				if (!found) {
					 fprintf(stderr,"%s: Unkown argument %s\n",prgname,argv[i]);
           arg_errors=1;
				   hlpflag=1;
           exit(1);
				}
     } 

   }

   if (!hlpflag) {

	    while ((i = getopt(argn, argv, "hrv")) != -1) {

			    switch (i) {

		   		  case 'h':
						  hlpflag=1;
						  break;
		   		  case 'r':
						  rawflag=1;
              display_raw=1;
						  break;
		   		  case 'v':
						  verboseflag=1;
						  break;
            default:
						  hlpflag=1;
              arg_errors=1;
					    fprintf(stderr,"%s: Unkown argument\n",prgname);
						  break;
		   	  }
	    }

      if (optind < argn) {

        arg_filename=argv[optind];

      }

		}

    if (verboseflag) {

        fprintf(stderr,"\nhlpflag %d \n",hlpflag);
        fprintf(stderr,"rawflag %d \n",rawflag);
        fprintf(stderr,"verboseflag %d \n",verboseflag);
        if (arg_filename != NULL ) fprintf(stderr,"arg_filename %s \n",arg_filename);
          else  fprintf(stderr,"arg_filename not specified\n");
    }

    if (hlpflag || arg_errors) {
      usage();

      if (arg_errors) exit(1);

      exit(0);

    }

}

/*
 * Basic menu init
 */

m_jn.txt=txt_jn;
m_jn.hkey=hkey_jn;
m_jn.hplace=hplace_jn;
m_ok.txt=txt_ok;
m_ok.hkey=hkey_ok;
m_ok.hplace=hplace_ok;

/*
 * utf-8
 */

setlocale(LC_ALL, "");

/*
 * Parse the config
 */
{

char *c;
char **type=NULL;
int tt=0;
bv.view_exec=xmalloc(sizeof(char ***)); 
*bv.view_exec=NULL;

if ((fp=get_config_file("r"))==NULL) bv.mode=0;
else {
  c=lookat_get_config(fp,"cursor");

  if (c!=NULL) {
    if (!strcmp(c,"on")) {txt_om1[1]='X';bv.mode=1;}
    else bv.mode=0;
    xfree(c);
  }
  c=lookat_get_config(fp,"give_notice");

  if (c!=NULL) {
    if (!strcmp(c,"off")) txt_om2[1]=' ';
    xfree(c);
   }

   if(fp!=NULL) rewind(fp);

   while ((type=lookat_get_next_config(fp))!=NULL) {

     if (type[0][0]=='.') {
       bv.view_exec=xrealloc(bv.view_exec,sizeof(char ***)*(tt+1));
       bv.view_exec[tt]=type;
       ++tt;
     }
     else {
       free_string_array(type);
     }

   }

   bv.view_exec=xrealloc(bv.view_exec,sizeof(char ***)*(tt+1));
   bv.view_exec[tt]=NULL; 
  }

}

bv.file=NULL;
bv.filename=NULL;
bv.cmd=1;
bv.load=0;
bv.txt_f_open2=txt_f_open2;
bv.txt_f_open1=txt_f_open1;
bv.txt_f_load=txt_f_laden;
bv.txt_f_fseek=txt_f_fseek;
bv.txt_f_freopen=txt_f_freopen;

if (!(initscr())) {
   fprintf(stderr,"%s",txt_f_initscr);exit(1);
}

set_memerr(outofmemory);

/*
 * Store the LINES and COLS for resize detect
 */

prev_lines=LINES;
prev_cols=COLS;

bv.lines=LINES-2;
bv.cols=COLS;
bv.sx=bv.sy=0;
view_par(&bv);

/* Detect if we're reading from stdin */
if (!isatty(STDIN_FILENO)) {

  /* just print the file when not on a terminal */
  if (!isatty(STDOUT_FILENO)) {

      cp2stdout(stdin);
      fclose(stdin);
      fclose(stdout);
      wexit(0);

  }

  bv.cmd=2;

  bv.filename=(char *)xmalloc(strlen("STDIN")+1);
  strcpy(bv.filename,"STDIN");

  if (bekijk_view_load()) {

      endwin();
      fprintf(stdout,"Sorry view_load() failed");
      exit(1);

  };

  if (bekijk_freopen("/dev/tty","r",stdin,win1)) {

     endwin();
     exit(1);

  };

} else {

  if (arg_filename!=NULL) { 

      /* check if arg_filename is a directory */
      if (chdir(arg_filename)==-1) {

        bv.filename=(char *)xmalloc(strlen(arg_filename)+1);
        strcpy(bv.filename,arg_filename);

        if (isatty(STDOUT_FILENO)) {

          if (bekijk_view_load()) {
            wexit(1);
          }

        }
        else {

          FILE *fp;
          if ((fp=fopen(bv.filename,"r"))==NULL) {
            fprintf(stderr,"%s %s %s",txt_f_open1,bv.filename,txt_f_open2); 
            fclose(stdout);
            wexit(1);
          }
          cp2stdout(fp);
          wexit(0);

        }
      }
  } else {

    /*
     * we only open the file selection window if if there is no filename argument
     * and we don't read from stdin
     */
    open_filewin_flag=1;
  }
}

if (!isatty(STDOUT_FILENO)) wexit(1);
bv.cmd=0;
cp=bv.file;bv.x=0;s[0]=0;
win1=open_win(LINES-2,COLS,1,0);
win2=open_win(1,COLS,LINES-1,0);
hm_win=open_win(1,COLS,0,0);
hm.w=hm_win;
keypad(hm.w,TRUE);
start_color();

{
  color_array=new_color_table(color_vars);
  mono_array=new_color_table(color_vars);
 
  set_color(txt_view,mono_array,A_NORMAL,A_NORMAL,0,1);
  set_color(txt_view_bold,mono_array,A_BOLD,A_NORMAL,0,1);
  set_color(txt_view_italic,mono_array,A_BOLD,A_NORMAL,0,1);
  set_color(txt_view_underline,mono_array,A_UNDERLINE,A_NORMAL,0,1);
  set_color(txt_view_blink,mono_array,A_BLINK,A_NORMAL,0,1);
  set_color(txt_view_reverse,mono_array,A_REVERSE,A_NORMAL,0,1);
  set_color(txt_main_menu,mono_array,A_REVERSE,A_NORMAL,0,1);
  set_color(txt_main_menusel,mono_array,A_NORMAL,A_NORMAL,0,1);
  set_color(txt_main_menuhot,mono_array,A_REVERSE,A_BOLD,0,1);
  set_color(txt_main_menuhotsel,mono_array,A_BOLD,A_NORMAL,0,1);
  set_color(txt_status_bar,mono_array,A_REVERSE,A_NORMAL,0,1);
  set_color(txt_win1,mono_array,A_REVERSE,A_NORMAL,0,1);
  set_color(txt_win1_edit,mono_array,A_NORMAL,A_NORMAL,0,1);
  set_color(txt_win1_menu,mono_array,A_REVERSE,A_NORMAL,0,1);
  set_color(txt_win1_menusel,mono_array,A_NORMAL,A_NORMAL,0,1);
  set_color(txt_win1_menuhot,mono_array,A_REVERSE,A_BOLD,0,1);
  set_color(txt_win1_menuhotsel,mono_array,A_NORMAL,A_BOLD,0,1);
  set_color(txt_win2,mono_array,A_REVERSE,A_NORMAL,0,1);
  set_color(txt_win2_edit,mono_array,A_NORMAL,A_NORMAL,0,1);
  set_color(txt_win2_menu,mono_array,A_REVERSE,A_NORMAL,0,1);
  set_color(txt_win2_menusel,mono_array,A_NORMAL,A_NORMAL,0,1);
  set_color(txt_win2_menuhot,mono_array,A_REVERSE,A_BOLD,0,1);
  set_color(txt_win2_menuhotsel,mono_array,A_NORMAL,A_BOLD,0,1);
  set_color(txt_view_found,mono_array,A_REVERSE,A_NORMAL,0,1);
    
  set_color(txt_view,color_array,COLOR_WHITE,COLOR_BLACK,A_NORMAL,color_mode);
  set_color(txt_view_bold,color_array,COLOR_WHITE,COLOR_BLACK,A_BOLD,color_mode);
  set_color(txt_view_italic,color_array,COLOR_RED,COLOR_BLACK,A_BOLD,color_mode);
  set_color(txt_view_underline,color_array,COLOR_YELLOW,COLOR_BLACK,A_UNDERLINE,color_mode);
  set_color(txt_view_blink,color_array,COLOR_WHITE,COLOR_BLACK,A_BLINK,color_mode);
  set_color(txt_view_reverse,color_array,COLOR_WHITE,COLOR_BLACK,A_REVERSE,color_mode);
  set_color(txt_main_menu,color_array,COLOR_WHITE,COLOR_BLUE,A_NORMAL,color_mode);
  set_color(txt_main_menusel,color_array,COLOR_BLUE,COLOR_WHITE,A_NORMAL,color_mode);
  set_color(txt_main_menuhot,color_array,COLOR_GREEN,COLOR_BLUE,A_BOLD,color_mode);
  set_color(txt_main_menuhotsel,color_array,COLOR_GREEN,COLOR_WHITE,A_BOLD,color_mode);
  set_color(txt_status_bar,color_array,COLOR_WHITE,COLOR_BLUE,A_NORMAL,color_mode);
  set_color(txt_win1,color_array,COLOR_WHITE,COLOR_BLUE,A_NORMAL,color_mode);
  set_color(txt_win1_edit,color_array,COLOR_BLACK,COLOR_WHITE,A_NORMAL,color_mode);
  set_color(txt_win1_menu,color_array,COLOR_WHITE,COLOR_BLUE,A_NORMAL,color_mode);
  set_color(txt_win1_menusel,color_array,COLOR_WHITE,COLOR_BLACK,A_NORMAL,color_mode);
  set_color(txt_win1_menuhot,color_array,COLOR_GREEN,COLOR_BLUE,A_BOLD,color_mode);
  set_color(txt_win1_menuhotsel,color_array,COLOR_GREEN,COLOR_BLACK,A_BOLD,color_mode);
  set_color(txt_win2,color_array,COLOR_BLUE,COLOR_WHITE,A_NORMAL,color_mode);
  set_color(txt_win2_edit,color_array,COLOR_WHITE,COLOR_BLACK,A_NORMAL,color_mode);
  set_color(txt_win2_menu,color_array,COLOR_BLACK,COLOR_WHITE,A_BOLD,color_mode);
  set_color(txt_win2_menusel,color_array,COLOR_WHITE,COLOR_BLUE,A_NORMAL,color_mode);
  set_color(txt_win2_menuhot,color_array,COLOR_YELLOW,COLOR_WHITE,A_BOLD,color_mode);
  set_color(txt_win2_menuhotsel,color_array,COLOR_YELLOW,COLOR_BLUE,A_BOLD,color_mode);
  set_color(txt_view_found,color_array,COLOR_WHITE,COLOR_GREEN,A_BOLD,color_mode);
  
  if(!has_colors()) {
    colors=mono_array;
    color_mode=1;
  }
  else {
    colors=color_array;
    color_mode=0;
  }

  if (fp!=NULL) rewind(fp);
  read_config_colors(fp,colors,color_mode);
  init_colors(colors);
  wbkgd(win1,get_color(txt_view,colors));

};

if(fp!=NULL) fclose(fp);

wbkgd(win2,get_color(txt_status_bar,colors));
wbkgd(hm_win,get_color(txt_main_menu,colors));
signal(SIGINT,SIG_IGN);
signal(SIGTERM,terminate);

/* ------------------------------------------------------------------------- */
/* START MENU INIT                   */
/* ------------------------------------------------------------------------- */

hm.sel=0;
hm.used=0;
hm.amount=5;
hm.place=pl_hm;
hm.txt=txt_hm;
hm.key=key_hm;
hm.hkey=NULL;
hm.next=sub_hm;
hm.parent=NULL;
hm.call_open=NULL;
hm.call_close=NULL;
init_pullmenu_colors(&hm,get_color(txt_main_menu,colors),get_color(txt_main_menusel,colors),get_color(txt_main_menuhot,colors),get_color(txt_main_menuhotsel,colors));
hm_menup=&hm;
bm.sel=gm.sel=zm.sel=om.sel=hlpm.sel=0;
bm.place=pl_bm;
gm.place=pl_gm;
zm.place=pl_zm;
om.place=pl_om;
hlpm.place=pl_hlpm;
bm.txt=txt_bm;
gm.txt=txt_gm;
zm.txt=txt_zm;
om.txt=txt_om;
hlpm.txt=txt_hlpm;
bm.next=gm.next=zm.next=om.next=hlpm.next=sub_NULL;
bm.parent=gm.parent=zm.parent=om.parent=hlpm.parent=&hm;
bm.key=gm.key=zm.key=om.key=hlpm.key=key_sub;
bm.call_open=open_bm;
gm.call_open=open_gm;
zm.call_open=open_zm;
om.call_open=open_om;
hlpm.call_open=open_hlpm;
bm.call_close=gm.call_close=zm.call_close=om.call_close=hlpm.call_close=m_close;
bm.w=gm.w=zm.w=om.w=hlpm.w=sub_win;
bm.amount=3;
hlpm.amount=3;
zm.amount=3;
om.amount=5;
gm.amount=5;
hm.l=bm.l=gm.l=zm.l=om.l=hlpm.l=0;
bm.hkey=hkey_bm;
gm.hkey=hkey_gm;
zm.hkey=hkey_zm;
om.hkey=hkey_om;

hlpm.hkey=hkey_hlp;
hm.hplace=hplace_hm;
bm.hplace=hplace_bm;
gm.hplace=hplace_gm;
om.hplace=hplace_om;
zm.hplace=hplace_zm;
hlpm.hplace=hplace_hlpm;

pl_hm[3]=strlen(txt_hm[0])+2;
pl_hm[5]=strlen(txt_hm[0])+strlen(txt_hm[1])+3;
pl_hm[7]=pl_hm[5]+strlen(txt_hm[2])+1;
pl_hm[9]=COLS-strlen(txt_hm[3])-2;

for (i=0;i<bm.amount;i++) 
  if (strlen(bm.txt[i])>bm.l) bm.l=strlen(bm.txt[i]);

for (i=0;i<gm.amount;i++) 
  if (strlen(gm.txt[i])>gm.l) gm.l=strlen(gm.txt[i]);

for (i=0;i<zm.amount;i++) 
  if (strlen(zm.txt[i])>zm.l) zm.l=strlen(zm.txt[i]);

for (i=0;i<om.amount;i++) 
  if (strlen(om.txt[i])>om.l) om.l=strlen(om.txt[i]);

for (i=0;i<hlpm.amount;i++) 
  if (strlen(hlpm.txt[i])>hlpm.l) hlpm.l=strlen(hlpm.txt[i]);
/* ------------------------------------------------------------------------- */
/* EINDE MENU INIT                         */
/* ------------------------------------------------------------------------- */

bv.m_ok=&m_ok;
bv.win=win1;
menu_print(&hm);
meta(win1,TRUE);
noecho();
cbreak();
leaveok(win1,TRUE);
curs_set(0);
keypad(win1,TRUE);
nodelay(win1,FALSE); 
nodelay(hm_win,FALSE);
leaveok(stdscr,TRUE);
leaveok(hm_win,TRUE);
curs_set(0);
keypad(stdscr,TRUE);
nodelay(stdscr,TRUE); 
keypad(hm_win,TRUE);
wrefresh(win1);
wrefresh(win2);

zoek_str.c=NULL;
zoek_str.n=32;
zoek_str.m=0;
zoek_str.count=0;
zoek_str.mode=0;
zoek_str.insert=1;


/*
 * Reinit hm_win and hm_menu
 */

wresize(hm_win,1,COLS);
werase(hm_win);
pl_hm[9]=COLS-strlen(txt_hm[3])-2;
menu_print(&hm);
touchwin(hm_win);
wrefresh(hm_win);

/* 
 * Reinit win2
 */

wresize(win2,1,COLS);
mvwin(win2,LINES-1,0);
werase(win2);
touchwin(win2);
wrefresh(win2);

/*
 * Reinit & Refresh view
 */

wresize(win1,LINES-2,COLS);
werase(win1);
touchwin(win1);
wrefresh(win1);

bv.lines=LINES-2;
bv.cols=COLS;

/*
 * refresh curscr
 */

wrefresh(curscr);

if (bekijk_freopen("/dev/null","w",stderr,win1)) {
  endwin();
  exit(1);
}

if (open_filewin_flag) { 

  do {
    if ( bekijk_open_best(1) > 1 ) {
        delwin(win1);
        delwin(win2);
        refresh();
        wexit(0);
      };
    } while (bv.file==NULL);

};

bv.y=0;view_refresh();
touchwin(win1);
touchwin(win2);
wrefresh(win1);
wrefresh(win2);

int firstStart=1;

do {

do {

   if (bv.mode) { 
                leaveok(win1,FALSE);
    curs_set(1);
    }
      else { 
             leaveok(win1,TRUE);
       curs_set(0);
           }
   werase(win2);
   mvwprintw(win2,0,0,"%s %4d / %4d  %s %4d %s %ld",
   txt_lijn,view_gety(),bv.y_max,txt_kol,view_getx(),txt_Grootte,bv.size);
   mvwprintw(win2,0,COLS-strlen(bv.filename)-3,"%s",bv.filename);
   touchwin(win1);
   touchwin(win2);
   wrefresh(win2);
   wrefresh(win1);

   /*
    * redraw the screen at first start or when het window gets resized
    */

   if (firstStart) {

     c='r';
     firstStart=0;

   } else {

      if ( prev_cols != COLS && prev_lines != LINES ) {

        c='r';

      } else {

        c=wgetch(win1);

      }
   }

   if ((c==KEY_F(9))||(c==27)) {

      int m_sel=0;

      if (c==27) {

        nodelay(win1,TRUE);
        c=wgetch(win1);

        if (c==ERR) c=27;
        else c=tolower(c);

        nodelay(win1,FALSE);

        if (c==sel_bm) m_sel=1;
          else
            if (c==sel_gm) m_sel=2;
              else
                if (c==sel_zm) m_sel=3;
                  else
                    if (c==sel_om) m_sel=4;
                      else       
                        if (c==sel_hm) m_sel=5;
                          else m_sel=6;
      };

      if (m_sel<6) {

        c=0;
        werase(win2);
        mvwaddstr(win2,0,0,txt_maak_sel);
        wrefresh(win2);

        for (i=0;i<4;i++) sub_hm[i]->used=0;

        menu_pull(&hm,m_sel);

        for (i=0;i<hm.amount;i++) if (sub_hm[i]->used) break;

        menu_print(&hm);
        werase(win2);
        wrefresh(win2);

        if (hm.used==2) c=0;
          else { /* if(hm.used==2) else */

            switch (hm.sel) { /* switch (hm.sel) */
              case 0:

                switch (sub_hm[i]->sel) {

                  case 0:

                    c='o';
                    break;

                  case 1:
                    c='r';
                    break;

                  case 2:
                    c='q';
                    break;

                  default:
                    break;

                }

                break; 

              case 1:

                switch (sub_hm[i]->sel) {

                  case 0:

                    c='t';
                    break;

                  case 1:

                    c='e';
                    break;

                  case 2:

                    ga_lijn();
                    break;

                  case 3:
                    
                    c=KEY_END;
                    break;

                  case 4:
                    c=KEY_HOME;
                    break;

                  default:
                    break;
                }

                break;

             case 2:

                switch (sub_hm[i]->sel) {

                    case 0:
                      c='z';
                      break;

                    case 1:
                      zoek_tekst(1);
                      c=0;
                      break;

                    case 2:
                      c=KEY_F(3);
                      break;

                }
                break;

            case 3:

                switch (sub_hm[i]->sel) {

                  case 0:

                    if (bv.mode) {

                      bv.mode=0;
                      txt_om1[1]=txt_spacie[0];

                    } else {

                        bv.mode=1;
                        txt_om1[1]=txt_X[0];
                        bv.y-=bv.lines;
                        view_refresh();
                    }

                    break;

                  case 1:

                    if (txt_om2[1]==' ') txt_om2[1]='X';
                      else txt_om2[1]=' ';

                    break;

                  case 2:
                    set_types();
                    break;

                  case 3:
                    set_colors();
                    break;

                  case 4:

                    if ((fp=get_config_file("w"))==NULL) {
                    #ifdef _NED
                      open_okwin(7,40,&m_ok,txt_f_writecfg,win1);touchwin(win1);wrefresh(win1);
                    #endif
                    #ifdef _ENG
                      open_okwin(6,40,&m_ok,txt_f_writecfg,win1);touchwin(win1);wrefresh(win1);
                    #endif

                    break;

                }

                if (bv.mode) save_config(fp,"cursor","on");
                  else save_config(fp,"cursor","off");

                if (txt_om2[1]=='X') save_config(fp,"give_notice","on");
                  else save_config(fp,"give_notice","off");

                ccc=bv.view_exec;

                while (*ccc) {

                  sp=xmalloc(strlen(ccc[0][1])+3);
                  strcpy(sp,"\"");
                  strcat(sp,ccc[0][1]);
                  strcat(sp,"\"");
                  save_config(fp,ccc[0][0],sp);
                  ++ccc;

                }

                save_colors(fp,color_array,0);
                save_colors(fp,mono_array,1);

                fclose(fp);
                break;

                };

                break;

            case 4:   

                switch (sub_hm[i]->sel) {

                  case 0:

                    c='?';
                    break;

                  case 1: 

                    open_okwin(22,60,&m_ok,txt_t,win1);
                    break;

                  case 2:

                    {

                      char *txt_prgname_version=combine_strings( (char *[]){txt_help_prgname," ",PACKAGE_VERSION,NULL});
                      char *txt_over_1=right_align_2_strings(txt_prgname_version,txt_gpl_2,over_width);
                      char *txt_over_2=right_align_2_strings(txt_author,txt_prg_dates,over_width);
                      char *txt_over_3=right_align_2_strings(txt_email,txt_author_email,over_width);
                      char *txt_over_4=right_align_2_strings(txt_homepage,txt_homepage_url,over_width);
                      char *txt_over[] = {
                        txt_over_1,
                        "\n",
                        txt_over_2,
                        txt_over_3,
                        txt_over_4,
                        "\n",
                        NULL
                      };

                      open_animwin(12,60,&m_ok,txt_over,txt_bedank,win1,0);
                      xfree(txt_prgname_version);
                      xfree(txt_over_1);
                      xfree(txt_over_2);
                      xfree(txt_over_3);
                      xfree(txt_over_4);

                    }
                    break;

                  default:
                    break;

                }

                break;

          default:

              c=0;
              break;

          } /* switch (hm.sel) */
        } /* if(hm.used==2) else */
      }
    } 
    switch (c) {

      case '<'      :
      case 't'      :
      case 'T'      :
      
        bv.y=0;bv.x=0;view_refresh();
        break;

    case 'b'      :
    case 'B'      :
    case '>'      :
    case 'E'      :
    case 'e'      :

        bv.y=bv.y_max;bv.x=0;view_refresh();
        break;

    case '\n'     :
    case 14       :
    case 'j'      :
    case KEY_DOWN :

        view_down();
        break;    

    case 16       :
    case 'k'      :
    case KEY_UP   :

        view_up();
        break;

    case ' '      :
    case 'd'      :
    case 'D'      :
    case KEY_NPAGE:

        view_refresh();
        break;

    case 'u'      :
    case 'U'      :
    case KEY_PPAGE:

        view_previous(); 
        break;  

    case 6        :
    case 'L'      :
    case 'l'      :
    case KEY_RIGHT:

        view_right();
        break;

    case 2        :
    case KEY_LEFT :

        view_left(); 
        break;

    case 'g'      :        
    case 'G'      :

        ga_lijn();
        break;

    case KEY_F(7) :
    case '/'      :
    case 'z'      :
    case 'Z'      :
    case 's'      :
    case 'S'      :
    case 'F'      :
    case 'f'      :

        zoek_tekst(0);
        break;

    case 'n'      :
    case '\\'     :
    case 'v'      :
    case 'V'      :
    case 'c'      :
    case 'C'      :
    case KEY_F(3) :
       
        if (zoek_str.c)
          zoek((view_gety()-1),1);
        else
          zoek_tekst(0);
        break;

    case '0'      :
    case '^'      :
    case KEY_HOME :

        bv.sx=bv.x=0;
        bv.y-=(LINES-2);
        view_refresh();
        break;

    case '$'      :
    case KEY_END  :

        bv.y-=(LINES-2);

        if (!bv.mode) bv.x=utf8_strlen(bv.file[bv.y]);
        else bv.x=utf8_strlen(bv.file[bv.y+bv.sy]);

        if (bv.x<COLS) bv.x=0;
        else bv.x-=COLS;

        bv.sx=utf8_strlen(bv.file[bv.y+bv.sy])-bv.x-1;
        view_refresh();

        break;

    case 'o'      :
    case 'O'      :        

        if (bekijk_open_best(0) > 1) wexit(1);
        break;

    case KEY_RESIZE:
    case 12       :
    case 'R'      : 
    case 'r'      :

      {

        int current_y=view_gety()-1;

        /*
         * Reinit hm_win and hm_menu
         */

        wresize(hm_win,1,COLS);
        werase(hm_win);
        pl_hm[9]=COLS-strlen(txt_hm[3])-2;
        menu_print(&hm);
        touchwin(hm_win);
        wrefresh(hm_win);

        /* 
         * Reinit win2
         */

        wresize(win2,1,COLS);
        mvwin(win2,LINES-1,0);
        werase(win2);
        touchwin(win2);
        wrefresh(win2);

        /*
         * Reinit & Refresh view
         */

        wresize(win1,LINES-2,COLS);
        werase(win1);

        bv.lines=LINES-2;
        bv.cols=COLS;
        view_refresh();

        view_sety(current_y);
        view_recal_xmax();

        view_refresh();

        /*
         * refresh curscr
         */

        wrefresh(curscr);

        /*
         * reset previous COLS and LINES
         */

        prev_cols=COLS;
        prev_lines=LINES;

       }
       break;

    case 'h'      :
    case 'H'      :
    case '?'      :
    case KEY_F(1) :

       {

         char *vb;
         vb=bv.filename;
         bv.filename=xmalloc(strlen(DOCFILE)+1);
         strcpy(bv.filename,DOCFILE);

         if (view_load()) {

          xfree(bv.filename);
          bv.filename=vb;

         } else {

           view_refresh();
           if (free_vb) xfree(vb);

         }

       };  
       break;
    } /* switch(c) */
  } while ((c!='Q')&&(c!='q')&&(c!=27)&&(c!=KEY_F(10)));
} while (!exit_lookat(win1));

view_free();
touchwin(win1);
touchwin(win2);
wrefresh(win1);
wrefresh(win2);
delwin(win1);
delwin(win2);
endwin();

exit(0);

}
