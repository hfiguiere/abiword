/*
 * Copyright 1987, 1988, 1989, 1992, 1993, Geoff Kuenning, Granada Hills, CA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All modifications to the source code must be clearly marked as
 *    such.  Binary redistributions based on modified source code
 *    must be clearly marked as modified versions in the documentation
 *    and/or other materials provided with the distribution.
 * 4. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgment:
 *      This product includes software developed by Geoff Kuenning and
 *      other unpaid contributors.
 * 5. The name of Geoff Kuenning may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GEOFF KUENNING AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL GEOFF KUENNING OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Table-driven version of good.c.
 *
 * Geoff Kuenning, July 1987
 */

/*
 * $Log$
 * Revision 1.3  2002/09/13 17:20:14  mpritchett
 * Fix more warnings for Linux build
 *
 * Revision 1.2  2001/05/12 16:05:42  thomasf
 * Big pseudo changes to ispell to make it pass around a structure rather
 * than rely on all sorts of gloabals willy nilly here and there.  Also
 * fixed our spelling class to work with accepting suggestions once more.
 * This code is dirty, gross and ugly (not to mention still not supporting
 * multiple hash sized just yet) but it works on my machine and will no
 * doubt break other machines.
 *
 * Revision 1.1  2001/04/15 16:01:24  tomas_f
 * moving to spell/xp
 *
 * Revision 1.7  1999/10/20 06:03:56  sterwill
 * Changed C++-style comments to C-style comments in C code.
 *
 * Revision 1.6  1999/10/20 03:19:35  paul
 * Hacked ispell code to ignore any characters that don't fit in the lookup tables loaded from the dictionary.  It ain't pretty, but at least we don't crash there any more.
 *
 * Revision 1.5  1999/04/13 17:12:51  jeff
 * Applied "Darren O. Benham" <gecko@benham.net> spell check changes.
 * Fixed crash on Win32 with the new code.
 *
 * Revision 1.4  1998/12/29 14:55:33  eric
 *
 * I've doctored the ispell code pretty extensively here.  It is now
 * warning-free on Win32.  It also *works* on Win32 now, since I
 * replaced all the I/O calls with ANSI standard ones.
 *
 * Revision 1.4  1998/12/29 14:55:33  eric
 *
 * I've doctored the ispell code pretty extensively here.  It is now
 * warning-free on Win32.  It also *works* on Win32 now, since I
 * replaced all the I/O calls with ANSI standard ones.
 *
 * Revision 1.3  1998/12/28 23:11:30  eric
 *
 * modified spell code and integration to build on Windows.
 * This is still a hack.
 *
 * Actually, it doesn't yet WORK on Windows.  It just builds.
 * SpellCheckInit is failing for some reason.
 *
 * Revision 1.2  1998/12/28 22:16:22  eric
 *
 * These changes begin to incorporate the spell checker into AbiWord.  Most
 * of this is a hack.
 *
 * 1.  added other/spell to the -I list in config/abi_defs
 * 2.  replaced other/spell/Makefile with one which is more like
 * 	our build system.
 * 3.  added other/spell to other/Makefile so that the build will now
 * 	dive down and build the spell check library.
 * 4.  added the AbiSpell library to the Makefiles in wp/main
 * 5.  added a call to SpellCheckInit in wp/main/unix/UnixMain.cpp.
 * 	This call is a HACK and should be replaced with something
 * 	proper later.
 * 6.  added code to fv_View.cpp as follows:
 * 	whenever you double-click on a word, the spell checker
 * 	verifies that word and prints its status to stdout.
 *
 * Caveats:
 * 1.  This will break the Windows build.  I'm going to work on fixing it
 * 	now.
 * 2.  This only works if your dictionary is in /usr/lib/ispell/american.hash.
 * 	The dictionary location is currently hard-coded.  This will be
 * 	fixed as well.
 *
 * Anyway, such as it is, it works.
 *
 * Revision 1.1  1998/12/28 18:04:43  davet
 * Spell checker code stripped from ispell.  At this point, there are
 * two external routines...  the Init routine, and a check-a-word routine
 * which returns a boolean value, and takes a 16 bit char string.
 * The code resembles the ispell code as much as possible still.
 *
 * Revision 1.32  1994/11/02  06:56:16  geoff
 * Remove the anyword feature, which I've decided is a bad idea.
 *
 * Revision 1.31  1994/10/25  05:46:25  geoff
 * Add support for the FF_ANYWORD (affix applies to all words, even if
 * flag bit isn't set) flag option.
 *
 * Revision 1.30  1994/05/24  06:23:08  geoff
 * Don't create a hit if "allhits" is clear and capitalization
 * mismatches.  This cures a bug where a word could be in the dictionary
 * and yet not found.
 *
 * Revision 1.29  1994/05/17  06:44:21  geoff
 * Add support for controlled compound formation and the COMPOUNDONLY
 * option to affix flags.
 *
 * Revision 1.28  1994/01/25  07:12:13  geoff
 * Get rid of all old RCS log lines in preparation for the 3.1 release.
 *
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "ispell.h"

/*
void		chk_aff P ((ichar_t * word, ichar_t * ucword, int len,
		  int ignoreflagbits, int allhits, int pfxopts, int sfxopts));
*/
static void	pfx_list_chk P ((FIRST_ARG(istate) ichar_t * word, ichar_t * ucword,
		  int len, int optflags, int sfxopts, struct flagptr * ind,
		  int ignoreflagbits, int allhits));
static void	chk_suf P ((FIRST_ARG(istate) ichar_t * word, ichar_t * ucword, int len,
		  int optflags, struct flagent * pfxent, int ignoreflagbits,
		  int allhits));
static void	suf_list_chk P ((FIRST_ARG(istate) ichar_t * word, ichar_t * ucword, int len,
		  struct flagptr * ind, int optflags, struct flagent * pfxent,
		  int ignoreflagbits, int allhits));
int		expand_pre P ((FIRST_ARG(istate) char * croot, ichar_t * rootword,
		  MASKTYPE mask[], int option, char * extra));
static int	pr_pre_expansion P ((FIRST_ARG(istate) char * croot, ichar_t * rootword,
		  struct flagent * flent, MASKTYPE mask[], int option,
		  char * extra));
int		expand_suf P ((FIRST_ARG(istate) char * croot, ichar_t * rootword,
		  MASKTYPE mask[], int optflags, int option, char * extra));
static int	pr_suf_expansion P ((FIRST_ARG(istate) char * croot, ichar_t * rootword,
		  struct flagent * flent, int option, char * extra));
static void	forcelc P ((FIRST_ARG(istate) ichar_t * dst, int len));

/* Check possible affixes */
void chk_aff (FIRST_ARG(istate) ichar_t *word, ichar_t *ucword, 
			  int len, int ignoreflagbits, int allhits, int pfxopts, int sfxopts)
#if 0
    ichar_t *		word;		/* Word to be checked */
    ichar_t *		ucword;		/* Upper-case-only copy of word */
    int			len;		/* The length of word/ucword */
    int			ignoreflagbits;	/* Ignore whether affix is legal */
    int			allhits;	/* Keep going after first hit */
    int			pfxopts;	/* Options to apply to prefixes */
    int			sfxopts;	/* Options to apply to suffixes */
#endif
    {
    register ichar_t *	cp;		/* Pointer to char to index on */
    struct flagptr *	ind;		/* Flag index table to test */

    pfx_list_chk (DEREF_FIRST_ARG(istate) word, ucword, len, pfxopts, sfxopts, &DEREF(istate, pflagindex[0]),
      ignoreflagbits, allhits);
    cp = ucword;
	/* HACK: bail on unrecognized chars */
	if (*cp >= (SET_SIZE + MAXSTRINGCHARS))
		return;
    ind = &DEREF(istate, pflagindex[*cp++]);
    while (ind->numents == 0  &&  ind->pu.fp != NULL)
	{
	if (*cp == 0)
	    return;
	if (ind->pu.fp[0].numents)
	    {
	    pfx_list_chk (DEREF_FIRST_ARG(istate) word, ucword, len, pfxopts, sfxopts, &ind->pu.fp[0],
	      ignoreflagbits, allhits);
	    if (DEREF(istate, numhits)  &&  !allhits  &&  /* !cflag  && */  !ignoreflagbits)
		return;
	    }
	/* HACK: bail on unrecognized chars */
	if (*cp >= (SET_SIZE + MAXSTRINGCHARS))
		return;
	ind = &ind->pu.fp[*cp++];
	}
    pfx_list_chk (DEREF_FIRST_ARG(istate) word, ucword, len, pfxopts, sfxopts, ind, ignoreflagbits,
      allhits);
    if (DEREF(istate, numhits)  &&  !allhits  &&  /* !cflag  &&*/  !ignoreflagbits)
	return;
    chk_suf (DEREF_FIRST_ARG(istate) word, ucword, len, sfxopts, (struct flagent *) NULL,
      ignoreflagbits, allhits);
    }

/* Check some prefix flags */
static void pfx_list_chk (FIRST_ARG(istate) ichar_t *word, ichar_t *ucword, int len, int optflags, 
					int sfxopts, struct flagptr * ind, int ignoreflagbits, int allhits)
#if 0
    ichar_t *		word;		/* Word to be checked */
    ichar_t *		ucword;		/* Upper-case-only word */
    int			len;		/* The length of ucword */
    int			optflags;	/* Options to apply */
    int			sfxopts;	/* Options to apply to suffixes */
    struct flagptr *	ind;		/* Flag index table */
    int			ignoreflagbits;	/* Ignore whether affix is legal */
    int			allhits;	/* Keep going after first hit */
#endif
    {
    int			cond;		/* Condition number */
    register ichar_t *	cp;		/* Pointer into end of ucword */
    struct dent *	dent;		/* Dictionary entry we found */
    int			entcount;	/* Number of entries to process */
    register struct flagent *
			flent;		/* Current table entry */
    int			preadd;		/* Length added to tword2 as prefix */
    register int	tlen;		/* Length of tword */
    ichar_t		tword[INPUTWORDLEN + 4 * MAXAFFIXLEN + 4]; /* Tmp cpy */
    ichar_t		tword2[sizeof tword]; /* 2nd copy for ins_root_cap */

    for (flent = ind->pu.ent, entcount = ind->numents;
      entcount > 0;
      flent++, entcount--)
	{
	/*
	 * If this is a compound-only affix, ignore it unless we're
	 * looking for that specific thing.
	 */
	if ((flent->flagflags & FF_COMPOUNDONLY) != 0
	  &&  (optflags & FF_COMPOUNDONLY) == 0)
	    continue;
	/*
	 * In COMPOUND_CONTROLLED mode, the FF_COMPOUNDONLY bit must
	 * match exactly.
	 */
#if 0 /* DELETE_ME */
	if (compoundflag == COMPOUND_CONTROLLED
	  &&  ((flent->flagflags ^ optflags) & FF_COMPOUNDONLY) != 0)
	    continue;
#endif /* DELETE_ME */
	/*
	 * See if the prefix matches.
	 */
	tlen = len - flent->affl;
	if (tlen > 0
	  &&  (flent->affl == 0
	    ||  icharncmp (flent->affix, ucword, flent->affl) == 0)
	  &&  tlen + flent->stripl >= flent->numconds)
	    {
	    /*
	     * The prefix matches.  Remove it, replace it by the "strip"
	     * string (if any), and check the original conditions.
	     */
	    if (flent->stripl)
		(void) icharcpy (tword, flent->strip);
	    (void) icharcpy (tword + flent->stripl, ucword + flent->affl);
	    cp = tword;
	    for (cond = 0;  cond < flent->numconds;  cond++)
		{
		if ((flent->conds[*cp++] & (1 << cond)) == 0)
		    break;
		}
	    if (cond >= flent->numconds)
		{
		/*
		 * The conditions match.  See if the word is in the
		 * dictionary.
		 */
		tlen += flent->stripl;

		if (ignoreflagbits)
		    {
		    if ((dent = ispell_lookup (DEREF_FIRST_ARG(istate) tword, 1)) != NULL)
			{
			cp = tword2;
			if (flent->affl)
			    {
			    (void) icharcpy (cp, flent->affix);
			    cp += flent->affl;
			    *cp++ = '+';
			    }
			preadd = cp - tword2;
			(void) icharcpy (cp, tword);
			cp += tlen;
			if (flent->stripl)
			    {
			    *cp++ = '-';
			    (void) icharcpy (cp, flent->strip);
			    }
#if 0 /* Capitalization handler 12/98 */
			(void) ins_root_cap (tword2, word,
			  flent->stripl, preadd,
			  0, (cp - tword2) - tlen - preadd,
			  dent, flent, (struct flagent *) NULL);
#endif /* Capitalizaition handler 12/98 */
			}
		    }
		else if ((dent = ispell_lookup (DEREF_FIRST_ARG(istate) tword, 1)) != NULL
		  &&  TSTMASKBIT (dent->mask, flent->flagbit))
		    {
		    if (DEREF(istate, numhits) < MAX_HITS)
			{
			DEREF(istate, hits[DEREF(istate, numhits)].dictent) = dent;
			DEREF(istate, hits[DEREF(istate, numhits)].prefix) = flent;
			DEREF(istate, hits[DEREF(istate, numhits)].suffix) = NULL;
			DEREF(istate, numhits)++;
			}
		    if (!allhits)
			{
#ifndef NO_CAPITALIZATION_SUPPORT
			if (cap_ok (DEREF_FIRST_ARG(istate) word, &DEREF(istate, hits[0]), len))
			    return;
			DEREF(istate, numhits) = 0;
#else /* NO_CAPITALIZATION_SUPPORT */
			return;
#endif /* NO_CAPITALIZATION_SUPPORT */
			}
		    }
		/*
		 * Handle cross-products.
		 */
		if (flent->flagflags & FF_CROSSPRODUCT)
		    chk_suf (DEREF_FIRST_ARG(istate) word, tword, tlen, sfxopts | FF_CROSSPRODUCT,
		      flent, ignoreflagbits, allhits);
		}
	    }
	}
    }

/* Check possible suffixes */
static void chk_suf (FIRST_ARG(istate) ichar_t *word, ichar_t *ucword, 
					int len, int optflags, struct flagent *pfxent, 
					int ignoreflagbits, int allhits)
#if 0
    ichar_t *		word;		/* Word to be checked */
    ichar_t *		ucword;		/* Upper-case-only word */
    int			len;		/* The length of ucword */
    int			optflags;	/* Affix option flags */
    struct flagent *	pfxent;		/* Prefix flag entry if cross-prod */
    int			ignoreflagbits;	/* Ignore whether affix is legal */
    int			allhits;	/* Keep going after first hit */
#endif
    {
    register ichar_t *	cp;		/* Pointer to char to index on */
    struct flagptr *	ind;		/* Flag index table to test */

    suf_list_chk (DEREF_FIRST_ARG(istate) word, ucword, len, &DEREF(istate, sflagindex[0]), optflags, pfxent,
      ignoreflagbits, allhits);
    cp = ucword + len - 1;
	/* HACK: bail on unrecognized chars */
	if (*cp >= (SET_SIZE + MAXSTRINGCHARS))
		return;
    ind = &DEREF(istate, sflagindex[*cp]);
    while (ind->numents == 0  &&  ind->pu.fp != NULL)
	{
	if (cp == ucword)
	    return;
	if (ind->pu.fp[0].numents)
	    {
	    suf_list_chk (DEREF_FIRST_ARG(istate) word, ucword, len, &ind->pu.fp[0],
	      optflags, pfxent, ignoreflagbits, allhits);
	    if (DEREF(istate, numhits) != 0  &&  !allhits  &&  /* !cflag  && */  !ignoreflagbits)
		return;
	    }
	/* HACK: bail on unrecognized chars */
	if (*(cp-1) >= (SET_SIZE + MAXSTRINGCHARS))
		return;
	ind = &ind->pu.fp[*--cp];
	}
    suf_list_chk (DEREF_FIRST_ARG(istate) word, ucword, len, ind, optflags, pfxent,
      ignoreflagbits, allhits);
    }
    
static void suf_list_chk (FIRST_ARG(istate) ichar_t *word, ichar_t *ucword, 
						  int len, struct flagptr *ind, int optflags, 
						  struct flagent *pfxent, int ignoreflagbits, int allhits)
#if 0
    ichar_t *		word;		/* Word to be checked */
    ichar_t *		ucword;		/* Upper-case-only word */
    int			len;		/* The length of ucword */
    struct flagptr *	ind;		/* Flag index table */
    int			optflags;	/* Affix option flags */
    struct flagent *	pfxent;		/* Prefix flag entry if crossonly */
    int			ignoreflagbits;	/* Ignore whether affix is legal */
    int			allhits;	/* Keep going after first hit */
#endif
    {
    register ichar_t *	cp;		/* Pointer into end of ucword */
    int			cond;		/* Condition number */
    struct dent *	dent;		/* Dictionary entry we found */
    int			entcount;	/* Number of entries to process */
    register struct flagent *
			flent;		/* Current table entry */
    int			preadd;		/* Length added to tword2 as prefix */
    register int	tlen;		/* Length of tword */
    ichar_t		tword[INPUTWORDLEN + 4 * MAXAFFIXLEN + 4]; /* Tmp cpy */
    ichar_t		tword2[sizeof tword]; /* 2nd copy for ins_root_cap */

    (void) icharcpy (tword, ucword);
    for (flent = ind->pu.ent, entcount = ind->numents;
      entcount > 0;
      flent++, entcount--)
	{
	if ((optflags & FF_CROSSPRODUCT) != 0
	  &&  (flent->flagflags & FF_CROSSPRODUCT) == 0)
	    continue;
	/*
	 * If this is a compound-only affix, ignore it unless we're
	 * looking for that specific thing.
	 */
	if ((flent->flagflags & FF_COMPOUNDONLY) != 0
	  &&  (optflags & FF_COMPOUNDONLY) == 0)
	    continue;
	/*
	 * In COMPOUND_CONTROLLED mode, the FF_COMPOUNDONLY bit must
	 * match exactly.
	 */
#if 0 /*  DELETE_ME */
	if (compoundflag == COMPOUND_CONTROLLED
	  &&  ((flent->flagflags ^ optflags) & FF_COMPOUNDONLY) != 0)
	    continue;
#endif /* DELETE_ME */
	/*
	 * See if the suffix matches.
	 */
	tlen = len - flent->affl;
	if (tlen > 0
	  &&  (flent->affl == 0
	    ||  icharcmp (flent->affix, ucword + tlen) == 0)
	  &&  tlen + flent->stripl >= flent->numconds)
	    {
	    /*
	     * The suffix matches.  Remove it, replace it by the "strip"
	     * string (if any), and check the original conditions.
	     */
	    (void) icharcpy (tword, ucword);
	    cp = tword + tlen;
	    if (flent->stripl)
		{
		(void) icharcpy (cp, flent->strip);
		tlen += flent->stripl;
		cp = tword + tlen;
		}
	    else
		*cp = '\0';
	    for (cond = flent->numconds;  --cond >= 0;  )
		{
		if ((flent->conds[*--cp] & (1 << cond)) == 0)
		    break;
		}
	    if (cond < 0)
		{
		/*
		 * The conditions match.  See if the word is in the
		 * dictionary.
		 */
		if (ignoreflagbits)
		    {
		    if ((dent = ispell_lookup (DEREF_FIRST_ARG(istate) tword, 1)) != NULL)
			{
			cp = tword2;
			if ((optflags & FF_CROSSPRODUCT)
			  &&  pfxent->affl != 0)
			    {
			    (void) icharcpy (cp, pfxent->affix);
			    cp += pfxent->affl;
			    *cp++ = '+';
			    }
			preadd = cp - tword2;
			(void) icharcpy (cp, tword);
			cp += tlen;
			if ((optflags & FF_CROSSPRODUCT)
			  &&  pfxent->stripl != 0)
			    {
			    *cp++ = '-';
			    (void) icharcpy (cp, pfxent->strip);
			    cp += pfxent->stripl;
			    }
			if (flent->stripl)
			    {
			    *cp++ = '-';
			    (void) icharcpy (cp, flent->strip);
			    cp += flent->stripl;
			    }
			if (flent->affl)
			    {
			    *cp++ = '+';
			    (void) icharcpy (cp, flent->affix);
			    cp += flent->affl;
			    }
#if 0 /* Capitalization handler 12/98 */
			(void) ins_root_cap (tword2, word,
			  (optflags & FF_CROSSPRODUCT) ? pfxent->stripl : 0,
			  preadd,
			  flent->stripl, (cp - tword2) - tlen - preadd,
			  dent, pfxent, flent);
#endif /* Capitalization handler 12/98 */
			}
		    }
		else if ((dent = ispell_lookup (DEREF_FIRST_ARG(istate) tword, 1)) != NULL
		  &&  TSTMASKBIT (dent->mask, flent->flagbit)
		  &&  ((optflags & FF_CROSSPRODUCT) == 0
		    || TSTMASKBIT (dent->mask, pfxent->flagbit)))
		    {
		    if (DEREF(istate, numhits) < MAX_HITS)
			{
			DEREF(istate, hits[DEREF(istate, numhits)].dictent) = dent;
			DEREF(istate, hits[DEREF(istate, numhits)].prefix) = pfxent;
			DEREF(istate, hits[DEREF(istate, numhits)].suffix) = flent;
			DEREF(istate, numhits)++;
			}
		    if (!allhits)
			{
#ifndef NO_CAPITALIZATION_SUPPORT
			if (cap_ok (DEREF_FIRST_ARG(istate) word, &DEREF(istate, hits[0]), len))
			    return;
			DEREF(istate, numhits) = 0;
#else /* NO_CAPITALIZATION_SUPPORT */
			return;
#endif /* NO_CAPITALIZATION_SUPPORT */
			}
		    }
		}
	    }
	}
    }

/*
 * Expand a dictionary prefix entry
 */
int expand_pre (FIRST_ARG(istate) char *croot, ichar_t *rootword, MASKTYPE mask[], 
				int option, char *extra)
#if 0
    char *			croot;		/* Char version of rootword */
    ichar_t *			rootword;	/* Root word to expand */
    register MASKTYPE		mask[];		/* Mask bits to expand on */
    int				option;		/* Option, see expandmode */
    char *			extra;		/* Extra info to add to line */
#endif
    {
    int				entcount;	/* No. of entries to process */
    int				explength;	/* Length of expansions */
    register struct flagent *
				flent;		/* Current table entry */

    for (flent = DEREF(istate, pflaglist), entcount = DEREF(istate, numpflags), explength = 0;
      entcount > 0;
      flent++, entcount--)
	{
	if (TSTMASKBIT (mask, flent->flagbit))
	    explength +=
	      pr_pre_expansion (DEREF_FIRST_ARG(istate) croot, rootword, flent, mask, option, extra);
	}
    return explength;
    }

/* Print a prefix expansion */
static int pr_pre_expansion (FIRST_ARG(istate)  char *croot, ichar_t *rootword, 
							struct flagent *flent, MASKTYPE mask[], int option, 
							char *extra)
#if 0
    char *			croot;		/* Char version of rootword */
    register ichar_t *		rootword;	/* Root word to expand */
    register struct flagent *	flent;		/* Current table entry */
    MASKTYPE			mask[];		/* Mask bits to expand on */
    int				option;		/* Option, see	expandmode */
    char *			extra;		/* Extra info to add to line */
#endif
    {
    int				cond;		/* Current condition number */
    register ichar_t *		nextc;		/* Next case choice */
    int				tlen;		/* Length of tword */
    ichar_t			tword[INPUTWORDLEN + MAXAFFIXLEN]; /* Temp */

    tlen = icharlen (rootword);
    if (flent->numconds > tlen)
	return 0;
    tlen -= flent->stripl;
    if (tlen <= 0)
	return 0;
    tlen += flent->affl;
    for (cond = 0, nextc = rootword;  cond < flent->numconds;  cond++)
	{
	if ((flent->conds[mytoupper (DEREF_FIRST_ARG(istate) *nextc++)] & (1 << cond)) == 0)
	    return 0;
	}
    /*
     * The conditions are satisfied.  Copy the word, add the prefix,
     * and make it the proper case.   This code is carefully written
     * to match that ins_cap and cap_ok.  Note that the affix, as
     * inserted, is uppercase.
     *
     * There is a tricky bit here:  if the root is capitalized, we
     * want a capitalized result.  If the root is followcase, however,
     * we want to duplicate the case of the first remaining letter
     * of the root.  In other words, "Loved/U" should generate "Unloved",
     * but "LOved/U" should generate "UNLOved" and "lOved/U" should
     * produce "unlOved".
     */
    if (flent->affl)
	{
	(void) icharcpy (tword, flent->affix);
	nextc = tword + flent->affl;
	}
    (void) icharcpy (nextc, rootword + flent->stripl);
    if (myupper (DEREF_FIRST_ARG(istate) rootword[0]))
	{
	/* We must distinguish followcase from capitalized and all-upper */
	for (nextc = rootword + 1;  *nextc;  nextc++)
	    {
	    if (!myupper (DEREF_FIRST_ARG(istate) *nextc))
		break;
	    }
	if (*nextc)
	    {
	    /* It's a followcase or capitalized word.  Figure out which. */
	    for (  ;  *nextc;  nextc++)
		{
		if (myupper (DEREF_FIRST_ARG(istate) *nextc))
		    break;
		}
	    if (*nextc)
		{
		/* It's followcase. */
		if (!myupper (DEREF_FIRST_ARG(istate) tword[flent->affl]))
		    forcelc (DEREF_FIRST_ARG(istate) tword, flent->affl);
		}
	    else
		{
		/* It's capitalized */
		forcelc (DEREF_FIRST_ARG(istate) tword + 1, tlen - 1);
		}
	    }
	}
    else
	{
	/* Followcase or all-lower, we don't care which */
	if (!myupper (DEREF_FIRST_ARG(istate) *nextc))
	    forcelc (DEREF_FIRST_ARG(istate) tword, flent->affl);
	}
    if (option == 3)
	(void) printf ("\n%s", croot);
    if (option != 4)
	(void) printf (" %s%s", ichartosstr (DEREF_FIRST_ARG(istate) tword, 1), extra);
    if (flent->flagflags & FF_CROSSPRODUCT)
	return tlen
	  + expand_suf (DEREF_FIRST_ARG(istate) croot, tword, mask, FF_CROSSPRODUCT, option, extra);
    else
	return tlen;
    }

/*
 * Expand a dictionary suffix entry
 */
int expand_suf (FIRST_ARG(istate) char *croot, ichar_t *rootword, MASKTYPE mask[], 
				int optflags, int option, char *extra)
#if 0
    char *			croot;		/* Char version of rootword */
    ichar_t *			rootword;	/* Root word to expand */
    register MASKTYPE		mask[];		/* Mask bits to expand on */
    int				optflags;	/* Affix option flags */
    int				option;		/* Option, see expandmode */
    char *			extra;		/* Extra info to add to line */
#endif
    {
    int				entcount;	/* No. of entries to process */
    int				explength;	/* Length of expansions */
    register struct flagent *
				flent;		/* Current table entry */

    for (flent = DEREF(istate, sflaglist), entcount = DEREF(istate, numsflags), explength = 0;
      entcount > 0;
      flent++, entcount--)
	{
	if (TSTMASKBIT (mask, flent->flagbit))
	    {
	    if ((optflags & FF_CROSSPRODUCT) == 0
	      ||  (flent->flagflags & FF_CROSSPRODUCT))
		explength +=
		  pr_suf_expansion (DEREF_FIRST_ARG(istate) croot, rootword, flent, option, extra);
	    }
	}
    return explength;
    }

/* Print a suffix expansion */
static int pr_suf_expansion (FIRST_ARG(istate) char *croot, ichar_t *rootword, 
							struct flagent *flent, int option, char *extra)
#if 0
    char *			croot;		/* Char version of rootword */
    register ichar_t *		rootword;	/* Root word to expand */
    register struct flagent *	flent;		/* Current table entry */
    int				option;		/* Option, see expandmode */
    char *			extra;		/* Extra info to add to line */
#endif
    {
    int				cond;		/* Current condition number */
    register ichar_t *		nextc;		/* Next case choice */
    int				tlen;		/* Length of tword */
    ichar_t			tword[INPUTWORDLEN + MAXAFFIXLEN]; /* Temp */

    tlen = icharlen (rootword);
    cond = flent->numconds;
    if (cond > tlen)
	return 0;
    if (tlen - flent->stripl <= 0)
	return 0;
    for (nextc = rootword + tlen;  --cond >= 0;  )
	{
	if ((flent->conds[mytoupper (DEREF_FIRST_ARG(istate) *--nextc)] & (1 << cond)) == 0)
	    return 0;
	}
    /*
     * The conditions are satisfied.  Copy the word, add the suffix,
     * and make it match the case of the last remaining character of the
     * root.  Again, this code carefully matches ins_cap and cap_ok.
     */
    (void) icharcpy (tword, rootword);
    nextc = tword + tlen - flent->stripl;
    if (flent->affl)
	{
	(void) icharcpy (nextc, flent->affix);
	if (!myupper (DEREF_FIRST_ARG(istate) nextc[-1]))
	    forcelc (DEREF_FIRST_ARG(istate) nextc, flent->affl);
	}
    else
	*nextc = 0;
    if (option == 3)
	(void) printf ("\n%s", croot);
    if (option != 4)
	(void) printf (" %s%s", ichartosstr (DEREF_FIRST_ARG(istate) tword, 1), extra);
    return tlen + flent->affl - flent->stripl;
    }

static void forcelc (FIRST_ARG(istate) ichar_t *dst, int len)			/* Force to lowercase */
#if 0
    register ichar_t *		dst;		/* Destination to modify */
    register int		len;		/* Length to copy */
#endif
    {

    for (  ;  --len >= 0;  dst++)
	*dst = mytolower (DEREF_FIRST_ARG(istate) *dst);
    }
