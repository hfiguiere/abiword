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

/* Portuguese translations provided by Rui M. Silva <rms@ssi.aaum.pt> */

/*****************************************************************
******************************************************************
** IT IS IMPORTANT THAT THIS FILE ALLOW ITSELF TO BE INCLUDED
** MORE THAN ONE TIME.
******************************************************************
*****************************************************************/

// We use the Win32 '&' character to denote a keyboard accelerator on a menu item.
// If your platform doesn't have a way to do accelerators or uses a different
// character, remove or change the '&' in your menu constructor code.

// If the third argument is UT_TRUE, then this is the fall-back for
// this language (named in the first argument).

BeginSet(pt,PT,UT_TRUE)

    MenuLabel(AP_MENU_ID__BOGUS1__,         NULL,               NULL)

    //       (id,                           szLabel,            szStatusMsg)

    MenuLabel(AP_MENU_ID_FILE,              "&Ficheiro",         NULL)
    MenuLabel(AP_MENU_ID_FILE_NEW,          "&Novo",         "Criar um novo documento")    
    MenuLabel(AP_MENU_ID_FILE_OPEN,         "&Abrir",          "Abrir um documento")
    MenuLabel(AP_MENU_ID_FILE_CLOSE,        "Fe&char",          "Fechar o documento")
    MenuLabel(AP_MENU_ID_FILE_SAVE,         "&Gravar",     "Gravar documento")
    MenuLabel(AP_MENU_ID_FILE_SAVEAS,       "Gra&var como",    "Gravar ficheiro atruindo um nome")
    MenuLabel(AP_MENU_ID_FILE_PAGESETUP,    "&Paginamento",    "Modificar o paginamento")
    MenuLabel(AP_MENU_ID_FILE_PRINT,        "&Imprimir",        "Imprimir o documento")
    MenuLabel(AP_MENU_ID_FILE_RECENT_1,     "&1 %s",            "�ltimo ficheiro aberto")
    MenuLabel(AP_MENU_ID_FILE_RECENT_2,     "&2 %s",            "Segundo ficheiro aberto")
    MenuLabel(AP_MENU_ID_FILE_RECENT_3,     "&3 %s",            "Terceiro ficheiro aberto")
    MenuLabel(AP_MENU_ID_FILE_RECENT_4,     "&4 %s",            "Quarto ficheiro aberto")
    MenuLabel(AP_MENU_ID_FILE_RECENT_5,     "&5 %s",            "Quinto ficheiro aberto")
    MenuLabel(AP_MENU_ID_FILE_RECENT_6,     "&6 %s",            "Sexto ficheiro aberto")
    MenuLabel(AP_MENU_ID_FILE_RECENT_7,     "&7 %s",            "S�ptimo ficheiro aberto")
    MenuLabel(AP_MENU_ID_FILE_RECENT_8,     "&8 %s",            "Oitavo ficheiro aberto")
    MenuLabel(AP_MENU_ID_FILE_RECENT_9,     "&9 %s",            "Nono ficheiro aberto")
    MenuLabel(AP_MENU_ID_FILE_EXIT,         "&Sair",         "Fechar todas as janelas e sair")

    MenuLabel(AP_MENU_ID_EDIT,              "&Editar",         NULL)
    MenuLabel(AP_MENU_ID_EDIT_UNDO,         "&Anular",         "Anular a �ltima opera��o")
    MenuLabel(AP_MENU_ID_EDIT_REDO,         "&Repetir",         "Repetir a �ltima opera��o")
    MenuLabel(AP_MENU_ID_EDIT_CUT,          "Cortar",          "Cortar a selec��o")
    MenuLabel(AP_MENU_ID_EDIT_COPY,         "Copiar",          "Copiar a selec��o")
    MenuLabel(AP_MENU_ID_EDIT_PASTE,        "Colar",          "Colar a selec��o")
    MenuLabel(AP_MENU_ID_EDIT_CLEAR,        "Apa&gar",         "Apagar a selec��o")
    MenuLabel(AP_MENU_ID_EDIT_SELECTALL,    "&Seleccio&nar &tudo",    "Seleccionar o documento inteiro")
    MenuLabel(AP_MENU_ID_EDIT_FIND,         "&Procurar",      "Procurar texto")
    MenuLabel(AP_MENU_ID_EDIT_REPLACE,      "&Substituir",       "Substituir no documento")
    MenuLabel(AP_MENU_ID_EDIT_GOTO,         "&Ir para",       "Deslocar o cursor para")
    
    MenuLabel(AP_MENU_ID_VIEW,              "&Ver",       NULL)
    MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,     "&Barras de utilit�rios",  NULL)
    MenuLabel(AP_MENU_ID_VIEW_TB_STD,       "&Standard",        "Barra standard")
    MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,    "&Formata��o",   "Barra de formata��o")
    MenuLabel(AP_MENU_ID_VIEW_RULER,        "&R�guas",          "Ver ou omitir as r�guas")
    MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,    "Barra de &mensagens",    "Ver ou omitir a barra de mensagens")
    MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,     "&Mostrar �",          "Mostrar/Esconder caracteres n�o imprim�veis")
    MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,     "&Cabe�alhos e Rodap�s",    "Editar os cabe�alhos e rodap�s")
    MenuLabel(AP_MENU_ID_VIEW_ZOOM,         "&Zoom",            "Aproximar ou afastar do texto")

    MenuLabel(AP_MENU_ID_INSERT,            "&Inserir",      NULL)
    MenuLabel(AP_MENU_ID_INSERT_BREAK,      "&Quebra",           "Inserir quebras de p�gina, sec��o")
    MenuLabel(AP_MENU_ID_INSERT_PAGENO,     "&N�mera��o",         "Inserir n�mera��o das p�ginas")
    MenuLabel(AP_MENU_ID_INSERT_DATETIME,   "Data e &hora",  "Inserir data e hora")
    MenuLabel(AP_MENU_ID_INSERT_FIELD,      "&Campo",          "Inserir um campo calculado")
    MenuLabel(AP_MENU_ID_INSERT_SYMBOL,     "Caracteres especiais",    "Inserir caracteres especiais")
    MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,    "Ima&gem",          "Inserir uma imagem")

    MenuLabel(AP_MENU_ID_FORMAT,            "F&ormato",         NULL)
    MenuLabel(AP_MENU_ID_FMT_FONT,          "&Tipo",         "Mudar o tipo de letra")
    MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,     "&Par�grafo",     "Mudar a defini��o do par�grafo")
    MenuLabel(AP_MENU_ID_FMT_BULLETS,       "&Listas",     "Formatar listas (n�o) numeradas")
    MenuLabel(AP_MENU_ID_FMT_BORDERS,       "Bordas e &Sombras",   "ajustar bordas e sombreados")
    MenuLabel(AP_MENU_ID_FMT_COLUMNS,       "C&olunas",       "Mudar o n�mero de colunas")
    MenuLabel(AP_MENU_ID_FMT_STYLE,         "&Estilo",          "Definir ou aplicar estilos a uma selec��o")
    MenuLabel(AP_MENU_ID_FMT_TABS,          "Ta&bula��o",    "Definir tabula��o")
    MenuLabel(AP_MENU_ID_FMT_BOLD,          "&Negrito",           "Engrossar texto")
    MenuLabel(AP_MENU_ID_FMT_ITALIC,        "&It�lico",       "Inclinar texto")
    MenuLabel(AP_MENU_ID_FMT_UNDERLINE,     "S&ublinhado",       "Sublinhar texto")
    MenuLabel(AP_MENU_ID_FMT_OVERLINE,      "Sob&relinhado",     "Sobrelinhar texto")
    MenuLabel(AP_MENU_ID_FMT_STRIKE,        "&Riscar",          "Riscar texto")
    MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,   "&Expoente",       "Elevar texto")
    MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,     "�&ndice",         "Indiciar texto")

    MenuLabel(AP_MENU_ID_TOOLS,        "Fe&rramentas",	NULL)
    MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,        "&Contas palavras",	"Contar palavras no documento")
    MenuLabel(AP_MENU_ID_TOOLS_SPELL,        "O&rtografia",	"Verificar ortografia")
    MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,      "&Op��es",         "Configurar as op��es")
    
    MenuLabel(AP_MENU_ID_ALIGN,             "&Alinhamento",     NULL)
    MenuLabel(AP_MENU_ID_ALIGN_LEFT,        "� &Esquerda",       "Alinhar � esquerda")
    MenuLabel(AP_MENU_ID_ALIGN_CENTER,      "&Centrar",         "Centrar texto")
    MenuLabel(AP_MENU_ID_ALIGN_RIGHT,       "� &Direita",       "Alinhar � direita")
    MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,     "&Justificar",       "Justificar texto")

    MenuLabel(AP_MENU_ID_WINDOW,            "&Janela",        NULL)
    MenuLabel(AP_MENU_ID_WINDOW_NEW,        "&Nova",    "Abrir outra janela para este documento")
    MenuLabel(AP_MENU_ID_WINDOW_1,          "&1 %s",           "Mudar para a primeira janela")
    MenuLabel(AP_MENU_ID_WINDOW_2,          "&2 %s",           "Mudar para a segunda janela")
    MenuLabel(AP_MENU_ID_WINDOW_3,          "&3 %s",           "Mudar para a terceira janela")
    MenuLabel(AP_MENU_ID_WINDOW_4,          "&4 %s",           "Mudar para a quarta janela")
    MenuLabel(AP_MENU_ID_WINDOW_5,          "&5 %s",           "Mudar para a quinta janela")
    MenuLabel(AP_MENU_ID_WINDOW_6,          "&6 %s",           "Mudar para a sexta janela")
    MenuLabel(AP_MENU_ID_WINDOW_7,          "&7 %s",           "Mudar para a septima janela")
    MenuLabel(AP_MENU_ID_WINDOW_8,          "&8 %s",           "Mudar para a oitava janela")
    MenuLabel(AP_MENU_ID_WINDOW_9,          "&9 %s",           "Mudar para a nona janela")
    MenuLabel(AP_MENU_ID_WINDOW_MORE,       "&Mais janelas",    "Mostrar lista completa de janelas")

    MenuLabel(AP_MENU_ID_HELP,              "&Ajuda",            NULL)
    MenuLabel(AP_MENU_ID_HELP_ABOUT,        "&Acerca de %s",  "Ver informa��es sobre o programa, n�mero de versao e copyrights")

    //  add others here 

    MenuLabel(AP_MENU_ID__BOGUS2__,         NULL,               NULL)

EndSet()
 


