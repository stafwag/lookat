/*
 *  eng.h
 *
 *  Copyright (C) 1997, 1998, 2000, 2001, 2002, 2003, 2004, 2006, 2011, 2012  Staf Wagemakers Belgie/Belgium
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

const char sel_bm		='f';
const char sel_gm		='g';
const char sel_zm		='s';
const char sel_hm		='h';
const int sel_om                ='o';
char *txt_help[]		= {
"---------------------------------------------------------------------------",
"lookat 1.4.3                                (GPL) 1997-2013 Belgie  Belgium",
"                                                            Staf Wagemakers",
"                                                  email: staf@wagemakers.be",
" ",
"How to start 'lookat'?",
" ",
"- by just typing 'lookat', after the program is started you can select the",
"  file you want to view.",
"- by typing 'lookat <dir_name>', change the current directory to <dir_name>.",
"- by typing 'lookat <filename>', the file with <filename> will automatically",
"  be loaded.",
"- read form stdin",
"  examples : lookat < test.txt",
"             ls | lookat",
"- use 'lookat' as a manual page viewer : man -Plookat ls",
" ",
"Thank you for using this program!",
"--------------------------------------------------------------------------\n",NULL};
								    
int hkey_jn[]		={'y','n'};
int hkey_typem[]	={'i','a','m','d','d'};
int hkey_cv_color[]	={'f','b','a','o','t','c'};
int hkey_cv_mono[]	={'f','b','o','t','c'};
int hkey_colorm[]	={'m','o','c',0,0};
int hkey_ok[]           ={'o'};
int hkey_ok_anu[]       ={'o','c'};
int hkey_zoek[]         ={0,0,'o','c'};
int hkey_type[]		={'o','c'};
int hkey_bm[]           ={'o','r','q'};
int hkey_gm[]           ={'t','b','l',KEY_END,KEY_HOME};
int hkey_zm[]           ={'s','b','a'};
int hkey_om[]           ={0,'n','t','c','s'};
int hkey_hlp[]          ={'s','k','a'};
unsigned hplace_bm[]         	={0,1,0,1,0,1};
unsigned hplace_gm[]         	={3,1,3,1,3,1,19,5,19,6};
unsigned hplace_zm[]         	={2,1,5,1,0,1};
unsigned hplace_om[]         	={3,1,3,1,0,1,0,1}; 
unsigned hplace_hlpm[]       	={0,1,0,1,0,1};
unsigned hplace_hm[]         	={0,1,0,1,0,1,0,1,0,1};
unsigned hplace_ok[]         	={2,1};
unsigned hplace_jn[]         	={3,1,3,1};
unsigned hplace_typem[]		={2,1,3,1,2,1,2,1,3,1,0,0};
unsigned hplace_cv_color[]	={0,1, 0,1, 0,1, 4,1, 3,1, 2,1};
unsigned hplace_cv_mono[]	={0,1, 0,1, 4,1, 3,1, 2,1};
unsigned hplace_colorm[]	={2,1,4,1,2,1,0,0,0,0};
unsigned hplace_ok_anu[]     	={4,1,2,1};
unsigned hplace_zoek[]       	={0,0,0,0,4,1,2,1};
unsigned hplace_type[]      	={4,1,2,1};


char txt_info_open[]	="Select a file or directory     TAB = files <-> directories       ESC = cancel";
char txt_term[]         ="Program terminated by signal ";
char *txt_f_writecfg[]	={"Sorry, cant't write config file...\n",NULL};
char txt_f_open1[]	="Sorry, I can't open : ";
char txt_f_open2[]	="\n";
char txt_f_laden[]	="No such file or directory...";
char txt_f_fseek[]	="Sorry, fseek() has failed ...";
char txt_f_toegang[]	="Access denied ...";
char txt_f_ok[]         ="[ Ok ]";
char txt_f_initscr[]	="Sorry, initscr() has failed ...";
char txt_h_dir[]	=" Current directory : ";
char txt_sel[]		=" Selection         : "; 
char txt_lezen_bestn[]	=" Reading the filename ... ";
char txt_bestanden[]	=" Files ";
char txt_dirs[]		=" Directories ";
char txt_t_links[]      ="C-b        : left  / back";
char txt_t_rechts[]     ="C-f        : right / forward";
char txt_t_omhoog[]     ="C-p        : up    / previous line";
char txt_t_omlaag[]     ="C-n        : down  / next line";
char txt_t_u[]          ="u          : PgUp";
char txt_t_d[]          ="d          : PgDwn";
char txt_t_o[]		="o          : open a file";
char txt_t_zsf[]	="/,z,s,f    : search text";
char txt_t_F3[]		="F3,n,c,v,\\ : continue search";
char txt_t_HOME[]	="HOME,0,^   : go to the beginning of a line";
char txt_t_END[]	="END        : go to the end of a line";
char txt_t_b[]		="t,<        : go to begin of the file";
char txt_t_e[]		="b,e,>      : go the end of the file";
char txt_t_F9[]		="F9         : menu";
char txt_t_help[]	="h,?,F1     : view help file";
char txt_t_QUIT[]	="Q,ESC,F10  : quit program ... :-(";
char txt_t_rebuild[]	="r,C-l      : rebuild screen";
char *txt_t[]={txt_t_links,txt_t_rechts,txt_t_omhoog,txt_t_omlaag,txt_t_u,txt_t_d,txt_t_o,txt_t_zsf,txt_t_F3,txt_t_HOME,txt_t_END,txt_t_b,txt_t_e,txt_t_F9,txt_t_help,txt_t_QUIT,txt_t_rebuild,NULL};

char *txt_ok[]		={"[ OK ]"};
char *txt_ok_anu[]	={"[   OK   ]","[ Cancel ]"};
char txt_X[]		="X";
char txt_spacie[]	=" ";
char *txt_m_zoek[]	={txt_spacie,txt_spacie,"[   OK   ]","[ Cancel ]"};
char *txt_m_type[]	={"[   OK   ]","[ Cancel ]"};
char *txt_cm_color[]	={"Foreground",
		          "Background",
			  "Attribute ",
			  "[   OK   ]","[  Try   ]","[ Cancel ]",NULL};
char *txt_cm_mono[]	={"Foreground",
		          "Background",
			  "[   OK   ]","[  Try   ]","[ Cancel ]",NULL};
char *txt_kleuren[] 	= {"black","red","green","yellow","blue",
		           "magenta","cyan", "white",NULL};
char *txt_attributen[] = {"normal","bold","underline","reverse",
                          "standout",NULL};
char txt_ondersheid_g[]	="[ ] Case sensitive";
char txt_search_back[]  ="[ ] Search backwards";
char *txt_over[]	={
"Lookat version 1.4.3               GNU Public License",
"Staf Wagemakers                   (1997-2013) Belgium","\n",
"email:                             staf@wagemakers.be",
"homepage:                    http://www.wagemakers.be","\n",NULL};
char txt_bedank[]       ="Thank you for using this program!";
char *txt_verder_zoeken_begin[]={"Top of document reached, continue from bottom?",NULL};
char *txt_verder_zoeken_einde[]={"End of document reached, continue from beginning?",NULL};
char *txt_t_nt_gevonden[]={"String not found ...",NULL};
char txt_z_v_tekst[]	=" Search a string ... ";
char txt_c_type[]	=" Change a type ... ";
char txt_n_type[]	=" New type ... ";
char txt_geef_de_tekst[]="String to search  : ";
char txt_typename[]	="Type name   :";
char txt_typeval[]	="Type value  :";
char txt_ga_lijn[]	=" Go to a line ... ";
char txt_geef_lijnnr[]	="Number of the line : ";
char txt_lijn[]		="line";
char txt_kol[]		="col.";
char txt_Grootte[]	="size";
char txt_maak_sel[]	="Make a selection ...";
char *txt_stop[]	={"Do you really want to quit \"lookat\"?",NULL};
char *txt_jn[]		={"[  Yes  ]","[  No  ]"};
char *txt_typem[]	={"[ Insert ]",
			  "[  Add  ]",
			  "[ Modify ]",
		          "[ Delete ]",
                          "[  Done  ]",
			  " "};
char *txt_colorm[]       ={"[ Modify ]",
                           "[   Ok   ]",
                           "[ Cancel ]",
                           " ",
			   NULL};
char *txt_bo[]={txt_info_open,txt_f_toegang,txt_f_laden,txt_h_dir,txt_sel,txt_lezen_bestn,txt_bestanden,txt_dirs,txt_f_ok};
char *txt_hm[]={"File","Go","Search","Options","Help"};
char *txt_bm[]={"Open <o>",
	        "Refresh screen <r>",
		"Quit <F10,ESC,Q>"};
char *txt_gm[]={"to top             <T>",
                "to bottom          <B>",
                "to line            <G>",
                "to end of a line   <END>",
                "to begin of a line <HOME>"};
char *txt_zm[]={"a string   <Z,S,F>",
                "form begin of the file",
                "Again      <F3,n,c,v>"};
char txt_om1[]="[ ] cursor";
char txt_om2[]="[X] give notice";
char *txt_om[]= {txt_om1,txt_om2,
		  "Types",
		  "Colors",
	          "Save options" };
char *txt_hlpm[]={"View help file <F1,H,?>","Keys ...","About ..."};

char txt_fg[]="Foreground:";
char txt_bg[]="Background:";
char txt_attr[]="Attribute:";
		


