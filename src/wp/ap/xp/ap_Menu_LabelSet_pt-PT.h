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

BeginSetEnc(pt,PT,true,"iso-8859-1")

    MenuLabel(AP_MENU_ID__BOGUS1__,		NULL,			NULL)

    //       (id,                           szLabel,            szStatusMsg)

    MenuLabel(AP_MENU_ID_FILE,			"&Ficheiro",		NULL)
    MenuLabel(AP_MENU_ID_FILE_NEW,		"&Novo",		"Criar novo documento")    
    MenuLabel(AP_MENU_ID_FILE_OPEN,		"&Abrir",		"Abrir documento")
    MenuLabel(AP_MENU_ID_FILE_CLOSE,		"Fe&char",		"Fechar documento")
    MenuLabel(AP_MENU_ID_FILE_SAVE,		"&Gravar",		"Gravar documento")
    MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"Gra&var como",		"Gravar documento com novo nome")
    MenuLabel(AP_MENU_ID_FILE_IMPORT,		"&Importar",		"Importar documento para a posi��o actual do cursor")
    MenuLabel(AP_MENU_ID_FILE_EXPORT,		"&Exportar",		"Gravar c�pia do documento com novo nome")

    MenuLabel(AP_MENU_ID_FILE_PAGESETUP,	"&Pagina��o",		"Modificar Pagina��o")
    MenuLabel(AP_MENU_ID_FILE_PRINT,		"&Imprimir",		"Imprimir documento")
    MenuLabel(AP_MENU_ID_FILE_PRINT_DIRECTLY,	"Imprimir &Directamente","Imprimir documento com o driver interno de PostScript")
    MenuLabel(AP_MENU_ID_FILE_PRINT_PREVIEW,	"Ver Antes",		"Ver documento antes de imprimir")
    MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"&1 %s",		"�ltimo documento utilizado")
    MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"&2 %s",		"Pen�ltimo documento utilizado")
    MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"&3 %s",		"Antepen�ltimo documento utilizado")
    MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"&4 %s",		"Quarto documento utilizado")
    MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"&5 %s",		"Quinto documento utilizado")
    MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"&6 %s",		"Sexto documento utilizado")
    MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"&7 %s",		"S�ptimo documento utilizado")
    MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"&8 %s",		"Oitavo documento utilizado")
    MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"&9 %s",		"Nono documento utilizado")
    MenuLabel(AP_MENU_ID_FILE_REVERT,		"&Reverter",		"Reverter o documento para o �ltimo estado em que foi gravado")
    MenuLabel(AP_MENU_ID_FILE_EXIT,		"&Sair",		"Fechar todas as janelas e sair")

    MenuLabel(AP_MENU_ID_EDIT,			"&Editar",		NULL)
    MenuLabel(AP_MENU_ID_EDIT_UNDO,		"&Desfazer",		"Cancelar �ltima ac��o")
    MenuLabel(AP_MENU_ID_EDIT_REDO,		"&Refazer",		"Refazer �ltima ac��o cancelada")
    MenuLabel(AP_MENU_ID_EDIT_CUT,		"Co&rtar",		"Cortar selec��o")
    MenuLabel(AP_MENU_ID_EDIT_COPY,		"&Copiar",		"Copiar selec��o")
    MenuLabel(AP_MENU_ID_EDIT_PASTE,		"Co&lar",		"Colar selec��o")
    MenuLabel(AP_MENU_ID_EDIT_PASTE_SPECIAL,	"Colar &Especial",	"Colar objecto n�o formatado")
    MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"Apa&gar",		"Apagar selec��o")
    MenuLabel(AP_MENU_ID_EDIT_SELECTALL,	"Seleccionar &tudo",	"Seleccionar todo o documento")
    MenuLabel(AP_MENU_ID_EDIT_FIND,		"&Procurar",		"Procurar no documento")
    MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"&Substituir",		"Substituir no documento")
    MenuLabel(AP_MENU_ID_EDIT_GOTO,		"&Ir para",		"Deslocar o cursor para")
    MenuLabel(AP_MENU_ID_EDIT_EDITHEADER,	"Editar &Cabe�alho",	"Editar cabe�alho")
    MenuLabel(AP_MENU_ID_EDIT_EDITFOOTER,	"Editar &Rodap�",	"Editar rodap�")
    MenuLabel(AP_MENU_ID_EDIT_REMOVEHEADER,	"Remover Cabe�alho",	"Remover cabe�alho")
    MenuLabel(AP_MENU_ID_EDIT_REMOVEFOOTER,	"Remover Rodap�",	"Remover rodap�")

    MenuLabel(AP_MENU_ID_VIEW,			"&Ver",			NULL)
    MenuLabel(AP_MENU_ID_VIEW_NORMAL,		"Esquema Normal",	"Esquema Normal")
    MenuLabel(AP_MENU_ID_VIEW_WEB,		"Esquema &Web",		"Ver documento como p�gina WWW")
    MenuLabel(AP_MENU_ID_VIEW_PRINT,		"Layout de &Impress�o",	"Ver documento como se impresso")
    MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"&Barras de utilit�rios", NULL)
    MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"&Standard",		"Ver/Omitir Barra Standard")
    MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,	"&Formata��o",		"Ver/Omitir Barra de Formata��o")
    MenuLabel(AP_MENU_ID_VIEW_TB_EXTRA,		"&Extra",		"Ver/Omitir Barra Extra")
    MenuLabel(AP_MENU_ID_VIEW_RULER,		"&R�guas",		"Ver/Omitir R�guas")
    MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,	"Barra de &mensagens",	"Ver/Omitir Barra de Mensagens")
    MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"M&ostrar �",		"Ver/Omitir Caracteres N�o Imprim�veis")
    MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"&Cabe�alhos e Rodap�s","Editar os cabe�alhos e rodap�s")
    MenuLabel(AP_MENU_ID_VIEW_FULLSCREEN,	"&S� a P�gina",		"Mostrar somente as p�ginas")
    MenuLabel(AP_MENU_ID_VIEW_ZOOM,		"&Zoom",		"Aproximar ou afastar do texto")
    MenuLabel(AP_MENU_ID_VIEW_ZOOM_MENU,	"&Zoom",		"Aproximar ou afastar do texto")
    MenuLabel(AP_MENU_ID_VIEW_ZOOM_200,		"Zoom to &200%",	"Zoom a 200%")
    MenuLabel(AP_MENU_ID_VIEW_ZOOM_100,		"Zoom to &100%",	"Zoom a 100%")
    MenuLabel(AP_MENU_ID_VIEW_ZOOM_75,		"Zoom to &75%",		"Zoom a 75%")
    MenuLabel(AP_MENU_ID_VIEW_ZOOM_50,		"Zoom to &50%",		"Zoom a 50%")
    MenuLabel(AP_MENU_ID_VIEW_ZOOM_WIDTH,	"Zoom to &Page Width",	"Zoom � largura da p�gina")
    MenuLabel(AP_MENU_ID_VIEW_ZOOM_WHOLE,	"Zoom to &Whole Page",	"Zoom � p�gina inteira")

    MenuLabel(AP_MENU_ID_INSERT,		"&Inserir",		NULL)
    MenuLabel(AP_MENU_ID_INSERT_BREAK,		"&Quebra",		"Inserir quebras de p�gina, sec��o...")
    MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"&N�mera��o",		"Inserir n�mera��o de p�ginas")
    MenuLabel(AP_MENU_ID_INSERT_DATETIME,	"Data e &Hora",		"Inserir data e hora")
    MenuLabel(AP_MENU_ID_INSERT_FIELD,		"&Campo",		"Inserir campo calculado")
    MenuLabel(AP_MENU_ID_INSERT_FILE,		"&Inserir Documento",	"Inserir o conte�do de um outro documento")
    MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"&S�mbolo",		"Inserir caracteres especiais")
    MenuLabel(AP_MENU_ID_INSERT_ENDNOTE,	"Nota &Final",		"Inserir nota final")
    MenuLabel(AP_MENU_ID_INSERT_PICTURE,	"&Imagem",		"Inserir imagem")
    MenuLabel(AP_MENU_ID_INSERT_CLIPART,	"&Portfolio",		"Inserir gr�ficos do portfolio")
#ifdef HAVE_GNOME
    MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,	"Do &Ficheiro",		"Inserir uma imagem a partir de um ficheiro")
#else
    MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,	"&Imagem",		"Inserir imagem")
#endif
    MenuLabel(AP_MENU_ID_INSERT_BOOKMARK,	"&Bookmark",		"Inserir bookmark")
    MenuLabel(AP_MENU_ID_INSERT_HYPERLINK,	"&Hiperliga��o",	"Inserir hiperliga��o")

    MenuLabel(AP_MENU_ID_FORMAT,		"F&ormatar",		NULL)
    MenuLabel(AP_MENU_ID_FMT_LANGUAGE,		"&L�ngua",		"Mudar l�gua da �rea seleccionada")
    MenuLabel(AP_MENU_ID_FMT_FONT,		"&Tipo",		"Mudar tipo de letra")
    MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"&Par�grafo",		"Mudar defini��o do par�grafo")
    MenuLabel(AP_MENU_ID_FMT_BULLETS,		"&Listas",		"Formatar listas (n�o) numeradas")
    MenuLabel(AP_MENU_ID_FMT_DOCUMENT,		"&Documento",		"Formatar propriedades do documento, tais como tamanho do papel e margens")
    MenuLabel(AP_MENU_ID_FMT_BORDERS,		"Bordas e &Sombras",	"ajustar bordas e sombreados")
    MenuLabel(AP_MENU_ID_FMT_HDRFTR,		"Formatar Cabe�alho/Rodap�",	"Definir os tipos de Cabe�alhos e Rodap�s")
    MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"C&olunas",		"Mudar o n�mero de colunas")
    MenuLabel(AP_MENU_ID_FMT_TOGGLECASE,	"&Mai�sculas/Min�sculas","Mudar capitaliza��o da �rea seleccionada")
    MenuLabel(AP_MENU_ID_FMT_BACKGROUND,	"&Cor de Fundo",	"Mudar cor de fundo")
    MenuLabel(AP_MENU_ID_FMT_STYLE,		"&Estilo",		"Definir ou aplicar estilos")
    MenuLabel(AP_MENU_ID_FMT_TABS,		"Ta&bula��o",		"Definir tabula��o")
    MenuLabel(AP_MENU_ID_FMT_BOLD,		"&Negrito",		"Engrossar texto")
    MenuLabel(AP_MENU_ID_FMT_ITALIC,		"&It�lico",		"Inclinar texto")
    MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"S&ublinhado",		"Sublinhar texto")
    MenuLabel(AP_MENU_ID_FMT_OVERLINE,		"Sobrelin&hado",	"Sobrelinhar texto")
    MenuLabel(AP_MENU_ID_FMT_STRIKE,		"&Riscar",		"Riscar texto")
    MenuLabel(AP_MENU_ID_FMT_TOPLINE,		"Linha &Superior",	"Colocar/Remover linha acima da selec��o")
    MenuLabel(AP_MENU_ID_FMT_BOTTOMLINE,	"Linha &Inferior",	"Colocar/Remover linha abaixo da selec��o")
    MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,	"&Expoente",		"Elevar texto")
    MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,		"�n&dice",		"Indiciar texto")

    MenuLabel(AP_MENU_ID_TOOLS,			"Fe&rramentas",		NULL)
    MenuLabel(AP_MENU_ID_TOOLS_SPELLING,	"&Ortografia",		NULL)
    MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"Verificar agora",	"Verificar ortografia do documento agora")
    MenuLabel(AP_MENU_ID_TOOLS_AUTOSPELL,	"Verifica��o &Autom�tica","Verificar ortografia do documento automaticamente")
    MenuLabel(AP_MENU_ID_TOOLS_SPELLPREFS,	"&Op��es de Ortografia", "Definir op��es de ortografia")
    MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,	"&Contar palavras",	"Contar palavras no documento")
    MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"&Op��es",		"Configurar as op��es")
    MenuLabel(AP_MENU_ID_TOOLS_LANGUAGE,	"&L�ngua",		"Mudar a l�ngua da �rea seleccionada")
    MenuLabel(AP_MENU_ID_TOOLS_PLUGINS,		"P&lugins",		"Gerir plugins")
    MenuLabel(AP_MENU_ID_TOOLS_SCRIPTS,		"S&cripts",		"Executar scripts")
    
    MenuLabel(AP_MENU_ID_ALIGN,			"&Alinhamento",		NULL)
    MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"� &Esquerda",		"Alinhar � esquerda")
    MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"&Centrar",		"Centrar texto")
    MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"� &Direita",		"Alinhar � direita")
    MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"&Justificar",		"Justificar texto")

    MenuLabel(AP_MENU_ID_WEB,			"We&b",			NULL)
    MenuLabel(AP_MENU_ID_WEB_SAVEASWEB,		"Gravar como &Web",	"Gravar o documento como uma p�gina de WWW")
    MenuLabel(AP_MENU_ID_WEB_WEBPREVIEW,	"Ver antes como &Web",	"Ver o documento (antes de imprimir) como uma p�gina de WWW")

    MenuLabel(AP_MENU_ID_WINDOW,		"&Janela",		NULL)
    MenuLabel(AP_MENU_ID_WINDOW_NEW,		"&Nova",		"Abrir outra janela para este documento")
    MenuLabel(AP_MENU_ID_WINDOW_1,		"&1 %s",		"Mudar para a primeira janela")
    MenuLabel(AP_MENU_ID_WINDOW_2,		"&2 %s",		"Mudar para a segunda janela")
    MenuLabel(AP_MENU_ID_WINDOW_3,		"&3 %s",		"Mudar para a terceira janela")
    MenuLabel(AP_MENU_ID_WINDOW_4,		"&4 %s",		"Mudar para a quarta janela")
    MenuLabel(AP_MENU_ID_WINDOW_5,		"&5 %s",		"Mudar para a quinta janela")
    MenuLabel(AP_MENU_ID_WINDOW_6,		"&6 %s",		"Mudar para a sexta janela")
    MenuLabel(AP_MENU_ID_WINDOW_7,		"&7 %s",		"Mudar para a septima janela")
    MenuLabel(AP_MENU_ID_WINDOW_8,		"&8 %s",		"Mudar para a oitava janela")
    MenuLabel(AP_MENU_ID_WINDOW_9,		"&9 %s",		"Mudar para a nona janela")
    MenuLabel(AP_MENU_ID_WINDOW_MORE,		"&Mais janelas",	"Mostrar lista completa de janelas")

    MenuLabel(AP_MENU_ID_HELP,			"&Ajuda",		NULL)
    MenuLabel(AP_MENU_ID_HELP_CREDITS,		"C&r�ditos",		"Mostrar cr�ditos")
    MenuLabel(AP_MENU_ID_HELP_CONTENTS,		"&T�picos de Ajuda",	"Mostrar t�picos de ajuda")
    MenuLabel(AP_MENU_ID_HELP_INDEX,		"&Index da Ajuda",	"Mostrar o Index da ajuda")
    MenuLabel(AP_MENU_ID_HELP_CHECKVER,		"&Vers�o",		"Mostrar vers�o do AbiWord")
    MenuLabel(AP_MENU_ID_HELP_SEARCH,		"&Pesquisar na Ajuda",	"Pesquisar ajuda sobre...")
    MenuLabel(AP_MENU_ID_HELP_ABOUT,		"&Acerca do %s",	"Ver informa��es sobre o programa, n�mero de vers�o e copyrights")
    MenuLabel(AP_MENU_ID_HELP_ABOUTOS,		"Acerca de &Open Source",	"Mostrar informa��o a respeito de Open Source")
    MenuLabel(AP_MENU_ID_HELP_ABOUT_GNU,	"Acerca do Software Livre &GNU", "Saiba mais sobre o Software Livre e o Projecto GNU")
    MenuLabel(AP_MENU_ID_HELP_ABOUT_GNOMEOFFICE, "Acerca do G&NOME Office", "Saiba mais sobre o projecto GNOME Office")

    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,	"%s",			"Mudar para a sugest�o")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,	"%s",			"Mudar para a sugest�o")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,	"%s",			"Mudar para a sugest�o")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,	"%s",			"Mudar para a sugest�o")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,	"%s",			"Mudar para a sugest�o")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,	"%s",			"Mudar para a sugest�o")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,	"%s",			"Mudar para a sugest�o")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,	"%s",			"Mudar para a sugest�o")
    MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,	"%s",			"Mudar para a sugest�o")
    MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,	"&Ignorar tudo",	"Ignorar todas as ocorr�ncias desta palavra no documento")
    MenuLabel(AP_MENU_ID_SPELL_ADD,		"&Adicionar",		"Adicionar esta palavra ao dicion�rio custumiz�vel")

    /* autotext submenu labels */
    MenuLabel(AP_MENU_ID_INSERT_AUTOTEXT,	"&Texto Autom�tico",	"")
    MenuLabel(AP_MENU_ID_AUTOTEXT_ATTN,		"Aten��o:",		"")
    MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING,	"Finalizando:",		"")
    MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL,		"Instru��es de Correio:","")
    MenuLabel(AP_MENU_ID_AUTOTEXT_REFERENCE,	"Refer�ncia:",		"")
    MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION,	"Sauda��es:",		"")
    MenuLabel(AP_MENU_ID_AUTOTEXT_SUBJECT,	"Assunto:",		"")
    MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL,	"Email:",		"")
    
    //  add others here 

    MenuLabel(AP_MENU_ID__BOGUS2__,		NULL,			NULL)

EndSet()
