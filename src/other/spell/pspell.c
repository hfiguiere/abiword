/* AbiSuite
 * Copyright (C) 2000 AbiSource, Inc.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pspell/pspell.h>

#include "sp_spell.h"

#define WORD_SIZE 256 /* or whatever */

/**********************************************************************/
/*       encoding manager method hack so we can use it in C code      */
/**********************************************************************/
extern const char * xap_encoding_manager_get_language_iso_name(void);

/**********************************************************************/
/*                     Globals we'll need for pspell                  */
/**********************************************************************/
static PspellManager *spell_manager = NULL;

/*
 * Pspell's author tells me that we probably don't need these
 * Two methods because Pspell can detect and handle utf16
 * Character strings, so long as they're *double null terminated*
 * These are defined in ut_string.[cpp,h]
 */
typedef unsigned short U16;
extern char * UT_UCS_strcpy_to_char (char * dest, const U16 * src);
extern U16  * UT_UCS_strcpy_char (U16 * dest, const char * src);

/*
 * Should return 1 on success, 0 on failure
 */
int SpellCheckInit(char *unused_ispell_hashfile_name)
{
  PspellConfig *spell_config;
  PspellCanHaveError *spell_error;

  spell_config = new_pspell_config();
  pspell_config_replace(spell_config, "language-tag",
			xap_encoding_manager_get_language_iso_name());
  pspell_config_replace(spell_config, "encoding", "utf-8");

  spell_error = new_pspell_manager(spell_config);
  delete_pspell_config(spell_config);

  if(pspell_error_number(spell_error) != 0)
    {
      fprintf(stderr, "SpellCheckInit: Pspell error: %s\n",
	      pspell_error_message(spell_error));
      return 0;
    }

  spell_manager = to_pspell_manager(spell_error);
  return 1;
}

void SpellCheckCleanup(void)
{
#if 0
  /* pspell segfaults for some reason. get this fixed */
  if(spell_manager)
    delete_pspell_manager(spell_manager);
#endif
}

/*
 * These next 2 functions should return 0 if not found,
 * > 1 if found, -1 on error
 */
int SpellCheckNWord16(const unsigned short *word16, int length)
{
  unsigned char  word8[WORD_SIZE];

  /* pspell segfaults if we don't pass it a valid spell_manager */
  if (spell_manager == NULL)
      return -1;

  /* trying to spell-check a 0 length word will (rightly) cause pspell to segfault */
  if(word16 == NULL || length == 0)
      return 0;

  UT_UCS_strcpy_to_char ((char*)word8, word16);
  return pspell_manager_check(spell_manager, (char*)word8);
}

int SpellCheckSuggestNWord16(const unsigned short *word16, 
			     int length, sp_suggestions *sg)
{
  PspellStringEmulation *suggestions = NULL;
  const PspellWordList *word_list = NULL;
  const char *new_word = NULL;
  unsigned char word8[WORD_SIZE];
  int count = 0, i = 0;

  /* pspell segfaults if we don't pass it a valid spell_manager */
  if (spell_manager == NULL || sg == NULL)
      return -1;

  /* trying to spell-check a 0 length word will (rightly) cause pspell to segfault */
  if(word16 == NULL || length == 0)
    {
      sg->count = 0;
      return 0;
    }

  UT_UCS_strcpy_to_char ((char*)word8, word16);
  word_list   = pspell_manager_suggest(spell_manager, (char*)word8);
  suggestions = pspell_word_list_elements(word_list);
  count       = pspell_word_list_size(word_list);

  if(count == 0)
    {
      sg->count = 0;
      return 0;
    }

  sg->score = (short *)malloc(sizeof(short) * count);
  sg->word = (U16**)malloc(sizeof(U16**) * count);
  if (sg->score == NULL || sg->word == NULL) 
    {
      sg->count = 0;
      return 0;
    }

  while ((new_word = pspell_string_emulation_next(suggestions)) != NULL) 
    {
      int len = strlen(new_word);

      sg->word[i] = (U16*)malloc(sizeof(U16) * len + 2);
      if (sg->word[i] == NULL) 
        {
	  /* out of memory, but return what was copied so far */
	  sg->count = i;
	  return i;
        }
      
      UT_UCS_strcpy_char (sg->word[i], new_word);
      sg->score[i] = 1000;
      i++;
    }

  sg->count = count;
  return count;
}
