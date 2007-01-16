#ifndef ISPELL_CHECKER_H
#define ISPELL_CHECKER_H

#include "ispell.h"
#include "spell_manager.h"
#include "ut_path.h"
#include "ut_string_class.h"
#include "ut_vector.h"
#include "ut_hash.h"


class ISpellChecker : public SpellChecker
{
	friend class SpellManager;

public:
	ISpellChecker();
	virtual ~ISpellChecker();

	// vector of DictionaryMapping*
	virtual	UT_Vector & getMapping();
	virtual bool  doesDictionaryExist (const char * szLang);

	virtual void ignoreWord (const UT_UCSChar *toCorrect, size_t toCorrectLen);

	virtual bool isIgnored (const UT_UCSChar * pWord, size_t len) const;

private:
	ISpellChecker(const ISpellChecker&);	// no impl
	void operator=(const ISpellChecker&);	// no impl

	virtual bool _requestDictionary (const char * szLang);

	bool   loadDictionaryForLanguage ( const char * szLang );
	void   setDictionaryEncoding ( const char * hashname, const char * enc );


	virtual SpellCheckResult	_checkWord(const UT_UCSChar* word, size_t len);
	virtual UT_GenericVector<UT_UCSChar*>*	_suggestWord(const UT_UCSChar* word, size_t len);

	//
	// The member functions after this point were formerly global functions
	//  passed a context structure pointer...
	//

	void try_autodetect_charset(const UT_String & inEncoding);

	void clearIgnores ();

	UT_StringPtrMap * m_pIgnoreList;

	//
	// From ispell correct.c
	//

	int		casecmp P ((char * a, char * b, int canonical));
	void		makepossibilities P ((ichar_t * word));
	int	insert P ((ichar_t * word));
#ifndef NO_CAPITALIZATION_SUPPORT
	void	wrongcapital P ((ichar_t * word));
#endif /* NO_CAPITALIZATION_SUPPORT */
	void	wrongletter P ((ichar_t * word));
	void	extraletter P ((ichar_t * word));
	void	missingletter P ((ichar_t * word));
	void	missingspace P ((ichar_t * word));
	int		compoundgood P ((ichar_t * word, int pfxopts));
	void	transposedletter P ((ichar_t * word));
	int	ins_cap P ((ichar_t * word, ichar_t * pattern));
	int	save_cap P ((ichar_t * word, ichar_t * pattern,
			  ichar_t savearea[MAX_CAPS][INPUTWORDLEN + MAXAFFIXLEN]));
	int		ins_root_cap P ((ichar_t * word, ichar_t * pattern,
			  int prestrip, int preadd, int sufstrip, int sufadd,
			  struct dent * firstdent, struct flagent * pfxent,
			  struct flagent * sufent));
	void	save_root_cap P ((ichar_t * word, ichar_t * pattern,
			  int prestrip, int preadd, int sufstrip, int sufadd,
			  struct dent * firstdent, struct flagent * pfxent,
			  struct flagent * sufent,
			  ichar_t savearea[MAX_CAPS][INPUTWORDLEN + MAXAFFIXLEN],
			  int * nsaved));
	/*
	extern void upcase P ((ichar_t * string));
	extern void lowcase P ((ichar_t * string));
	extern ichar_t * strtosichar P ((char * in, int canonical));

	int compoundflag = COMPOUND_CONTROLLED;
	*/

	//
	// From ispell good.c
	//

	int good (ichar_t* w, int ignoreflagbits, int allhits, int pfxopts, int sfxopts);
	void chk_aff (ichar_t* word, ichar_t* ucword, int len, int ignoreflagbits, int allhits, int pfxopts, int sfxopts);
	int linit(char*);
	struct dent * ispell_lookup (ichar_t* s, int dotree);
	int strtoichar (ichar_t* out, char* in, int outlen, int canonical);
	int ichartostr (char* out, ichar_t* in, int outlen, int canonical);
	char * ichartosstr (ichar_t* in, int canonical);
	int	findfiletype (const char * name, int searchnames, int * deformatter);
	long whatcap (ichar_t* word);

	/*
	** The isXXXX macros normally only check ASCII range, and don't support
	** the character sets of other languages.  These private versions handle
	** whatever character sets have been defined in the affix files.
	*/
#ifdef lint
	extern int	myupper P ((unsigned int ch));
	extern int	mylower P ((unsigned int ch));
	extern int	myspace P ((unsigned int ch));
	extern int	iswordch P ((unsigned int ch));
	extern int	isboundarych P ((unsigned int ch));
	extern int	isstringstart P ((unsigned int ch));
	extern ichar_t	mytolower P ((unsigned int ch));
	extern ichar_t	mytoupper P ((unsigned int ch));
#else /* lint */
#if 0
#define myupper(X)	(hashheader.upperchars[(X)])
#define mylower(X)	(hashheader.lowerchars[(X)])
#define myspace(X)	(((X) > 0)  &&  ((X) < 0x80) \
				  &&  isspace((unsigned char) (X)))
#define iswordch(X)	(hashheader.wordchars[(X)])
#define isboundarych(X) (hashheader.boundarychars[(X)])
#define isstringstart(X) (hashheader.stringstarts[(unsigned char) (X)])
#define mytolower(X)	(hashheader.lowerconv[(X)])
#define mytoupper(X)	(hashheader.upperconv[(X)])
#else
	/*
		HACK: macros replaced with function implementations 
		so we could do a side-effect-g_free check for unicode
		characters which aren't in hashheader
	*/
	char myupper(ichar_t c);
	char mylower(ichar_t c);
	int myspace(ichar_t c);
	char iswordch(ichar_t c);
	char isboundarych(ichar_t c);
	char isstringstart(ichar_t c);
	ichar_t mytolower(ichar_t c);
	ichar_t mytoupper(ichar_t c);
#endif
#endif /* lint */

#ifdef ICHAR_IS_CHAR
#else

	int cap_ok (ichar_t* word, struct success* hit, int len);

	int hash (ichar_t* s, int hashtblsize);
#endif

	//
	// From ispell lookup.c
	//

	void	clearindex P ((struct flagptr * indexp));
	void     initckch P ((char *));

	void alloc_ispell_struct();
	void free_ispell_struct();

	//
	// From ispell makedent.c
	//

	int		addvheader P ((struct dent * ent));
	/*int		combinecaps P ((struct dent * hdr, struct dent * newent));
#ifndef NO_CAPITALIZATION_SUPPORT
	void	forcevheader P ((struct dent * hdrp, struct dent * oldp,
			  struct dent * newp));
#endif / * NO_CAPITALIZATION_SUPPORT * /
	int	combine_two_entries P ((struct dent * hdrp,
			  struct dent * oldp, struct dent * newp));
	int	acoversb P ((struct dent * enta, struct dent * entb));
	*/
	void		upcase P ((ichar_t * string));
	void		lowcase P ((ichar_t * string));
	void		chupcase P ((char * s));

	int		stringcharlen P ((char * bufp, int canonical));
	ichar_t *	strtosichar P ((char * in, int canonical));
	char *		printichar P ((int in));

	//
	// From ispell tgood.c
	//

	/*
	void		chk_aff P ((ichar_t * word, ichar_t * ucword, int len,
			  int ignoreflagbits, int allhits, int pfxopts, int sfxopts));
	*/
	void	pfx_list_chk P ((ichar_t * word, ichar_t * ucword,
			  int len, int optflags, int sfxopts, struct flagptr * ind,
			  int ignoreflagbits, int allhits));
	void	chk_suf P ((ichar_t * word, ichar_t * ucword, int len,
			  int optflags, struct flagent * pfxent, int ignoreflagbits,
			  int allhits));
	void	suf_list_chk P ((ichar_t * word, ichar_t * ucword, int len,
			  struct flagptr * ind, int optflags, struct flagent * pfxent,
			  int ignoreflagbits, int allhits));
	int		expand_pre P ((char * croot, ichar_t * rootword,
			  MASKTYPE mask[], int option, char * extra));
	int	pr_pre_expansion P ((char * croot, ichar_t * rootword,
			  struct flagent * flent, MASKTYPE mask[], int option,
			  char * extra));
	int		expand_suf P ((char * croot, ichar_t * rootword,
			  MASKTYPE mask[], int optflags, int option, char * extra));
	int	pr_suf_expansion P ((char * croot, ichar_t * rootword,
			  struct flagent * flent, int option, char * extra));
	void	forcelc P ((ichar_t * dst, int len));

#ifdef __cplusplus
#endif

	/* this is used for converting form unsigned short to UCS-4 */

	int deftflag;              /* NZ for TeX mode by default */
	int prefstringchar;        /* Preferred string character type */
	bool m_bSuccessfulInit;

	//
	// The members after this point were formerly global variables
	//  in the original ispell code
	//

	char *	m_BC;	/* backspace if not ^H */
	char *	m_cd;	/* clear to end of display */
	char *	m_cl;	/* clear display */
	char *	m_cm;	/* cursor movement */
	char *	m_ho;	/* home */
	char *	m_nd;	/* non-destructive space */
	char *	m_so;	/* standout */
	char *	m_se;	/* standout end */
	int	m_sg;	/* space taken by so/se */
	char *	m_ti;	/* terminal initialization sequence */
	char *	m_te;	/* terminal termination sequence */
	int	m_li;	/* lines */
	int	m_co;	/* columns */

#if 0
	int	m_contextsize;	/* number of lines of context to show */
	char	m_contextbufs[MAXCONTEXT][BUFSIZ]; /* Context of current line */
	int	m_contextoffset;	/* Offset of line start in contextbufs[0] */
	char *	m_currentchar;	/* Location in contextbufs */
#endif

	char	m_ctoken[INPUTWORDLEN + MAXAFFIXLEN]; /* Current token as char */
	ichar_t	m_itoken[INPUTWORDLEN + MAXAFFIXLEN]; /* Ctoken as ichar_t str */

#if 0
	char	m_termcap[2048];	/* termcap entry */
	char	m_termstr[2048];	/* for string values */
	char *	m_termptr;	/* pointer into termcap, used by tgetstr */
#endif

	int	m_numhits;	/* number of hits in dictionary lookups */
	struct success
			m_hits[MAX_HITS]; /* table of hits gotten in lookup */

	char *	m_hashstrings;	/* Strings in hash table */
	struct hashheader
			m_hashheader;	/* Header of hash table */
	struct dent *
			m_hashtbl;	/* Main hash table, for dictionary */
	int	m_hashsize;	/* Size of main hash table */

	char	m_hashname[PATH_MAX]; /* Name of hash table file */

	int	m_aflag;		/* NZ if -a or -A option specified */
	int	m_cflag;		/* NZ if -c (crunch) option */
	int	m_lflag;		/* NZ if -l (list) option */
	int	m_incfileflag;	/* whether xgets() acts exactly like gets() */
	int	m_nodictflag;	/* NZ if dictionary not needed */

	int	m_uerasechar;	/* User's erase character, from stty */
	int	m_ukillchar;	/* User's kill character */

	unsigned int m_laststringch; /* Number of last string character */
	int	m_defdupchar;	/* Default duplicate string type */

	int	m_numpflags;		/* Number of prefix flags in table */
	int	m_numsflags;		/* Number of suffix flags in table */
	struct flagptr m_pflagindex[SET_SIZE + MAXSTRINGCHARS];
						/* Fast index to pflaglist */
	struct flagent *	m_pflaglist;	/* Prefix flag control list */
	struct flagptr m_sflagindex[SET_SIZE + MAXSTRINGCHARS];
						/* Fast index to sflaglist */
	struct flagent *	m_sflaglist;	/* Suffix flag control list */

	struct strchartype *		/* String character type collection */
			m_chartypes;

	FILE *	m_infile;			/* File being corrected */
	FILE *	m_outfile;		/* Corrected copy of infile */

	char *	m_askfilename;		/* File specified in -f option */

	int	m_changes;		/* NZ if changes made to cur. file */
	int	m_readonly;		/* NZ if current file is readonly */
	int	m_quit;			/* NZ if we're done with this file */

#define MAXPOSSIBLE	100	/* Max no. of possibilities to generate */

	char	m_possibilities[MAXPOSSIBLE][INPUTWORDLEN + MAXAFFIXLEN];
					/* Table of possible corrections */
	int	m_pcount;		/* Count of possibilities generated */
	int	m_maxposslen;	/* Length of longest possibility */
	int	m_easypossibilities; /* Number of "easy" corrections found */
					/* ..(defined as those using legal affixes) */

	/*
	 * The following array contains a list of characters that should be tried
	 * in "missingletter."  Note that lowercase characters are omitted.
	 */
	int	m_Trynum;		/* Size of "Try" array */
	ichar_t	m_Try[SET_SIZE + MAXSTRINGCHARS];

	UT_iconv_t  m_translate_in; /* Selected translation from/to Unicode */
	UT_iconv_t  m_translate_out;
};

#endif /* ISPELL_CHECKER_H */
