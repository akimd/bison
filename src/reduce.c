/* Grammar reduction for Bison.
   Copyright 1988, 1989, 2000, 2001  Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   Bison is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Bison is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bison; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */


/* Reduce the grammar: Find and eliminate unreachable terminals,
   nonterminals, and productions.  David S. Bakin.  */

/* Don't eliminate unreachable terminals: They may be used by the
   user's parser.  */

#include "system.h"
#include "getargs.h"
#include "files.h"
#include "gram.h"
#include "complain.h"
#include "reduce.h"
#include "reader.h"
#include "getargs.h"

typedef unsigned *BSet;
typedef short *rule;


/* Set of all nonterminals which are not useless.  */
static BSet N;

/* Set of all rules which have no useless nonterminals in their RHS.  */
static BSet P;

/* Set of all accessible symbols.  */
static BSet V;

/* Set of symbols used to define rule precedence (so they are
   `useless', but no warning should be issued).  */
static BSet V1;

static int nuseful_productions;
static int nuseless_productions;
static int nuseful_nonterminals;
int nuseless_nonterminals;

static bool
bits_equal (BSet L, BSet R, int n)
{
  int i;

  for (i = n - 1; i >= 0; i--)
    if (L[i] != R[i])
      return FALSE;
  return TRUE;
}


static int
nbits (unsigned i)
{
  int count = 0;

  while (i != 0)
    {
      i ^= (i & ((unsigned) (-(int) i)));
      ++count;
    }
  return count;
}


static int
bits_size (BSet S, int n)
{
  int i, count = 0;

  for (i = n - 1; i >= 0; i--)
    count += nbits (S[i]);
  return count;
}

/*-------------------------------------------------------------------.
| Another way to do this would be with a set for each production and |
| then do subset tests against N0, but even for the C grammar the    |
| whole reducing process takes only 2 seconds on my 8Mhz AT.         |
`-------------------------------------------------------------------*/

static bool
useful_production (int i, BSet N0)
{
  rule r;
  short n;

  /* A production is useful if all of the nonterminals in its appear
     in the set of useful nonterminals.  */

  for (r = &ritem[rule_table[i].rhs]; *r > 0; r++)
    if (ISVAR (n = *r))
      if (!BITISSET (N0, n - ntokens))
	return FALSE;
  return TRUE;
}


/*---------------------------------------------------------.
| Remember that rules are 1-origin, symbols are 0-origin.  |
`---------------------------------------------------------*/

static void
useless_nonterminals (void)
{
  BSet Np, Ns;
  int i;

  /* N is set as built.  Np is set being built this iteration. P is
     set of all productions which have a RHS all in N.  */

  Np = XCALLOC (unsigned, WORDSIZE (nvars));

  /* The set being computed is a set of nonterminals which can derive
     the empty string or strings consisting of all terminals. At each
     iteration a nonterminal is added to the set if there is a
     production with that nonterminal as its LHS for which all the
     nonterminals in its RHS are already in the set.  Iterate until
     the set being computed remains unchanged.  Any nonterminals not
     in the set at that point are useless in that they will never be
     used in deriving a sentence of the language.

     This iteration doesn't use any special traversal over the
     productions.  A set is kept of all productions for which all the
     nonterminals in the RHS are in useful.  Only productions not in
     this set are scanned on each iteration.  At the end, this set is
     saved to be used when finding useful productions: only
     productions in this set will appear in the final grammar.  */

  while (1)
    {
      for (i = WORDSIZE (nvars) - 1; i >= 0; i--)
	Np[i] = N[i];
      for (i = 1; i <= nrules; i++)
	{
	  if (!BITISSET (P, i))
	    {
	      if (useful_production (i, N))
		{
		  SETBIT (Np, rule_table[i].lhs - ntokens);
		  SETBIT (P, i);
		}
	    }
	}
      if (bits_equal (N, Np, WORDSIZE (nvars)))
	break;
      Ns = Np;
      Np = N;
      N = Ns;
    }
  XFREE (N);
  N = Np;
}


static void
inaccessable_symbols (void)
{
  BSet Vp, Vs, Pp;
  int i;
  short t;
  rule r;

  /* Find out which productions are reachable and which symbols are
     used.  Starting with an empty set of productions and a set of
     symbols which only has the start symbol in it, iterate over all
     productions until the set of productions remains unchanged for an
     iteration.  For each production which has a LHS in the set of
     reachable symbols, add the production to the set of reachable
     productions, and add all of the nonterminals in the RHS of the
     production to the set of reachable symbols.

     Consider only the (partially) reduced grammar which has only
     nonterminals in N and productions in P.

     The result is the set P of productions in the reduced grammar,
     and the set V of symbols in the reduced grammar.

     Although this algorithm also computes the set of terminals which
     are reachable, no terminal will be deleted from the grammar. Some
     terminals might not be in the grammar but might be generated by
     semantic routines, and so the user might want them available with
     specified numbers.  (Is this true?)  However, the nonreachable
     terminals are printed (if running in verbose mode) so that the
     user can know.  */

  Vp = XCALLOC (unsigned, WORDSIZE (nsyms));
  Pp = XCALLOC (unsigned, WORDSIZE (nrules + 1));

  /* If the start symbol isn't useful, then nothing will be useful. */
  if (BITISSET (N, start_symbol - ntokens))
    {
      SETBIT (V, start_symbol);

      while (1)
	{
	  for (i = WORDSIZE (nsyms) - 1; i >= 0; i--)
	    Vp[i] = V[i];
	  for (i = 1; i <= nrules; i++)
	    {
	      if (!BITISSET (Pp, i)
		  && BITISSET (P, i)
		  && BITISSET (V, rule_table[i].lhs))
		{
		  for (r = &ritem[rule_table[i].rhs]; *r >= 0; r++)
		    if (ISTOKEN (t = *r) || BITISSET (N, t - ntokens))
		      SETBIT (Vp, t);
		  SETBIT (Pp, i);
		}
	    }
	  if (bits_equal (V, Vp, WORDSIZE (nsyms)))
	    break;
	  Vs = Vp;
	  Vp = V;
	  V = Vs;
	}
    }

  XFREE (V);
  V = Vp;

  /* Tokens 0, 1, and 2 are internal to Bison.  Consider them useful. */
  SETBIT (V, 0);		/* end-of-input token */
  SETBIT (V, 1);		/* error token */
  SETBIT (V, 2);		/* some undefined token */

  XFREE (P);
  P = Pp;

  nuseful_productions = bits_size (P, WORDSIZE (nrules + 1));
  nuseless_productions = nrules - nuseful_productions;

  nuseful_nonterminals = 0;
  for (i = ntokens; i < nsyms; i++)
    if (BITISSET (V, i))
      nuseful_nonterminals++;
  nuseless_nonterminals = nvars - nuseful_nonterminals;

  /* A token that was used in %prec should not be warned about.  */
  for (i = 1; i < nrules; i++)
    if (rule_table[i].precsym != 0)
      SETBIT (V1, rule_table[i].precsym);
}

static void
reduce_grammar_tables (void)
{
  /* This is turned off because we would need to change the numbers in
     the case statements in the actions file.

     We don't disable it via CPP so that it is still checked with the
     rest of the code, to avoid its becoming completely obsolete.

     FIXME: I think the comment above demonstrates this code must be
     turned off for *semantic* parser, not in the general case.  Try
     to understand this better --akim.  */

  if (0)
    /* remove useless productions */
    if (nuseless_productions > 0)
      {
	short np, pn, ni, pi;

	np = 0;
	ni = 0;
	for (pn = 1; pn <= nrules; pn++)
	  if (BITISSET (P, pn))
	    {
	      np++;
	      if (pn != np)
		{
		  rule_table[np].lhs   = rule_table[pn].lhs;
		  rule_table[np].line  = rule_table[pn].line;
		  rule_table[np].prec  = rule_table[pn].prec;
		  rule_table[np].assoc = rule_table[pn].assoc;
		  rule_table[np].rhs   = rule_table[pn].rhs;
		  if (rule_table[np].rhs != ni)
		    {
		      pi = rule_table[np].rhs;
		      rule_table[np].rhs = ni;
		      while (ritem[pi] >= 0)
			ritem[ni++] = ritem[pi++];
		      ritem[ni++] = -np;
		    }
		}
	      else
		{
		  while (ritem[ni++] >= 0);
		}
	    }

	ritem[ni] = 0;
	nrules -= nuseless_productions;
	nitems = ni;

	/* Is it worth it to reduce the amount of memory for the
	   grammar? Probably not.  */
      }

  /* Disable useless productions. */
  if (nuseless_productions > 0)
    {
      int pn;
      for (pn = 1; pn <= nrules; pn++)
	rule_table[pn].useful = BITISSET (P, pn);
    }
}


/*------------------------------.
| Remove useless nonterminals.  |
`------------------------------*/

static void
nonterminals_reduce (void)
{
  int i, n;
  rule r;

  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */

  short *nontermmap = XCALLOC (short, nvars) - ntokens;
  n = ntokens;
  for (i = ntokens; i < nsyms; i++)
    if (BITISSET (V, i))
      nontermmap[i] = n++;
  for (i = ntokens; i < nsyms; i++)
    if (!BITISSET (V, i))
      nontermmap[i] = n++;


  /* Shuffle elements of tables indexed by symbol number.  */
  {
    short *sassoc_sorted = XMALLOC (short, nvars) - ntokens;
    short *sprec_sorted  = XMALLOC (short, nvars) - ntokens;
    char **tags_sorted   = XMALLOC (char *, nvars) - ntokens;

    for (i = ntokens; i < nsyms; i++)
      {
	n = nontermmap[i];
	sassoc_sorted[n] = sassoc[i];
	sprec_sorted[n]  = sprec[i];
	tags_sorted[n]   = tags[i];
      }
    for (i = ntokens; i < nsyms; i++)
      {
	sassoc[i] = sassoc_sorted[i];
	sprec[i]  = sprec_sorted[i];
	tags[i]   = tags_sorted[i];
      }
    free (sassoc_sorted + ntokens);
    free (sprec_sorted + ntokens);
    free (tags_sorted + ntokens);
  }

  /* Replace all symbol numbers in valid data structures.  */

  for (i = 1; i <= nrules; i++)
    {
      rule_table[i].lhs = nontermmap[rule_table[i].lhs];
      if (ISVAR (rule_table[i].precsym))
	/* Can this happen?  */
	rule_table[i].precsym = nontermmap[rule_table[i].precsym];
    }

  for (r = ritem; *r; r++)
    if (ISVAR (*r))
      *r = nontermmap[*r];

  start_symbol = nontermmap[start_symbol];

  nsyms -= nuseless_nonterminals;
  nvars -= nuseless_nonterminals;

  free (nontermmap + ntokens);
}


/*------------------------------------------------------------------.
| Output the detailed results of the reductions.  For FILE.output.  |
`------------------------------------------------------------------*/

void
reduce_output (FILE *out)
{
  if (nuseless_nonterminals > 0)
    {
      int i;
      fprintf (out, "%s\n\n", _("Useless nonterminals:"));
      for (i = 0; i < nuseless_nonterminals; ++i)
	fprintf (out, "   %s\n", tags[nsyms + i]);
      fputs ("\n\n", out);
    }

  {
    bool b = FALSE;
    int i;
    for (i = 0; i < ntokens; i++)
      if (!BITISSET (V, i) && !BITISSET (V1, i))
	{
	  if (!b)
	    fprintf (out, "%s\n\n", _("Terminals which are not used:"));
	  b = TRUE;
	  fprintf (out, "   %s\n", tags[i]);
	}
    if (b)
      fputs ("\n\n", out);
  }

  if (nuseless_productions > 0)
    {
      int i;
      fprintf (out, "%s\n\n", _("Useless rules:"));
      for (i = 1; i <= nrules; i++)
	if (!rule_table[i].useful)
	  {
	    rule r;
	    fprintf (out, "#%-4d  ", i);
	    fprintf (out, "%s:", tags[rule_table[i].lhs]);
	    for (r = &ritem[rule_table[i].rhs]; *r >= 0; r++)
	      fprintf (out, " %s", tags[*r]);
	    fputs (";\n", out);
	  }
      fputs ("\n\n", out);
    }
}

static void
dump_grammar (FILE *out)
{
  int i;
  rule r;

  fprintf (out, "REDUCED GRAMMAR\n\n");
  fprintf (out,
	   "ntokens = %d, nvars = %d, nsyms = %d, nrules = %d, nitems = %d\n\n",
	   ntokens, nvars, nsyms, nrules, nitems);
  fprintf (out, "Variables\n---------\n\n");
  fprintf (out, "Value  Sprec  Sassoc  Tag\n");
  for (i = ntokens; i < nsyms; i++)
    fprintf (out, "%5d  %5d   %5d  %s\n", i, sprec[i], sassoc[i], tags[i]);
  fprintf (out, "\n\n");
  fprintf (out, "Rules\n-----\n\n");
  fprintf (out, "Num (Prec, Assoc, Useful, Ritem Range) Lhs -> Rhs (Ritem range) [Num]\n");
  for (i = 1; i <= nrules; i++)
    {
      int rhs_count = 0;
      /* Find the last RHS index in ritems. */
      for (r = &ritem[rule_table[i].rhs]; *r > 0; ++r)
	++rhs_count;
      fprintf (out, "%3d (%2d, %2d, %2d, %2d-%2d)   %2d ->",
	       i,
	       rule_table[i].prec, rule_table[i].assoc, rule_table[i].useful,
	       rule_table[i].rhs, rule_table[i].rhs + rhs_count - 1,
	       rule_table[i].lhs);
      /* Dumped the RHS. */
      for (r = &ritem[rule_table[i].rhs]; *r > 0; r++)
	fprintf (out, "%3d", *r);
      fprintf (out, "  [%d]\n", -(*r));
    }
  fprintf (out, "\n\n");
  fprintf (out, "Rules interpreted\n-----------------\n\n");
  for (i = 1; i <= nrules; i++)
    {
      fprintf (out, "%-5d  %s :", i, tags[rule_table[i].lhs]);
      for (r = &ritem[rule_table[i].rhs]; *r > 0; r++)
	fprintf (out, " %s", tags[*r]);
      fputc ('\n', out);
    }
  fprintf (out, "\n\n");
}



/*-------------------------------.
| Report the results to STDERR.  |
`-------------------------------*/

static void
reduce_print (void)
{
  if (yacc_flag && nuseless_productions)
    fprintf (stderr, ngettext ("%d rule never reduced\n",
			       "%d rules never reduced\n",
			       nuseless_productions),
	     nuseless_productions);

  fprintf (stderr, _("%s contains "), infile);

  if (nuseless_nonterminals > 0)
    fprintf (stderr, ngettext ("%d useless nonterminal",
			       "%d useless nonterminals",
			       nuseless_nonterminals),
	     nuseless_nonterminals);

  if (nuseless_nonterminals > 0 && nuseless_productions > 0)
    fprintf (stderr, _(" and "));

  if (nuseless_productions > 0)
    fprintf (stderr, ngettext ("%d useless rule",
			       "%d useless rules",
			       nuseless_productions),
	     nuseless_productions);
  fprintf (stderr, "\n");
  fflush (stderr);
}

void
reduce_grammar (void)
{
  bool reduced;

  /* Allocate the global sets used to compute the reduced grammar */

  N = XCALLOC (unsigned, WORDSIZE (nvars));
  P = XCALLOC (unsigned, WORDSIZE (nrules + 1));
  V = XCALLOC (unsigned, WORDSIZE (nsyms));
  V1 = XCALLOC (unsigned, WORDSIZE (nsyms));

  useless_nonterminals ();
  inaccessable_symbols ();

  reduced = (bool) (nuseless_nonterminals + nuseless_productions > 0);

  if (!reduced)
    return;

  reduce_print ();

  if (!BITISSET (N, start_symbol - ntokens))
    fatal (_("Start symbol %s does not derive any sentence"),
	   tags[start_symbol]);

  reduce_grammar_tables ();
  if (nuseless_nonterminals > 0)
    nonterminals_reduce ();

  if (trace_flag)
    {
      dump_grammar (stderr);

      fprintf (stderr, "reduced %s defines %d terminals, %d nonterminals\
, and %d productions.\n",
	       infile, ntokens, nvars, nrules);
    }
}


/*-----------------------------------------------------------.
| Free the global sets used to compute the reduced grammar.  |
`-----------------------------------------------------------*/

void
reduce_free (void)
{
  XFREE (N);
  XFREE (V);
  XFREE (V1);
  XFREE (P);
}
