 
/*
** The contents of this file are subject to the AbiSource Public
** License Version 1.0 (the "License"); you may not use this file
** except in compliance with the License. You may obtain a copy
** of the License at http://www.abisource.com/LICENSE/ 
** 
** Software distributed under the License is distributed on an
** "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
** implied. See the License for the specific language governing
** rights and limitations under the License. 
** 
** The Original Code is AbiWord.
** 
** The Initial Developer of the Original Code is AbiSource, Inc.
** Portions created by AbiSource, Inc. are Copyright (C) 1998 AbiSource, Inc. 
** All Rights Reserved. 
** 
** Contributor(s):
**  
*/

/*****************************************************************
******************************************************************
** IT IS IMPORTANT THAT THIS FILE ALLOW ITSELF TO BE INCLUDED
** MORE THAN ONE TIME.
******************************************************************
*****************************************************************/

BeginSet(EnUS)

	MenuLabel(AP_MENU_ID__BOGUS1__,		NULL,NULL,NULL)

	MenuLabel(AP_MENU_ID_FILE,			"File",	NULL, NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,		"New", NULL, "Create a new document")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,		"Open", NULL, "Open an existing document")
	MenuLabel(AP_MENU_ID_FILE_SAVE,		"Save", NULL, "Save the current document")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,	"Save As", NULL, "Save the current document under a different name")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,		NULL,NULL,NULL)

EndSet()
