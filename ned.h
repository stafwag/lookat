/*
 *  ned.h
 *
 *  Copyright (C) 1997, 1998, 2000, 2001, 2002, 2003, 2004, 2006  Staf Wagemakers Belgium
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

const int sel_bm		='b';
const int sel_gm		='g';
const int sel_zm		='z';
const int sel_om                ='o';
const int sel_hm		='h';
char  *txt_help[] = {
"---------------------------------------------------------------------------",
"bekijk 1.4.3rc1                             (GPL) 1997-2012 Belgie  Belgium",
"                                                            Staf Wagemakers",
"                                                  email: staf@wagemakers.be",
" ",
"Je kan 'bekijk' op de volgende manieren starten:",
" "
"- door gewoon 'bekijk' te typen, hierna kan je uit het bestandsmenu",
"  een bestand selekteren om te bekijken.",
"- door 'bekijk <dir_naam>' te typen, de huidige direktorie wordt",
"  verandert in <dir_naam>.",
"- door 'bekijk <bestandsnaam>' te typen, hierdoor wordt bij het",
"  opstarten het bestand automatisch geladen.",
"- lezen van stdin",
"  voorbeeld:  bekijk < test.txt",
"              ls | bekijk",
"- om man paginas te bekijken: man -Pbekijk ls",
" ",
"Bedankt voor het gebruik van dit programma!",
"--------------------------------------------------------------------------\n",NULL};
int hkey_ok[]		={'o'};
int hkey_jn[]		={'j','n'};
int hkey_typem[]	={'b','i','v','w','g'};
int hkey_cv_color[]     ={'f','b','a','o','t','c'};
int hkey_cv_mono[]      ={'f','b','o','t','c'};
int hkey_colorm[]	={'v','o','a',0,0};
int hkey_ok_anu[]	={'o','a'};
int hkey_zoek[]		={0,'o','a'};
int hkey_type[]		={'o','a'};
int hkey_bm[]		={'o','v','a'};
int hkey_gm[]		={'b','e','l',KEY_END,KEY_HOME};
int hkey_zm[]		={'t','b','n'};
int hkey_om[]           ={0,'v','t','k','b'};
int hkey_hlp[]		={'z','t','o'};
unsigned hplace_bm[]		={0,1,0,1,0,1};
unsigned hplace_gm[]		={5,1,5,1,5,1,17,5,17,6};
unsigned hplace_zm[]		={0,1,6,1,0,1};
unsigned hplace_om[]         	={3,1,3,1,0,1,0,1};
unsigned hplace_hlpm[]		={0,1,0,1,0,1};
unsigned hplace_hm[]		={0,1,0,1,0,1,0,1,0,1};
unsigned hplace_ok[]		={2,1};
unsigned hplace_jn[]		={3,1,3,1};
unsigned hplace_typem[]		={2,1,2,1,2,1,2,1,2,1,0,0};
unsigned hplace_cv_color[]   	={0,1,0,1,0,1,4,1,2,1,2,1};
unsigned hplace_cv_mono[]   	={0,1,0,1,4,1,2,1,2,1};
unsigned hplace_colorm[]	={2,1,5,1,2,1,2,1,2,1,0,0,0,0};
unsigned hplace_ok_anu[]	={4,1,2,1};
unsigned hplace_zoek[]		={0,0,0,0,4,1,2,1};
unsigned hplace_type[]		={4,1,2,1};
char txt_term[]         ="Programma beeindigd door signaal ";
char txt_info_open[]	="ENTER = selektie       TAB = bestanden <-> direktories      ESC = anuleer";
char *txt_f_writecfg[]	={"Sorry, kan configuratie bestand","niet wegschrijven.",NULL};
char txt_f_open1[]	="Sorry, kan";
char txt_f_open2[]	="niet openen\n";
char txt_f_laden[]	="Kan bestand of directorie niet openen ...";
char txt_f_fseek[]	="Sorry, er is een fout opgetreden bij fseek()";
char txt_f_toegang[]	="Toegang geweigerd ...";
char txt_f_ok[]		="[ Ok ]";
char txt_f_initscr[]	="Sorry, er is een fout opgetreden bij initscr() ...";
char txt_h_dir[]	=" Huidige direktorie: ";
char txt_sel[]		=" Selektie          : ";
char txt_lezen_bestn[]	=" Inlezen van de bestandsnaam ... ";
char txt_bestanden[]	=" Bestanden ";
char txt_dirs[]		=" Direktories ";
char txt_t_links[]      ="C-b        : links";
char txt_t_rechts[]     ="C-f        : rechts";
char txt_t_omhoog[]     ="C-p        : omhoog";
char txt_t_omlaag[]     ="C-n        : omlaag";
char txt_t_u[]          ="u          : PgUp";
char txt_t_d[]          ="d          : PgDwn";
char txt_t_o[]		="o          : openen van een bestand";
char txt_t_zsf[]	="/,z,s,f    : zoek tekst";
char txt_t_F3[]		="F3,n,c,v,\\ : zoek volgende";
char txt_t_HOME[]	="HOME,0,^   : ga naar begin van de regel";
char txt_t_END[]	="END        : ga naar einde van de regel";
char txt_t_b[]		="t,<        : ga naar begin van het bestand";
char txt_t_e[]		="b,>        : ga naar einde van het bestand";
char txt_t_F9[]		="F9         : menu";
char txt_t_help[]	="h,?,F1     : zie helptekst";
char txt_t_QUIT[]	="Q,ESC,F10  : stop ... einde ... :-(";
char txt_t_rebuild[]	="r,C-l      : bouw het schrem opnieuw op";
char *txt_t[]={txt_t_links,txt_t_rechts,txt_t_omhoog,txt_t_omlaag,txt_t_u,txt_t_d,txt_t_o,txt_t_zsf,txt_t_F3,txt_t_HOME,txt_t_END,txt_t_b,txt_t_e,txt_t_F9,txt_t_help,txt_t_QUIT,txt_t_rebuild,NULL};

char *txt_ok[]		={"[ OK ]"};
char *txt_ok_anu[]	={"[   OK   ]","[ Anuleer ]"};
char txt_X[]		="X";
char txt_spacie[]	=" ";
char *txt_m_zoek[]	={txt_spacie,txt_spacie,"[   OK   ]","[ Anuleer ]"};
char *txt_m_type[]	={"[   OK   ]","[ Anuleer ]"};
char *txt_cm_color[]          ={"Voorgrond  ",
		          "Achtergrond",
			  "Attribuut  ",
			  "[   OK   ]","[ Probeer ]","[ Anuleer ]",NULL};
char *txt_cm_mono[]     ={"Voorgrond  ",
		          "Achtergrond",
			  "[   OK   ]","[ Probeer ]","[ Anuleer ]",NULL};
char *txt_kleuren[]     = {"zwart","rood","groen","geel","blauw",
	                           "magenta","cyan", "wit",NULL};
char *txt_attributen[] = {"normaal","dik","onderlijnd","reverse",
	                          "standout",NULL};

char txt_ondersheid_g[]	="[ ] grootte & kleine letters  ";
char txt_search_back[]  ="[ ] Achteruit zoeken";
char *txt_over[]	={
"Bekijk versie 1.4.3rc1             GNU Public License",
"Staf Wagemakers                    (1997-2013) Belgie","\n",
"email:                             staf@wagemakers.be",
"homepagina:                  http://www.wagemakers.be","\n",NULL};
char txt_bedank[]       ="Bedankt voor het gebruik van dit programma!";
char *txt_verder_zoeken_begin[]={"Begin van het document bereikt, van 't einde verder zoeken?",NULL};
char *txt_verder_zoeken_einde[]={"Einde van het document bereikt, van 't begin verder zoeken?",NULL};
char *txt_t_nt_gevonden[]={"Tekst niet gevonden ...",NULL};
char txt_z_v_tekst[]	=" Zoeken van een tekst ... ";
char txt_c_type[]       =" Wijzigen van een type ... ";
char txt_n_type[]	=" Nieuwe type ... ";
char txt_geef_de_tekst[]="Geef de tekst     : ";
char txt_typename[]     ="Type naam   :";
char txt_typeval[]      ="Type waarde :";
char txt_ga_lijn[]	=" Ga naar een lijn ... ";
char txt_geef_lijnnr[]	="Geef de lijnnummer : ";
char txt_lijn[]		="lijn";
char txt_kol[]		="kol.";
char txt_Grootte[]	="Grootte";
char txt_maak_sel[]	="Maak een selektie ...";
char *txt_stop[]	={"Wil je \"bekijk\" verlaten?",NULL};
char *txt_jn[]		={"[  Ja  ]","[  Nee  ]"};
char *txt_typem[]	={"[ Bijvoegen ]",
			  "[ Invoegen ]",
                          "[ Verander ]",
                          "[ Wis ]",
			  "[ Gedaan ]",
			  " "};
char *txt_colorm[]       ={"[ Verander ]",
                           "[    OK    ]",
                           "[ Anuleer  ]",
                           " ",
			   NULL};
			  
char *txt_bo[]={txt_info_open,txt_f_toegang,txt_f_laden,txt_h_dir,txt_sel,txt_lezen_bestn,txt_bestanden,txt_dirs,txt_f_ok};
char *txt_hm[]={"Bestand","Ga","Zoek","Opties","Help"};
char *txt_bm[]={"Openen    <o>",
	        "Ververs scherm <r>",
		"Afsluiten <F10,ESC,Q>"};
char *txt_gm[]={"Naar begin       <T>",
		"Naar einde       <B>",
	        "Naar lijn        <G>",
                "Naar einde regel <END>",
                "Naar begin regel <HOME>"};
char *txt_zm[]={"tekst    <Z,S,F>",
	        "vanaf begin tekst",
                "Nogmaals <F3,n,v,c>"};
char txt_om1[]="[ ] cursor";
char txt_om2[]="[X] vragen om af te sluiten";
char *txt_om[]={txt_om1,txt_om2,
                "Types",
		"Kleuren",
	        "Opties bewaren"};
char *txt_hlpm[]={"Zie helptekst <F1,H,?>","Toetsen ...","Over ..."};
char txt_fg[]="Voorgrond:";
char txt_bg[]="Achtergron:";
char txt_attr[]="Attribuut:";
		
