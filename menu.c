/*
 *  menu.c
 *
 *  Copyright (C) 1997, 1998, 2015  Staf Wagemakers Belgie/Belgium
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

#include "menu.h"

void waddnspace(WINDOW *w,int n) {

	int i;
	for (i=0;i<n;i++) waddch(w,' ');

}

unsigned lengte(MENU *m) {
	if (m->l) return(m->l);
	return(COLS);
}

void menu_hletter(MENU *m,unsigned i,chtype hcolor) {

	if ((i>m->amount)||(m->hplace==NULL)||((m->hplace+(i<<1))==NULL)) return;

	wbkgdset(m->w,hcolor);
	mvwaddnstr(m->w,m->place[i<<1],m->place[(i<<1)+1]+m->hplace[i<<1],
        m->txt[i]+m->hplace[i<<1],m->hplace[(i<<1)+1]);

}

void menu_print_item(MENU *m,unsigned i,chtype hcolor) {

	mvwaddnstr(m->w,m->place[i<<1],m->place[(i<<1)+1],m->txt[i],lengte(m));
	if(m->l) if(strlen(m->txt[i])<m->l) waddnspace(m->w,m->l-strlen(m->txt[i]));
	menu_hletter(m,i,hcolor);

}

/* Zet de menu tekst op het scherm in de normale kleur */
void menu_print(MENU *m) {

	unsigned i;

	for (i=0;i<m->amount;i++) {

 		wbkgdset(m->w,m->color1);
 		menu_print_item(m,i,m->color3);

	}

	wrefresh(m->w);

}

void menu_sel(MENU *m,unsigned s) {

	if (s>=m->amount) s=0;
	if (m->sel<=m->amount) {

   		wbkgdset(m->w,m->color1);
   		menu_print_item(m,m->sel,m->color3);

	}

	wbkgdset(m->w,m->color2);
	menu_print_item(m,s,m->color4);
	m->sel=s;
	wrefresh(m->w);
}

int menu_key (MENU *m)
{

	int i;
	unsigned s;
	s=m->sel;
	m->used=0;
	menu_sel(m,s);
	i=wgetch(m->w);

	if (isascii(i)) i=tolower(i);

	if (m->hkey!=NULL) {

   		unsigned u;

   		for (u=0;u<m->amount;u++) {

     			if (m->hkey[u]!=0) {

       				if (i==m->hkey[u]) {

         				menu_sel(m,u);
	 				m->used=1;
	 				return(3);

	 			}

       			}
     		}
  	}

	if (i==m->key[0]){if (s==0) s=m->amount; --s;menu_sel(m,s);return(1);};
	if (i==m->key[1]){++s;menu_sel(m,s);return(2);};
	if (i==m->key[2]){m->used=1;return(3);};
	if (i==m->key[3]) return(4);
	if (i==m->key[4]) return(5);
	if (i==m->key[5]) {m->used=2;return(6);}

	return(0);
}

void menu_pull (MENU *m,unsigned submenu) {

	int i;
	unsigned s;
	MENU *m2,*m3;

	if (!submenu) m3=m;
  	else {

    		if (m->call_open!=NULL) m->call_open(m);
    		menu_print(m);
    		menu_sel(m,submenu-1);

    		if(m->next!=NULL) m3=m->next[submenu-1];
   	}

	for(;;) {

   		m2=m3;

   		if (m2->call_open!=NULL) m2->call_open(m2);

   		menu_print(m2);

   		do i=menu_key(m2); while(i<3);

   		if ((m2->call_close!=NULL)&&(m2!=m)) m2->call_close(m2);
   		if (i==6) {m->used=2;break;}

   		if (m2!=m) {

      			switch (i) {

         			case 4: m3=m2->parent;m2=m3;s=m2->sel;if (s==0) s=m2->amount;
                 			--s;menu_sel(m2,s);
		 			break;

         			case 5: m3=m2->parent;m2=m3;s=m2->sel+1;
                 			menu_sel(m2,s);
		 			break;

         		}
      		}

   		if(m2->next!=NULL) {

   			m3=m2->next[m2->sel];
 			if (m2->next[m2->sel]==NULL) break;

   		}
 	}

   	if (m->call_close!=NULL) m->call_close(m);
}

/*
 * scroll_menu
 * MENU *m : menu para's
 *      m->key[0] : ga naar vorig item
 *      m->key[1] : ga naar volgend item
 *      m->key[2] : selektie van een item
 *      m->key[3] : ga naar vorig scherm
 *      m->key[4] : ga naar volgend scherm
 *      m->key[5] : break
 *	m->key[6] : ekstra
 *
 *      De andere para's zijn hetzelfde als bij
 *      normaal menu gebruik ...
 *
 *      m->parent, m->next, ... worden niet
 *      gebruikt!
 *
 *unsigned g    : het echte aantal item
 *unsigned y,x,l: pl. + maks. lengte van waar
 *                het huidige item ekstra
 *		            getoond moet worden
 *	              Indien l=0 wordt het sel.
 *                niet ekstra getoond
 *
 *unsigned *u   : voorselektie + hulpvar.
 *
 *Geeft terug 0 : selektie
 *		  m->sel is het gesel. item
 *            1 : break-toets ingedrukt
 *            2 : ekstra-toets ingedrukt
 */
int scroll_menu(MENU *m,unsigned g,void (*call_proc)(MENU *m),unsigned *u) {

	int c;
	unsigned teller,s,v_s;
	char **cp,**v_txt;
	cp=m->txt;
	m->txt=cp;
	v_s=*u+1;
	v_txt=NULL;
	teller=m->sel;
	s=m->sel=*u;

	do {

  		do {
  			*u=s;

  			if (m->txt!=v_txt) { menu_print(m); menu_sel(m,s);}
			else if (s!=v_s) menu_sel(m,s);

  			v_s=s;
  			v_txt=m->txt;

  			if (call_proc) call_proc(m);

  			c=wgetch(m->w);

  			if (c==m->key[0]) {

    				if (s) {--s;--teller;}
      				else if(teller) {
         				--teller;
	 				--m->txt;
	 			}
    				break;
    			}

    			if (c==m->key[1])  {

        			++s;

        			if (s>m->amount-1) {

          				s=m->amount-1;
	  				if (teller<g-1) {

            					++teller;
            					++m->txt;

	    				}
	  			}
        			else ++teller;

       				break;
       			}

    			if (c==m->key[2])  break;

    			if (c==m->key[3])  {

      				if (teller-s>m->amount) {

        				teller-=(m->amount-1);
        				m->txt=&cp[teller-s];

        			}
        			else {

            				teller=s=0;
	    				m->txt=cp;

	    			}

      				break;
      			}

    			if (c==m->key[4]) {

      				if (teller-s+2*m->amount<g-1) {

        				teller+=m->amount;
					m->txt=&cp[teller-s];

				}
				else {

	     				teller=g-1;
	     				s=m->amount-1;
	     				m->txt=cp+(g-1)-(m->amount-1);
	
	     			};

     				break;
     			}

    			if (c==m->key[5]) {

       				m->sel=teller;
       				return(1);
       				break;

       			}
	
    			if (c==m->key[6]) {

       				m->sel=teller;
       				return(2);
       				break;

       			}

    			if (c==m->key[7]) {

       				m->sel=teller;
       				return(3);
       				break;

       			}

  		} while(1);

  	} while (c!=m->key[2]);

  m->sel=teller;
  return(0);
}

