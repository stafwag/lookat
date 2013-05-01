/*
 *  win.c
 *
 *  Copyright (C) 1997, 2000, 2001, 2006, 2007  Staf Wagemakers Belgie/Belgium
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

#include "win.h"
void wexit(int i) {
	endwin();
	exit(i);
}
/* ----------------------------------------- */
/* Print een kar. als een string af, dit     */
/* geeft minder problemen met speciale kar.  */
/* ----------------------------------------- */
void waddch_fix (WINDOW *w,char c)
{
char s[2]={'\0','\0'};
if (isbin(c)) c='.';
s[0]=c;
waddstr(w,s);
}
/* ------------------------------------------- */
/* Tekenen van horizontale lijn		       */
/* ------------------------------------------- */
void win_hline(WINDOW *w,int h)
{
int i;
for (i=0;i<h;i++) waddch(w,ACS_HLINE);
}
/* ------------------------------------------- */
/* Tekenen van een vertikale lijn	       */
/* ------------------------------------------- */
void win_vline(WINDOW *w,int v)
{
int i;
for (i=0;i<v;i++) waddch(w,ACS_VLINE);
}
/* ----------------------------------------- */
/* Tekenen van een kader in een venster	...  */
/*					     */
/* w     : venster			     */
/* y,x   : horizontale & vertikale grootte   */
/* yp,xp : horizontale & vertikale positie   */
/* ----------------------------------------- */
void win_box(WINDOW *w,int y,int x,int yp,int xp)
{
mvwaddch (w,yp,xp,ACS_ULCORNER);
win_hline(w,x-1);
mvwaddch(w,yp,xp+x,ACS_URCORNER);
wmove(w,yp+1,xp+x);
wvline(w,ACS_VLINE,y-1);
wmove(w,yp+1,xp);
wvline(w,ACS_VLINE,y-1);
mvwaddch(w,yp+y,xp,ACS_LLCORNER);
whline(w,ACS_HLINE,x-1);
mvwaddch(w,yp+y,xp+x,ACS_LRCORNER);
}
/* -------------------------------------------- */
/* Procedure voor het openen van een venster +	*/
/* fout afhandeling				*/
/* -------------------------------------------- */
WINDOW * open_win(int yy,int xx,int y,int x)
{
WINDOW *w;
if (!(w=newwin(yy,xx,y,x))) { 
   endwin();
   fprintf(stderr,"Failed to create ncurses windows");
   exit(1);
   }
return(w);
}
/* -------------------------------------------- */
/* Procedure voor het openen van een gecenteerd */
/* venster					*/
/* int yy,xx  : lengte, breedte			*/
/*						*/
/* Geeft een pointer naar venster terug		*/
/* -------------------------------------------- */
WINDOW * open_cwin (int yy,int xx)
{
WINDOW *w;
w=open_win(yy,xx,LINES/2-yy/2,COLS/2-xx/2);
return(w);
}
/* -------------------------------------------- */
/* Procedure voor het openen van een		*/
/* "ok"-venster					*/
/* int yy,xx  : lengte, breedte			*/
/* char **txt : pointer naar array van strings  */
/*		NULL = einde array		*/
/* chtype   c : normale kleur			*/
/* -------------------------------------------- */
void open_okwin (int yy,int xx,MENU *m,char **txt,WINDOW *win1)
{
WINDOW *w;
int i;
unsigned pl_ok[2];
int key_m[]={'\n','\n','\n','\n','\n','\n'};
w=open_cwin(yy,xx);
leaveok(w,TRUE);
curs_set(0);
pl_ok[0]=yy-2;
pl_ok[1]=xx/2-strlen(m->txt[0])/2;
m->w=w;
m->amount=1;
m->l=0;
m->place=pl_ok;
m->sel=m->used=0;
m->key=key_m;
wbkgdset(w,m->color1);
werase(w);
i=0;
while (txt[i]!=NULL) mvwaddstr(w,2+i,3,txt[i++]); 
box(w,0,0);
do i=menu_key(m); while(!i);
wrefresh(w);
delwin(w);
if (win1!=NULL) touchwin(win1);
}
/* -------------------------------------------- */
/* Procedure het open van een "ja/nee"-venster  */
/* int yy,xx  : lengte, breedte			*/
/* char **txt : pointer naar array van strings  */
/*		NULL = einde array		*/
/* chtype   c : normale kleur			*/
/*						*/
/* Geeft terug 0 : -> nee / ESC			*/
/*	       1 : -> ja			*/
/* -------------------------------------------- */
int open_ynwin (int yy,int xx,MENU *m,char **txt,WINDOW *win1)
{
#define TAB 9
WINDOW *w;
int s,i;
unsigned pl_jn[4];
int key_m[]={KEY_LEFT,KEY_RIGHT,'\n',TAB,'\n',27};
w=open_cwin(yy,xx);
leaveok(w,TRUE);
curs_set(0);
keypad(w,TRUE);
nodelay(w,TRUE);
pl_jn[0]=pl_jn[2]=yy-2;
pl_jn[1]=xx/2-7-strlen(m->txt[0])/2;
pl_jn[3]=xx/2+7-strlen(m->txt[1])/2;
m->w=w;
m->amount=2;
m->l=0;
m->place=pl_jn;
m->sel=m->used=0;
m->key=key_m;
wbkgdset(w,m->color1);
werase(w);
i=0;
while (txt[i]!=NULL) mvwaddstr(w,2+i,3,txt[i++]);
box(w,0,0);
menu_print(m);
s=0;
do {
 menu_sel(m,s);
 do i=menu_key(m); while(i<3);
 if (++s>1) s=0;
 } while (i==4);
wrefresh(w); 
delwin(w);
if (win1!=NULL) touchwin(win1);
if (i==6)  return(0);
if (m->sel) return(0);
return(1);
}
/* -------------------------------------------- */
/* Procedure voor het openen van een		*/
/* anim-venster					*/
/* int yy,xx  : lengte, breedte			*/
/* char **txt : pointer naar array van strings  */
/*		NULL = einde array		*/
/* chtype   c : normale kleur			*/
/* char   mode: 0 -> stop                       */
/*              1 -> don't stop                 */
/* -------------------------------------------- */
void open_animwin (int yy,int xx,MENU *m,char **txt,char *animtxt,WINDOW *win1,char mode)
{
WINDOW *w;
int i;
unsigned pl_ok[2];
int key_m[]={'\n','\n','\n','\n','\n','\n'};
int xp,yp,t;
fd_set set;
struct timeval timeout;
w=open_cwin(yy,xx);
leaveok(w,TRUE);
curs_set(0);
nodelay(w,TRUE);
pl_ok[0]=yy-2;
pl_ok[1]=xx/2-strlen(m->txt[0])/2;
m->w=w;
m->amount=1;
m->l=0;
m->place=pl_ok;
m->sel=m->used=0;
m->key=key_m;
wbkgdset(w,m->color1);
werase(w);
i=0;
while (txt[i]!=NULL) mvwaddstr(w,2+i,3,txt[i++]); 
getyx(w,yp,xp);
box(w,0,0);
do {
   i=menu_key(m);
   wbkgdset(w,m->color1);
   for (t=0;t<strlen(animtxt);t++) {
       for (xp=getmaxx(w)-4;xp>(2+t);xp--) {
	   mvwaddch(w,yp,xp,animtxt[t]);
           mvwaddch(w,yp,xp+1,' '); 
           wrefresh(w);
	 {
	/* Initialize the file descriptor set. */
	FD_ZERO (&set);
	FD_SET (STDIN_FILENO,&set);
	/* Initialize the timeout data structure */
	timeout.tv_sec=0;
	timeout.tv_usec=2;
	select (FD_SETSIZE,&set,NULL,NULL,&timeout);
	 }
       }
       }
       timeout.tv_sec=3;
       select (FD_SETSIZE,&set,NULL,NULL,&timeout);
    if (!mode) break;
    for (xp=0;xp<strlen(animtxt);xp++) mvwaddch(w,yp,xp+3,' '); 
    wrefresh(w);
    i=menu_key(m);
} while(!i);
nodelay(w,FALSE);
if(!mode) {do {i=menu_key(m);} while(!i);}
	
wrefresh(w);
delwin(w);
if (win1!=NULL) touchwin(win1);
}
/* ------------------------------------------- */
/* Procedure voor het openen van menu venster  */
/* ------------------------------------------- */	
void m_open(MENU *m,WINDOW *win1)
{
touchwin(win1);
leaveok(m->w,TRUE);
curs_set(0);
keypad(m->w,TRUE);
nodelay(m->w,FALSE); 
wbkgdset (m->w,m->color1);
werase(m->w);
box(m->w,0,0);
wrefresh(m->w);
}
void print_item(MENU *m)
{
INPUT_STRING str;
str.w=m->w;
str.xp=22;
str.yp=3;
str.c=m->txt[m->sel];
str.count=0;
str.ox=0;
str.n=45;
str.m=0;
str.mode=0;
wbkgdset(m->w,m->color3);
input_show_string(&str);
}
/* ----------------------------------------- */
/* Inlezen van de bestandsnaam ...	     */	
/* *win2 : info venster indien NULL geen info */
/* *txt_info_open : info tekst                */
/* **txt      [0] : tekst in info venster    */
/*            [1] : geen toegang             */
/*            [2] : kan bestand nt openen    */
/*            [3] : huidige dir              */
/*            [4] : selektie		     */
/*            [5] : titel venster            */
/*            [6] : bestanden                */
/*            [7] : dirs                     */
/*            [8] : ok			     */
/* *kleur     [0] : win kleur                */
/*            [1] : menu color		     */
/*            [2] : selektie kleur           */
/*            [3] : balkkleur                */
/* *win1          : te verversen window      */
/*                                           */
/* Geeft terug :                             */
/* bestandsnaam                              */
/* NULL -> fout                              */
/* ----------------------------------------- */
char * open_filewin(WINDOW *win2,char **txt,chtype *kleur,WINDOW *win1,int (*ends)())
{
MENU dm,bm,fm;			/* menus 		*/
WINDOW *w;			/* my windows 		*/
INPUT_STRING str;		/* file name  		*/
int 	 toetsen[]={27,9,0};
unsigned bu=0,du=0;
bool   	 form_input=FALSE;
char     *bestandsnaam,*txt_path,*txt_wd,*vb;
char     txt_slash[]={'/',0};
unsigned pl_bm[]={6,3,7,3,8,3,9,3,10,3,11,3,12,3,13,3,14,3,15,3,16,3,17,3,18,3};
unsigned pl_dm[]={6,36,7,36,8,36,9,36,10,36,11,36,12,36,13,36,14,36,15,36,16,36,17,36,18,36,19,36};
struct 	 dir_data *data;
unsigned brol;
int 	 i,sub;
int 	 key_m[]={KEY_UP,KEY_DOWN,'\n',KEY_PPAGE,KEY_NPAGE,27,9,'q'};
struct 	 stat stat_buf;
char 	 *s;
char 	 *txt_f[]={txt[2],NULL};
char 	 txt_ret[]=" ";
char 	 *txt_f_ok[]={txt[8],txt_ret,txt_ret,NULL};

bestandsnaam=txt_path=txt_wd=vb=NULL;

fm.txt=txt_f_ok;
fm.color1=kleur[1];
fm.color2=kleur[3];
fm.hkey=NULL;
fm.hplace=NULL;
str.yp=3;
str.xp=22;
str.c=NULL;
str.key=toetsen;
str.n=45;
str.m=0;
str.mode=0;
str.insert=TRUE;
str.ox=str.count=0;

if (win2!=NULL) { 
	werase(win2);
        mvwaddstr(win2,0,0,txt[0]);
        wrefresh(win2);
}

vb=bestandsnaam;
txt_path=NULL;
txt_path=NULL;
txt_wd=NULL;

/*
 *  menu init
 */

bm.color1=kleur[2];
bm.color2=kleur[3];
bm.place=pl_bm;
bm.key=key_m;
bm.l=dm.l=30;
dm.color1=kleur[2];
dm.color2=kleur[3];
bm.color3=dm.color3=kleur[1];
dm.place=pl_dm;
dm.key=key_m;
bm.hkey=NULL;
bm.hplace=NULL;
dm.hkey=NULL;
dm.hplace=NULL;

brol=0;
w=open_cwin(21,70);
str.w=w;
leaveok(w,TRUE);
curs_set(0);
keypad(w,TRUE);
nodelay(w,FALSE); 
wbkgdset(w,kleur[0]);
sub=0;

do {

  int fout;
  if (txt_path!=NULL) {
       wrefresh(win2);
       chdir(txt_path);
       xfree(txt_path);
       txt_path=NULL;
     }
  
  txt_path=(char *) xmalloc(2);
  strcpy(txt_path,".");

  do {
  
   int size=100;
   txt_wd=(char *) xrealloc(txt_wd,size); 
   fout=0;
   
   if (getcwd(txt_wd,size)==NULL) { 
      fout=1;
      
      if ((errno=EACCES)) {
         txt_wd=(char *) xrealloc(txt_wd,strlen(txt[1])+1);
         strcpy(txt_wd,txt[1]);
         ++fout;
      };
      
      break;
   };

   size*=2;

  } while (fout); 

  bu=du=bm.sel=dm.sel=0;
  txt_path=xrealloc(txt_path,strlen(txt_path)+strlen(txt_slash)+1);
  strcat(txt_path,txt_slash);

  if (fout==2) data=NULL;
    else data=get_dir(txt_path);

  if (data==NULL) {
     data=(struct dir_data*) xmalloc(sizeof(struct dir_data));
     data->files=0;
     data->dirs=0;
     data->file=NULL;
     data->dir=(char **) xmalloc(sizeof(char **));
     data->dir[0]=(char *) xmalloc(strlen("/")+1);
     strcpy(data->dir[0],"/");
     txt_wd=(char *) xrealloc(txt_wd,strlen(txt[1])+1);
     strcpy(txt_wd,txt[1]);
     };

  wbkgdset(w,kleur[0]);
  werase(w);
  wattrset(w,kleur[0]);
  win_box(w,14,32,5,2); 
  win_box(w,14,32,5,35);
  box(w,0,0); 
  wattroff(w,kleur[0]); 
  mvwaddstr(w,2,1,txt[3]);
  mvwaddstr(w,3,1,txt[4]);
  wbkgdset(w,kleur[3]);
  wattron(w,kleur[3]);
  wmove(w,3,22);
  whline(w,' ',45);
  wattroff(w,kleur[3]);
  wbkgdset(w,kleur[0]); 
  mvwaddnstr(w,2,23,txt_wd,44);
  mvwaddstr(w,0,2,txt[5]);
  mvwaddstr(w,5,4,txt[6]);
  mvwaddstr(w,5,37,txt[7]);
  wrefresh(w);

  if (data->files<13) bm.amount=data->files;
    else bm.amount=13;
  if (data->dirs<13)  dm.amount=data->dirs;
    else dm.amount=13;

  bm.txt=data->file;
  dm.txt=data->dir;
  bm.w=dm.w=w;

  do {
    form_input=FALSE;
    if (sub>2) sub=0;
    switch (sub) {

       case 0: if (bm.amount) {
                  menu_print(&dm);
    	          i=scroll_menu(&bm,data->files,print_item,&bu);
               }
	       else i=2;
	       break;

        case 1: if (dm.amount) {
                   menu_print(&bm);
                };
	        i=scroll_menu(&dm,data->dirs,NULL,&du);
	        break;

        case 2: menu_print(&dm);
                xfree(str.c);

                if (data->files) {
	             str.c=(char *) xmalloc(strlen(data->file[bm.sel])+1);
                     strcpy(str.c,data->file[bm.sel]); 
	        }
	        else str.c=NULL;
	    
		wbkgdset(w,kleur[1]);
            	str.ox=str.count=0;

	    	switch (input(&str)) {
	    
			case 0: form_input=TRUE;
                    		if (stat(str.c,&stat_buf)==-1) {
		    		   int l=0;
		    		   i=0;

		    		   while(txt_f[i]!=NULL) { 
                         	      if (strlen(txt_f[i])>l) 
					     l=strlen(txt_f[i]);
			 		  i++;
			 	   };

		    		   open_okwin(i+5,l+5,&fm,txt_f,w);
		    		   i=2;
		    		   sub=1;
		                 }
		    	         else { 
                      		    if (S_ISDIR(stat_buf.st_mode)) sub=1;
			 	       else sub=0;
	 	      		    i=0;
		                 }
		    	         break;
	    		 case 1: i=1;
	            		 break;
	    		 case 2: break;
	     };
	    leaveok(w,TRUE);
	    curs_set(0);
	    break;
    };
    
    if ((i==1)||(i==3)) {
	if(ends==NULL) break;
	if(ends(w)) break;
    } 
 
    if (i==2) ++sub;
   
  } while (i);

  if (i) break;

  if (sub) { 
    if (form_input) s=str.c;
       else s=data->dir[dm.sel];
    txt_path=(char *) xrealloc(txt_path,strlen(s)+1);
    strcpy(txt_path,s);
    get_dir_free(data);
    data=NULL;
    };
} while(sub);

if (!i) {
  if (form_input) s=str.c;
     else s=data->file[bm.sel];
  bestandsnaam=(char *) xmalloc(strlen(s)+1);
  strcpy(bestandsnaam,s);
};

if (txt_path!=NULL) xfree(txt_path);

xfree(str.c);

if (data!=NULL) get_dir_free(data);

xfree(txt_wd); 
delwin(w);

if (win1!=NULL) wrefresh(win1);

if (i) return (NULL);

return (bestandsnaam);
}
/* ----------------------------------------- */
/* Openen van een input venster              */
/*					     */
/* INPUT_STRING *s = pointer naar string     */
/* char *titel = pointer naar venster-titel  */
/* char *tekst = pointer naar info-tekst     */
/* int    mode = input-mode                  */
/*                   0 = string              */
/*                   1 = dec.                */
/*                   2 = hex                 */
/* Geeft terug 1 -> ok                       */
/*             0 -> nok                      */
/* ----------------------------------------- */
int open_inputwin (int yy,int xx,MENU *m,chtype kleur,INPUT_STRING *read_str,char **txt,char mode,WINDOW *win1)
{
WINDOW *w;
int brol;
int toetsen[]={27,9,0};
unsigned pl_ok[]={5,15,5,35};
int key_m[]={0,TAB,' ','\n','\n','\n'};
w=open_cwin(7,60);
read_str->w=w;
read_str->key=toetsen;
read_str->c=NULL;
read_str->yp=2;
read_str->xp=21;
read_str->ox=0;
read_str->insert=1;
read_str->mode=mode;
read_str->n=32;
read_str->m=0;
read_str->count=0;
m->w=w;
m->amount=2;
m->l=0;
m->place=pl_ok;
m->sel=m->used=0;
m->key=key_m;
wbkgdset(w,m->color1);
werase(w);
box(w,0,0);
wbkgdset(w,m->color1);
echo();
leaveok(w,FALSE);
curs_set(0);
mvwaddstr(w,0,2,txt[0]);
mvwaddstr(w,2,2,txt[1]);
wmove (w,2,21);
wbkgdset(w,kleur);
for (brol=0;brol<=32;brol++) waddch(w,' ');
do {
  menu_print(m);
  m->sel=0;
  wbkgdset(w,kleur);
  brol=input(read_str);
  leaveok(w,TRUE);
  curs_set(0);
  if (!brol)   return(0);
  if (brol!=2) return(1);
  do { 
    brol=menu_key(m);
    if ((brol==2)&&(m->sel==0)) break;
   } while(brol<3);
} while(brol<3);
delwin(w);
if (win1!=NULL) wrefresh(win1);
if (!m->sel) return(0);
return (1);
}


