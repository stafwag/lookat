/*
 *  input.c
 *
 *  Copyright (C) 1997, 1998, 2000, 2006  Staf Wagemakers Belgie/Belgium
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

/* ------------------------------------------------------------------------- */
/* Funkties voor de input(....) procedure			             */
/* ------------------------------------------------------------------------- */
#include "input.h"
/* ----------------------------------------- */
/* Bijvoegen van een karakter in een string  */
/* char     *c -> string                     */
/* unsigned i  -> positie in string          */
/* char     cc -> karakter		     */
/* ----------------------------------------- */
void ins_char (char *c,unsigned i,char cc)
{
unsigned x;
unsigned einde=strlen(c);
if (i!=einde) {
  if (i) for (x=einde;x>=i;x--) c[x]=c[x-1]; 
    else { for  (x=einde-1;x>0;x--) c[x+1]=c[x];c[1]=c[0];
    }
	
  }
c[i]=cc;
c[einde+1]=0;
}
/* ----------------------------------------- */
/* Verwijderen v/e karakter in een string    */
/* char     *c -> string                     */
/* unsigned i  -> positie in string          */
/*					     */
/* P.S. Deze funktie schuift de kar's ook    */
/* naar rechts!			             */
/* ----------------------------------------- */
void del_char (char *c,unsigned i)
{
unsigned x;
for (x=i;x<strlen(c);x++) c[x]=c[x+1];
}
/* ----------------------------------------- */
/* wissen van het huidige scherm entry       */
/* ----------------------------------------- */
void wis (WINDOW *w,int yp,int xp,int n)
{
int i;
for (i=0;i<n;i++) mvwaddch(w,yp,xp+i,' ');
}
/* ----------------------------------------- */
/* Aanpassen van van ox (xoffset)            */
/* ----------------------------------------- */
void xoffset (INPUT_STRING *p)
{
if ((p->count-p->ox)>p->n) p->ox+=5;
   else if (((p->count-p->ox)<=0)&(p->count!=0)) p->ox-=5;
}
/* ----------------------------------------- */
/* Overfow afhandeling                       */
/* ----------------------------------------- */
void overflow(INPUT_STRING *p)
{
if (p->count > p->n) p->ox=((p->count-p->n)/5)*5;
}
/* ----------------------------------------- */
/* Toont de string op het scherm             */
/* ----------------------------------------- */
void input_show_string (INPUT_STRING *p)
{
char *s=p->c;
xoffset(p);
wis(p->w,p->yp,p->xp,p->n);
if (p->mode > 2) {
   int  i;
   s=(char *) xmalloc(strlen(p->c)+1);
   for(i=0;i < strlen(p->c);i++) s[i]='*';
   s[i]=0;
   }
mvwaddnstr(p->w,p->yp,p->xp,s+p->ox,p->n);
wmove(p->w,p->yp,p->xp+p->count-p->ox);
wrefresh(p->w);
if (p->mode > 2) xfree(s);
}
/* --------------------------------------------- */
/* De eigelijke input routine ...                */
/*					         */
/* WINDOW   p->w   = input venster               */
/* unsigned p->yp  = y-positie in venster        */
/* unsigned p->xp  = x-positie in venster        */
/* char     p->c   = pointer naar de string      */
/* unsigned p->n   = aantal kar's op scherm      */
/*          p->m   = max. toegelaten kar's	 */
/*                   Indien 0 = oneindig         */
/*                     niet 0 = er voor p->c     */
/*				voldoende geh.   */
/*				gealloc. zijn!   */
/*						 */
/*          p->ox  = offset string               */
/* char     mode   = 0 -> normale string         */
/*                   1 -> dec. string            */
/*                   2 -> hex. string            */
/*		     3 -> passwd string		 */
/* int      p->key = pointer naar ekstra toetsen */ 
/* --------------------------------------------- */
int input (INPUT_STRING *p)
{
int i,brol;
unsigned einde=0;
if (p->c==NULL) { 
   p->c=(char *) xmalloc(sizeof(char));
   p->c[0]='\0';
   }
if (p->insert) mvwaddstr(p->w,p->w->_maxy,p->w->_maxx-2,"I");
   else mvwaddstr(p->w,p->w->_maxy,p->w->_maxx-2,"O");
noecho();
leaveok(p->w,FALSE); 
curs_set(1);
keypad(p->w,TRUE);
meta(p->w,TRUE);
wmove(p->w,p->yp,p->xp);
if (strlen(p->c)!=0) {
  einde=strlen(p->c);
  if (p->count>strlen(p->c)) p->count=einde;
  overflow(p);
  input_show_string(p);
  }
  else einde=p->count=0;
do {
    unsigned u=0;
    i=wgetch(p->w);
    while (p->key[u]!=0) {
     if (i==p->key[u++]) return (u);
    };
    switch (i) {
    case KEY_HOME:      p->count=0;
                        p->ox=0;
                        break;
    case KEY_END:	p->count=einde;
                        overflow(p);
			break;
    case KEY_IC:	if (p->insert) {
                          p->insert=FALSE;
			  mvwaddstr(p->w,p->w->_maxy,p->w->_maxx-2,"O");
			  }
                          else {
                          p->insert=TRUE;
			  mvwaddstr(p->w,p->w->_maxy,p->w->_maxx-2,"I");
			  };
			wrefresh(p->w);  
			i=0;
			break;
    case KEY_DC:	if (p->count==einde) break;
			++p->count;
    case KEY_BACKSPACE: if (einde) { 
                           wis(p->w,p->yp,p->xp,p->n);   
			   };
			if (p->count) --p->count;
                        p->c[einde]=0;
			del_char(p->c,p->count);
			if (einde) einde--;
			p->c[einde]=0;
			break;
    case KEY_LEFT:      if (p->count) --p->count;			
                        break;
    case KEY_RIGHT:     if (p->count<einde) ++p->count;
			break;
    }
    if (p->m) { if (einde>p->m) einde=p->m;}
       else { p->c=(char *) xrealloc(p->c,strlen(p->c)+2); }
    if (!p->c) { endwin();fprintf(stderr,"realloc failed...\n"); exit(1);} 
    switch (p->mode) {
      case 0 : brol=isprint(i);
               if (i==' ') brol=TRUE;
	       break;
      case 1 : brol=isdigit(i);
	       break;
      case 2 : brol=isdigit(i);
	       break;
      default: brol=TRUE;
      }
     if (isprint(i)) {
        if (brol) {
           if ((p->count < p->m)||(!p->m)) { 
	      if (p->insert) ins_char(p->c,p->count++,i);
	         else p->c[p->count++]=i;
	      einde=strlen(p->c);
	      if (p->m) if (einde>p->m) einde=p->m;
	      if (p->count>einde) einde=p->count;
	      p->c[einde]=0;
	   }
        }
      }
   input_show_string(p);
   } while (i!='\n');
p->c[einde]=0;
p->c=(char *)xrealloc(p->c,strlen(p->c)+1);
wrefresh(p->w);
return (0);
}


