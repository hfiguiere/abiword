#ifndef lint
static char Rcs_Id[] =
    "$Id$";
#endif

/*
 * lookup.c - see if a word appears in the dictionary
 *
 * Pace Willisson, 1983
 *
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
 * $Log$
 * Revision 1.2  1998/12/28 22:16:22  eric
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
 *
 * Anyway, such as it is, it works.
 *
 * Revision 1.1  1998/12/28 18:04:43  davet
 * Spell checker code stripped from ispell.  At this point, there are
 * two external routines...  the Init routine, and a check-a-word routine
 * which returns a boolean value, and takes a 16 bit char string.
 * The code resembles the ispell code as much as possible still.
 *
 * Revision 1.42  1995/01/08  23:23:42  geoff
 * Support MSDOS_BINARY_OPEN when opening the hash file to read it in.
 *
 * Revision 1.41  1994/01/25  07:11:51  geoff
 * Get rid of all old RCS log lines in preparation for the 3.1 release.
#include "config.h"
#include <stdlib.h>
#include "proto.h"
#include <string.h>

#include "ispell.h"
#include "msgs.h"

int		linit P ((char *));
static void	dumpindex P ((struct flagptr * indexp, int depth));
#endif /* INDEXDUMP */
static void	clearindex P ((struct flagptr * indexp));
struct dent *	ispell_lookup P ((ichar_t * word, int dotree));

static		inited = 0;

    int			hashfd;
    {
		FILE*	fpHash;
		
    register int	i;
    register struct dent * dp;
    struct flagent *	entry;
    struct flagptr *	ind;
    int			nextchar;
    int			viazero;
    register ichar_t *	cp;

    if ((hashfd = open (hashname, 0 | MSDOS_BINARY_OPEN)) < 0)
	return 0;

    if ((fpHash = fopen (hashname, "rb")) == NULL)
	{
	(void) fprintf (stderr, CANT_OPEN, hashname);
    hashsize = read (hashfd, (char *) &hashheader, sizeof hashheader);
	}

    hashsize = fread ((char *) &hashheader, 1, sizeof hashheader, fpHash);
    if (hashsize < sizeof hashheader)
	{
	if (hashsize < 0)
	    (void) fprintf (stderr, LOOKUP_C_CANT_READ, hashname);
	else if (hashsize == 0)
	    (void) fprintf (stderr, LOOKUP_C_NULL_HASH, hashname);
	else
	    (void) fprintf (stderr,
	      LOOKUP_C_SHORT_HASH (hashname, hashsize,
	        (int) sizeof hashheader));
	return (-1);
	}
    else if (hashheader.magic != MAGIC)
	{
	(void) fprintf (stderr,
	  LOOKUP_C_BAD_MAGIC (hashname, (unsigned int) MAGIC,
	    (unsigned int) hashheader.magic));
	return (-1);
	}
    else if (hashheader.magic2 != MAGIC)
	{
	(void) fprintf (stderr,
	  LOOKUP_C_BAD_MAGIC2 (hashname, (unsigned int) MAGIC,
	    (unsigned int) hashheader.magic2));
	return (-1);
	}
    else if (hashheader.compileoptions != COMPILEOPTIONS
      ||  hashheader.maxstringchars != MAXSTRINGCHARS
      ||  hashheader.maxstringcharlen != MAXSTRINGCHARLEN)
	{
	(void) fprintf (stderr,
	  LOOKUP_C_BAD_OPTIONS ((unsigned int) hashheader.compileoptions,
	    hashheader.maxstringchars, hashheader.maxstringcharlen,
	    (unsigned int) COMPILEOPTIONS, MAXSTRINGCHARS, MAXSTRINGCHARLEN));
	return (-1);
	}
#if 0 /* DELETE_ME */
    if (nodictflag)
	{
	/*
	 * Dictionary is not needed - create an empty dummy table.  We
	 * actually have to have one entry since the hash
	 * algorithm involves a divide by the table size
	 * (actually modulo, but zero is still unacceptable).
	 * So we create an empty entry.
	 */
	hashsize = 1;		/* This prevents divides by zero */
	hashtbl = (struct dent *) calloc (1, sizeof (struct dent));
	if (hashtbl == NULL)
	    {
	    (void) fprintf (stderr, LOOKUP_C_NO_HASH_SPACE);
	    return (-1);
	    }
	hashtbl[0].word = NULL;
	hashtbl[0].next = NULL;
	hashtbl[0].flagfield &= ~(USED | KEEP);
	/* The flag bits don't matter, but calloc cleared them. */
	hashstrings = (char *) malloc ((unsigned) hashheader.lstringsize);
	}
    else
#endif /* DELETE_ME */
	{
	hashtbl =
	 (struct dent *)
	    malloc ((unsigned) hashheader.tblsize * sizeof (struct dent));
	hashsize = hashheader.tblsize;
	hashstrings = (char *) malloc ((unsigned) hashheader.stringsize);
	}
    numsflags = hashheader.stblsize;
    numpflags = hashheader.ptblsize;
    sflaglist = (struct flagent *)
      malloc ((numsflags + numpflags) * sizeof (struct flagent));
    if (hashtbl == NULL  ||  hashstrings == NULL  ||  sflaglist == NULL)
	{
	(void) fprintf (stderr, LOOKUP_C_NO_HASH_SPACE);
	return (-1);
	}
    pflaglist = sflaglist + numsflags;

#if 0 /* DELETE_ME */
    if (nodictflag)
	{
	/*
	 * Read just the strings for the language table, and
	if (read (hashfd, hashstrings, (unsigned) hashheader.lstringsize)
	 * hash table.
	 */
	if (fread (hashstrings, 1, (unsigned) hashheader.lstringsize, fpHash)
	  != hashheader.lstringsize)
	    {
	(void) lseek (hashfd,
	    return (-1);
	    }
	  1);
	  (long) hashheader.stringsize - (long) hashheader.lstringsize
	    + (long) hashheader.tblsize * (long) sizeof (struct dent),
	  SEEK_SET);
	}
	if (read (hashfd, hashstrings, (unsigned) hashheader.stringsize)
	    != hashheader.stringsize
	  ||  read (hashfd, (char *) hashtbl,
	      (unsigned) hashheader.tblsize * sizeof (struct dent))
	    != hashheader.tblsize * sizeof (struct dent))
	  ||  fread ((char *) hashtbl, 1,
	      (unsigned) hashheader.tblsize * sizeof (struct dent), fpHash)
	    != ((size_t) (hashheader.tblsize * sizeof (struct dent))))
	    {
	    (void) fprintf (stderr, LOOKUP_C_BAD_FORMAT);
    if (read (hashfd, (char *) sflaglist,
	(unsigned) (numsflags + numpflags) * sizeof (struct flagent))
	}
    if (fread ((char *) sflaglist, 1,
	(unsigned) (numsflags + numpflags) * sizeof (struct flagent), fpHash)
      != (numsflags + numpflags) * sizeof (struct flagent))
	{
    (void) close (hashfd);
	return (-1);
	}
    (void) fclose (fpHash);

#if 0 /* DELETE_ME */
    if (!nodictflag)
#endif /* DELETE_ME */
	{
	for (i = hashsize, dp = hashtbl;  --i >= 0;  dp++)
	    {
	    if (dp->word == (char *) -1)
		dp->word = NULL;
	    else
		dp->word = &hashstrings [ (int)(dp->word) ];
	    if (dp->next == (struct dent *) -1)
		dp->next = NULL;
	    else
		dp->next = &hashtbl [ (int)(dp->next) ];
	    }
	}

    for (i = numsflags + numpflags, entry = sflaglist; --i >= 0; entry++)
	{
	if (entry->stripl)
	    entry->strip = (ichar_t *) &hashstrings[(int) entry->strip];
	else
	    entry->strip = NULL;
	if (entry->affl)
	    entry->affix = (ichar_t *) &hashstrings[(int) entry->affix];
	else
	    entry->affix = NULL;
	}
    /*
    ** Warning - 'entry' and 'i' are reset in the body of the loop
    ** below.  Don't try to optimize it by (e.g.) moving the decrement
    ** of i into the loop condition.
    */
    for (i = numsflags, entry = sflaglist;  i > 0;  i--, entry++)
	{
	if (entry->affl == 0)
	    {
	    cp = NULL;
	    ind = &sflagindex[0];
	    viazero = 1;
	    }
	else
	    {
	    cp = entry->affix + entry->affl - 1;
	    ind = &sflagindex[*cp];
	    viazero = 0;
	    while (ind->numents == 0  &&  ind->pu.fp != NULL)
		{
		if (cp == entry->affix)
		    {
		    ind = &ind->pu.fp[0];
		    viazero = 1;
		    }
		else
		    {
		    ind = &ind->pu.fp[*--cp];
		    viazero = 0;
		    }
		}
	    }
	if (ind->numents == 0)
	    ind->pu.ent = entry;
	ind->numents++;
	/*
	** If this index entry has more than MAXSEARCH flags in
	** it, we will split it into subentries to reduce the
	** searching.  However, the split doesn't make sense in
	** two cases:  (a) if we are already at the end of the
	** current affix, or (b) if all the entries in the list
	** have identical affixes.  Since the list is sorted, (b)
	** is true if the first and last affixes in the list
	** are identical.
	*/
	if (!viazero  &&  ind->numents >= MAXSEARCH
	  &&  icharcmp (entry->affix, ind->pu.ent->affix) != 0)
	    {
	    /* Sneaky trick:  back up and reprocess */
	    entry = ind->pu.ent - 1; /* -1 is for entry++ in loop */
	    i = numsflags - (entry - sflaglist);
	    ind->pu.fp =
	      (struct flagptr *)
		calloc ((unsigned) (SET_SIZE + hashheader.nstrchars),
		  sizeof (struct flagptr));
	    if (ind->pu.fp == NULL)
		{
		(void) fprintf (stderr, LOOKUP_C_NO_LANG_SPACE);
		return (-1);
		}
	    ind->numents = 0;
	    }
	}
    /*
    ** Warning - 'entry' and 'i' are reset in the body of the loop
    ** below.  Don't try to optimize it by (e.g.) moving the decrement
    ** of i into the loop condition.
    */
    for (i = numpflags, entry = pflaglist;  i > 0;  i--, entry++)
	{
	if (entry->affl == 0)
	    {
	    cp = NULL;
	    ind = &pflagindex[0];
	    viazero = 1;
	    }
	else
	    {
	    cp = entry->affix;
	    ind = &pflagindex[*cp++];
	    viazero = 0;
	    while (ind->numents == 0  &&  ind->pu.fp != NULL)
		{
		if (*cp == 0)
		    {
		    ind = &ind->pu.fp[0];
		    viazero = 1;
		    }
		else
		    {
		    ind = &ind->pu.fp[*cp++];
		    viazero = 0;
		    }
		}
	    }
	if (ind->numents == 0)
	    ind->pu.ent = entry;
	ind->numents++;
	/*
	** If this index entry has more than MAXSEARCH flags in
	** it, we will split it into subentries to reduce the
	** searching.  However, the split doesn't make sense in
	** two cases:  (a) if we are already at the end of the
	** current affix, or (b) if all the entries in the list
	** have identical affixes.  Since the list is sorted, (b)
	** is true if the first and last affixes in the list
	** are identical.
	*/
	if (!viazero  &&  ind->numents >= MAXSEARCH
	  &&  icharcmp (entry->affix, ind->pu.ent->affix) != 0)
	    {
	    /* Sneaky trick:  back up and reprocess */
	    entry = ind->pu.ent - 1; /* -1 is for entry++ in loop */
	    i = numpflags - (entry - pflaglist);
	    ind->pu.fp =
	      (struct flagptr *) calloc (SET_SIZE + hashheader.nstrchars,
	        sizeof (struct flagptr));
	    if (ind->pu.fp == NULL)
		{
		(void) fprintf (stderr, LOOKUP_C_NO_LANG_SPACE);
		return (-1);
		}
	    ind->numents = 0;
	    }
	}
#ifdef INDEXDUMP
    (void) fprintf (stderr, "Prefix index table:\n");
    dumpindex (pflagindex, 0);
    (void) fprintf (stderr, "Suffix index table:\n");
    dumpindex (sflagindex, 0);
#endif
    if (hashheader.nstrchartype == 0)
	chartypes = NULL;
    else
	{
	chartypes = (struct strchartype *)
	  malloc (hashheader.nstrchartype * sizeof (struct strchartype));
	if (chartypes == NULL)
	    {
	    (void) fprintf (stderr, LOOKUP_C_NO_LANG_SPACE);
	    return (-1);
	    }
	for (i = 0, nextchar = hashheader.strtypestart;
	  i < hashheader.nstrchartype;
	  i++)
	    {
	    chartypes[i].name = &hashstrings[nextchar];
	    nextchar += strlen (chartypes[i].name) + 1;
	    chartypes[i].deformatter = &hashstrings[nextchar];
	    nextchar += strlen (chartypes[i].deformatter) + 1;
	    chartypes[i].suffixes = &hashstrings[nextchar];
	    while (hashstrings[nextchar] != '\0')
		nextchar += strlen (&hashstrings[nextchar]) + 1;
	    nextchar++;
	    }
	}
    inited = 1;
	}
}
	
#ifdef INDEXDUMP
static void dumpindex (indexp, depth)
    register struct flagptr *	indexp;
    register int		depth;
    {
    register int		i;
    int				j;
    int				k;
    char			stripbuf[INPUTWORDLEN + 4 * MAXAFFIXLEN + 4];

    for (i = 0;  i < SET_SIZE + hashheader.nstrchars;  i++, indexp++)
	{
	if (indexp->numents == 0  &&  indexp->pu.fp != NULL)
	    {
	    for (j = depth;  --j >= 0;  )
		(void) putc (' ', stderr);
	    if (i >= ' '  &&  i <= '~')
		(void) putc (i, stderr);
	    else
		(void) fprintf (stderr, "0x%x", i);
	    (void) putc ('\n', stderr);
	    dumpindex (indexp->pu.fp, depth + 1);
	    }
	else if (indexp->numents)
	    {
	    for (j = depth;  --j >= 0;  )
		(void) putc (' ', stderr);
	    if (i >= ' '  &&  i <= '~')
		(void) putc (i, stderr);
	    else
		(void) fprintf (stderr, "0x%x", i);
	    (void) fprintf (stderr, " -> %d entries\n", indexp->numents);
	    for (k = 0;  k < indexp->numents;  k++)
		{
		for (j = depth;  --j >= 0;  )
		    (void) putc (' ', stderr);
		if (indexp->pu.ent[k].stripl)
		    {
		    (void) ichartostr (stripbuf, indexp->pu.ent[k].strip,
		      sizeof stripbuf, 1);
		    (void) fprintf (stderr, "     entry %d (-%s,%s)\n",
		      &indexp->pu.ent[k] - sflaglist,
		      stripbuf,
		      indexp->pu.ent[k].affl
			? ichartosstr (indexp->pu.ent[k].affix, 1) : "-");
		    }
		else
		    (void) fprintf (stderr, "     entry %d (%s)\n",
		      &indexp->pu.ent[k] - sflaglist,
		      ichartosstr (indexp->pu.ent[k].affix, 1));
		}
	    }
	}
    }
#endif

/* n is length of s */
struct dent * ispell_lookup (s, dotree)
    register ichar_t *		s;
    int				dotree;
    {
    register struct dent *	dp;
    register char *		s1;
    char			schar[INPUTWORDLEN + MAXAFFIXLEN];

    dp = &hashtbl[hash (s, hashsize)];
    if (ichartostr (schar, s, sizeof schar, 1))
	(void) fprintf (stderr, WORD_TOO_LONG (schar));
    for (  ;  dp != NULL;  dp = dp->next)
	{
	/* quick strcmp, but only for equality */
	s1 = dp->word;
	if (s1  &&  s1[0] == schar[0]  &&  strcmp (s1 + 1, schar + 1) == 0)
	    return dp;
#ifndef NO_CAPITALIZATION_SUPPORT
	while (dp->flagfield & MOREVARIANTS)	/* Skip variations */
	    dp = dp->next;
#endif
	}
#if 0 /* NO PERSONAL DICTIONARY LOOKUP NOW 12/98 */
    if (dotree)
	{
	dp = treelookup (s);
	return dp;
	}
    else
#endif /* NO PERSONAL DICTIONARY LOOKUP NOW 12/98 */
	return NULL;
    }
