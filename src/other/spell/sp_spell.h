
#ifndef SPELL_H
#define SPELL_H

/*
  TODO stuff we need to do for this spell module:

  eliminate all the stderr fprintfs
  rip out the support for ICHAR_IS_CHAR
*/

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _sp_suggestions {
   int count;
   short *score;
   short **word;
} sp_suggestions;
   
int SpellCheckInit(char *hashname);
void SpellCheckCleanup(void);
int SpellCheckNWord16(const unsigned short *word16, int length);
int SpellCheckWord16(unsigned short  *word16);
int SpellCheckSuggestNWord16(const unsigned short *word16, int length, sp_suggestions *sg);
int SpellCheckSuggestWord16(unsigned short  *word16, sp_suggestions *sg);
int SpellCheckWord(char *word);
int SpellCheckSuggestWord(char *word, sp_suggestions *sg);  

#ifdef __cplusplus
}
#endif
	
#endif /* SPELL_H */
