/* AbiWord
 * Copyright (C) 1998-2000 AbiSource, Inc.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
 * 02111-1307, USA.
 */

/* French translations provided by Philippe Duperron <duperron@mail.dotcom.fr> */
/* modifications by Gilles Saint-Denis <stdenisg@cedep.net> and 
   Christophe Caron <ChrisDigo@aol.com> */

/*****************************************************************
******************************************************************
** IT IS IMPORTANT THAT THIS FILE ALLOW ITSELF TO BE INCLUDED
** MORE THAN ONE TIME.
******************************************************************
*****************************************************************/

// We use the Win32 '&' character to denote a keyboard accelerator on a menu item.
// If your platform doesn't have a way to do accelerators or uses a different
// character, remove or change the '&' in your menu constructor code.

// If the third argument is true, then this is the fall-back for
// this language (named in the first argument).

BeginSetEnc(fr,FR,true,"iso-8859-1")

    MenuLabel(AP_MENU_ID__BOGUS1__,         NULL,               NULL)

    //       (id,                           	szLabel,            		szStatusMsg)

    MenuLabel(AP_MENU_ID_FILE,              	"&Fichier",         		NULL)
    MenuLabel(AP_MENU_ID_FILE_NEW,          	"&Nouveau",         		"Cr�e un nouveau document")    
    MenuLabel(AP_MENU_ID_FILE_OPEN,         	"&Ouvrir",          		"Ouvre un document existant")
    MenuLabel(AP_MENU_ID_FILE_CLOSE,        	"&Fermer",          		"Ferme le document actif")
    MenuLabel(AP_MENU_ID_FILE_SAVE,         	"&Enregistrer",     		"Enregistre le document actif")
    MenuLabel(AP_MENU_ID_FILE_SAVEAS,       	"En&registrer sous",		"Enregistre le document actif sous un nouveau nom")
	MenuLabel(AP_MENU_ID_FILE_IMPORT,			"I&mporter",				"Importe un document")
	MenuLabel(AP_MENU_ID_FILE_EXPORT,			"E&xporter",				"Enregistre le document sans changer de nom")

    MenuLabel(AP_MENU_ID_FILE_PAGESETUP,    	"Mise en &page",    		"Modifie la mise en page du document")
    MenuLabel(AP_MENU_ID_FILE_PRINT,        	"&Imprimer",        		"Imprime tout ou partie du document")
	MenuLabel(AP_MENU_ID_FILE_PRINT_DIRECTLY,	"Imprimer &directement",	"Imprime en utilisant le pilote PostScript interne")
    MenuLabel(AP_MENU_ID_FILE_PRINT_PREVIEW,	"&Aper�u avant impression",	"Aper�u du document avant impression")
    MenuLabel(AP_MENU_ID_FILE_RECENT_1,     	"&1 %s",            		"Ouvre ce document")
    MenuLabel(AP_MENU_ID_FILE_RECENT_2,     	"&2 %s",            		"Ouvre ce document")
    MenuLabel(AP_MENU_ID_FILE_RECENT_3,     	"&3 %s",            		"Ouvre ce document")
    MenuLabel(AP_MENU_ID_FILE_RECENT_4,    		"&4 %s",            		"Ouvre ce document")
    MenuLabel(AP_MENU_ID_FILE_RECENT_5,     	"&5 %s",            		"Ouvre ce document")
    MenuLabel(AP_MENU_ID_FILE_RECENT_6,    		"&6 %s",            		"Ouvre ce document")
    MenuLabel(AP_MENU_ID_FILE_RECENT_7,    		"&7 %s",            		"Ouvre ce document")
    MenuLabel(AP_MENU_ID_FILE_RECENT_8,    		"&8 %s",            		"Ouvre ce document")
    MenuLabel(AP_MENU_ID_FILE_RECENT_9,    		"&9 %s",            		"Ouvre ce document")
    MenuLabel(AP_MENU_ID_FILE_REVERT,			"&Version pr�c�dente",		"Revient � la version pr�c�dente du document")
    MenuLabel(AP_MENU_ID_FILE_EXIT,         	"&Quitter",         		"Ferme toutes les fen�tres et quitte l'application")

    MenuLabel(AP_MENU_ID_EDIT,              	"&Edition",         		NULL)
    MenuLabel(AP_MENU_ID_EDIT_UNDO,         	"&Annuler",         		"Annule la derni�re action")
    MenuLabel(AP_MENU_ID_EDIT_REDO,         	"&R�p�ter",         		"Refait l'action pr�c�demment annul�e")
    MenuLabel(AP_MENU_ID_EDIT_CUT,          	"&Couper",          		"Efface la s�lection et la place dans le presse-papiers")
    MenuLabel(AP_MENU_ID_EDIT_COPY,         	"Co&pier",          		"Copie la s�lection et la place dans le presse-papiers")
    MenuLabel(AP_MENU_ID_EDIT_PASTE,        	"C&oller",          		"Ins�re le contenu du presse-papiers au point d'insertion")
	MenuLabel(AP_MENU_ID_EDIT_PASTE_SPECIAL,	"Coller &sans format",		"Ins�re le contenu du presse-papiers sans son format")
    MenuLabel(AP_MENU_ID_EDIT_CLEAR,        	"E&ffacer",         		"Efface la selection")
    MenuLabel(AP_MENU_ID_EDIT_SELECTALL,    	"Selectio&nner tout",    	"Selectionne le document entier")
    MenuLabel(AP_MENU_ID_EDIT_FIND,         	"Rec&hercher",      		"Recherche le texte sp�cifi�")
    MenuLabel(AP_MENU_ID_EDIT_REPLACE,      	"R&emplacer",       		"Recherche et remplace le texte sp�cifi� par un diff�rent")
    MenuLabel(AP_MENU_ID_EDIT_GOTO,         	"A&tteindre",       		"D�place le point d'insertion � l'endroit sp�cifi�")
    MenuLabel(AP_MENU_ID_EDIT_EDITHEADER,		"En-t�te",					"Edite l'en-t�te de la page")
    MenuLabel(AP_MENU_ID_EDIT_EDITFOOTER,		"Pied de page",				"Edite le pied de la page")
	MenuLabel(AP_MENU_ID_EDIT_REMOVEHEADER,		"Enlever l'en-t�te",		"Enlever l'en-t�te de cette page du document")
	MenuLabel(AP_MENU_ID_EDIT_REMOVEFOOTER,		"Enlever le pied de page",	"Enlever le pied de page de cette page du document")

    MenuLabel(AP_MENU_ID_VIEW,              	"&Affichage",       		NULL)
    MenuLabel(AP_MENU_ID_VIEW_NORMAL,			"&Normale",					"Vue normale")
    MenuLabel(AP_MENU_ID_VIEW_WEB,				"&Web",						"Disposition sous forme Web")
    MenuLabel(AP_MENU_ID_VIEW_PRINT,			"&Impression",				"Disposition pour impression")
    MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,     	"&Barre d'outils",  		NULL)
    MenuLabel(AP_MENU_ID_VIEW_TB_STD,       	"&Standard",        		"Affiche ou masque la barre d'outils standard")
    MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,    	"&Mise en forme",   		"Affiche ou masque la barre d'outils de mise en forme")
    MenuLabel(AP_MENU_ID_VIEW_TB_EXTRA,			"&Extra",					"Affiche ou masque la barre d'outils extra")
    MenuLabel(AP_MENU_ID_VIEW_RULER,        	"&R�gles",          		"Affiche ou masque les r�gles")
    MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,    	"Barre d'�&tat",    		"Affiche ou masque la barre d'�tat")
    MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,     	"&Afficher/Masquer �",      "Affiche ou masque les caract�res non-imprimables")
    MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,     	"&En-t�tes et pieds de page","Edite le texte de l'en-t�te et du pied de page")
    MenuLabel(AP_MENU_ID_VIEW_FULLSCREEN, 		"Plein �&cran", 			"Affiche le document en mode plein �cran")
    MenuLabel(AP_MENU_ID_VIEW_ZOOM,         	"&Zoom",            		"R�gle le facteur d'agrandissement")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,				"&Zoom",					"R�gle le facteur d'agrandissement")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM_200,			"&200%",					"R�gle le facteur d'agrandissement � 200%")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM_100,			"&100%",					"R�gle le facteur d'agrandissement � 100%")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM_75,			"&75%",						"R�gle le facteur d'agrandissement � 75%")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM_50,			"&50%",						"R�gle le facteur d'agrandissement � 50%")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM_WIDTH,		"&Largeur de page",			"R�gle le facteur d'agrandissement � la largeur de page")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM_WHOLE,		"&Toute la page",			"R�gle le facteur d'agrandissement � la page enti�re")

    MenuLabel(AP_MENU_ID_INSERT,            	"&Insertion",      			NULL)
    MenuLabel(AP_MENU_ID_INSERT_BREAK,      	"&Saut",           			"Ins�re un saut de page, de colonne ou de section")
    MenuLabel(AP_MENU_ID_INSERT_PAGENO,     	"N&um�ros de page",         "Ins�re un num�ro de page mis � jour automatiquement")
    MenuLabel(AP_MENU_ID_INSERT_DATETIME,   	"Date et &heure",  			"Ins�re la date et/ou l'heure")
    MenuLabel(AP_MENU_ID_INSERT_FIELD,      	"&Champ",          			"Ins�re un champ calcul�")
    MenuLabel(AP_MENU_ID_INSERT_FILE,			"&Fichier",					"Ins�re le contenu d'un autre fichier")
    MenuLabel(AP_MENU_ID_INSERT_SYMBOL,     	"Caract�res s&p�ciaux",    	"Ins�re un symbole ou un caract�re sp�cial")
	MenuLabel(AP_MENU_ID_INSERT_ENDNOTE,		"&Note",					"Ins�re une note � la fin du document")
    MenuLabel(AP_MENU_ID_INSERT_PICTURE,		"&Image",					"Ins�re une image")
	MenuLabel(AP_MENU_ID_INSERT_CLIPART,		"&Clip Art",				"Ins�re un graphique pr�dessin�")
    MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,    	"&Image",          			"Ins�re une image existante � partir d'un autre fichier")

    MenuLabel(AP_MENU_ID_FORMAT,            	"F&ormat",         			NULL)
	MenuLabel(AP_MENU_ID_FMT_LANGUAGE,			"&Langue",					"Change la langue du texte s�lectionn�")
    MenuLabel(AP_MENU_ID_FMT_FONT,          	"&Police",         			"Change la police du texte s�lectionn�")
    MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,     	"Pa&ragraphe",     			"Change le format du paragraphe s�lectionn�")
    MenuLabel(AP_MENU_ID_FMT_BULLETS,       	"P&uces et num�ros",		"Ajoute ou modifie les puces et num�ros du paragraphe s�lectionn�")
    MenuLabel(AP_MENU_ID_FMT_DOCUMENT,      	"Page",             		"D�finit les propri�t�s du document comme la taille de la page et les marges")
    MenuLabel(AP_MENU_ID_FMT_BORDERS,       	"Bor&dures et trames",		"Ajoute des bordures et une trame � la s�lection")
	MenuLabel(AP_MENU_ID_FMT_HDRFTR,			"En-t�te et pied de page",	"D�finit les types d'en-t�te et de pied de page")
    MenuLabel(AP_MENU_ID_FMT_COLUMNS,       	"C&olonnes",       			"Change le nombre de colonnes")
    MenuLabel(AP_MENU_ID_FMT_TOGGLECASE,		"C&hanger la casse",		"Change la casse du texte s�lectionn�")
    MenuLabel(AP_MENU_ID_FMT_BACKGROUND,		"&Fond",					"Change la couleur de fond du document")
    MenuLabel(AP_MENU_ID_FMT_STYLE,         	"&Style",          			"Definit ou applique un style � la s�lection")
    MenuLabel(AP_MENU_ID_FMT_TABS,          	"&Tabulations",    			"Definit les tabulations")
    MenuLabel(AP_MENU_ID_FMT_BOLD,          	"&Gras",           			"Met la s�lection en gras (bascule)")
    MenuLabel(AP_MENU_ID_FMT_ITALIC,        	"&Italique",       			"Met la s�lection en italique (bascule)")
    MenuLabel(AP_MENU_ID_FMT_UNDERLINE,     	"Sou&lign�",       			"Souligne la s�lection (bascule)")
    MenuLabel(AP_MENU_ID_FMT_OVERLINE,      	"Barr� &haut",     			"Barre la s�lection en haut (bascule)")
    MenuLabel(AP_MENU_ID_FMT_STRIKE,        	"&Barr�",          			"Barre la s�lection (bascule)")
	MenuLabel(AP_MENU_ID_FMT_TOPLINE,			"Ligne en haut",			"Ligne au dessus de la s�lection (bascule)")
	MenuLabel(AP_MENU_ID_FMT_BOTTOMLINE,		"Ligne en bas",				"Ligne au bas de la s�lection (bascule)")
    MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,   	"&Exposant",       			"Met la s�lection en exposant (bascule)")
    MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,     	"I&ndice",         			"Met la s�lection en indice (bascule)")

    MenuLabel(AP_MENU_ID_TOOLS,					"O&utils",					NULL)   
    MenuLabel(AP_MENU_ID_TOOLS_SPELLING, 		"O&rthographe", 			NULL)
    MenuLabel(AP_MENU_ID_TOOLS_SPELL,       	"&Orthographe",				"V�rifie l'orthographe du document")
    MenuLabel(AP_MENU_ID_TOOLS_AUTOSPELL, 		"&V�rification automatique","V�rification automatique du document")
	MenuLabel(AP_MENU_ID_TOOLS_SPELLPREFS,		"&Options",					"Configure les options de v�rification orthographique")
    MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,		"&Statistiques",			"Statistiques du document")
    MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,     	"O&ptions",         		"Configure les diff�rentes options")
    MenuLabel(AP_MENU_ID_TOOLS_LANGUAGE,		"La&ngue",					"Change la langue du texte s�lectionn�")
	MenuLabel(AP_MENU_ID_TOOLS_PLUGINS,			"&Modules",					"Gestionnaire de modules")
	MenuLabel(AP_MENU_ID_TOOLS_SCRIPTS,			"S&cript",					"Ex�cute un script")
    
    MenuLabel(AP_MENU_ID_ALIGN,             	"&Alignement",     			NULL)
    MenuLabel(AP_MENU_ID_ALIGN_LEFT,        	"� &Gauche",       			"Aligne le paragraphe � gauche")
    MenuLabel(AP_MENU_ID_ALIGN_CENTER,      	"&Centr�",         			"Centre le paragraphe")
    MenuLabel(AP_MENU_ID_ALIGN_RIGHT,       	"� &Droite",       			"Aligne le paragraphe � droite")
    MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,     	"&Justifi�",       			"Justifie le paragraphe � droite et � gauche")

	MenuLabel(AP_MENU_ID_WEB,					"We&b",						NULL)
    MenuLabel(AP_MENU_ID_WEB_SAVEASWEB,        "&Enregistrer sous le web",	"Enregistre le document comme une page web")
    MenuLabel(AP_MENU_ID_WEB_WEBPREVIEW,       "&Aper�u de la page web",	"Aper�u du document comme une page web")

    MenuLabel(AP_MENU_ID_WINDOW,            	"Fe&n�tre",        			NULL)
    MenuLabel(AP_MENU_ID_WINDOW_NEW,        	"&Nouvelle fen�tre",		"Ouvre une nouvelle fen�tre sur le document actif")
    MenuLabel(AP_MENU_ID_WINDOW_1,          	"&1 %s",           			"Passe � la fen�tre contenant ce document")
    MenuLabel(AP_MENU_ID_WINDOW_2,          	"&2 %s",           			"Passe � la fen�tre contenant ce document")
    MenuLabel(AP_MENU_ID_WINDOW_3,          	"&3 %s",           			"Passe � la fen�tre contenant ce document")
    MenuLabel(AP_MENU_ID_WINDOW_4,          	"&4 %s",           			"Passe � la fen�tre contenant ce document")
    MenuLabel(AP_MENU_ID_WINDOW_5,          	"&5 %s",           			"Passe � la fen�tre contenant ce document")
    MenuLabel(AP_MENU_ID_WINDOW_6,          	"&6 %s",           			"Passe � la fen�tre contenant ce document")
    MenuLabel(AP_MENU_ID_WINDOW_7,          	"&7 %s",           			"Passe � la fen�tre contenant ce document")
    MenuLabel(AP_MENU_ID_WINDOW_8,          	"&8 %s",           			"Passe � la fen�tre contenant ce document")
    MenuLabel(AP_MENU_ID_WINDOW_9,          	"&9 %s",           			"Passe � la fen�tre contenant ce document")
    MenuLabel(AP_MENU_ID_WINDOW_MORE,       	"&Plus de fen�tres",		"Montre la liste compl�te des fen�tres")

    MenuLabel(AP_MENU_ID_HELP,              	"&Aide",            		NULL)
	MenuLabel(AP_MENU_ID_HELP_CREDITS,			"&Cr�dits et remerciements","Affiche la mention des contributions")
    MenuLabel(AP_MENU_ID_HELP_CONTENTS,			"&Sommaire",				"Affiche le sommaire de l'aide")
    MenuLabel(AP_MENU_ID_HELP_INDEX,			"&Index de l'aide",			"Affiche l'index de l'aide")
    MenuLabel(AP_MENU_ID_HELP_CHECKVER,			"&V�rifier la version",		"Affiche le num�ro de version du programme")
    MenuLabel(AP_MENU_ID_HELP_SEARCH,			"&Rechercher dans l'aide",	"Recherche de l'aide � propos de...")
    MenuLabel(AP_MENU_ID_HELP_ABOUT,        	"&A propos de %s",  		"Affiche des informations sur le programme, le num�ro de version et le copyright")
    MenuLabel(AP_MENU_ID_HELP_ABOUTOS,			"A propos de l'&Open Source",	"Affiche des informations sur le concept Open Source")
	MenuLabel(AP_MENU_ID_HELP_ABOUT_GNU,		"A propos de &GNU Free Software", "Affiche des informations sur le projet GNU")
	MenuLabel(AP_MENU_ID_HELP_ABOUT_GNOMEOFFICE, "A propos de G&NOME Office", "Affiche des informations sur le projet GNOME Office")

    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,		"%s",						"Change pour le texte sugg�r�")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,		"%s",						"Change pour le texte sugg�r�")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,		"%s",						"Change pour le texte sugg�r�")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,		"%s",						"Change pour le texte sugg�r�")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,		"%s",						"Change pour le texte sugg�r�")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,		"%s",						"Change pour le texte sugg�r�")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,		"%s",						"Change pour le texte sugg�r�")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,		"%s",						"Change pour le texte sugg�r�")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,		"%s",						"Change pour le texte sugg�r�")
    MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,		"&Ignorer tout", 			"Ignore le mot chaque fois qu'on le rencontre dans le document")
    MenuLabel(AP_MENU_ID_SPELL_ADD,				"&Ajouter", 				"Ajouter ce mot au dictionnaire personnel")

    /* autotext submenu labels */
    MenuLabel(AP_MENU_ID_INSERT_AUTOTEXT,		"&Texte automatique",		"")
    MenuLabel(AP_MENU_ID_AUTOTEXT_ATTN,			"Attention",				"")
    MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING,		"Formule de politesse",		"") 
    MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL,			"Instruction de courrier",	"")
    MenuLabel(AP_MENU_ID_AUTOTEXT_REFERENCE,	"R�f�rence",				"")
    MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION,	"Salutation",				"")
    MenuLabel(AP_MENU_ID_AUTOTEXT_SUBJECT,		"Sujet",					"")
    MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL,		"Courriel",					"")

	MenuLabel(AP_MENU_ID_AUTOTEXT_ATTN_1, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_ATTN_2, "%s", " ")
	
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_1, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_2, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_3, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_4, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_5, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_6, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_7, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_8, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_9, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_10, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_11, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_12, "%s", " ")
	
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_1, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_2, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_3, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_4, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_5, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_6, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_7, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_8, "%s", " ")
	
	MenuLabel(AP_MENU_ID_AUTOTEXT_REFERENCE_1, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_REFERENCE_2, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_REFERENCE_3, "%s", " ")
	
	MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION_1, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION_2, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION_3, "%s", " ")

	MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION_4, "%s", " ")

	MenuLabel(AP_MENU_ID_AUTOTEXT_SUBJECT_1, "%s", " ")

	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL_1, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL_2, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL_3, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL_4, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL_5, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL_6, "%s", " ")

    // ... add others here ...

    MenuLabel(AP_MENU_ID__BOGUS2__,				NULL,						NULL)

EndSet()
