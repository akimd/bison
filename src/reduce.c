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
#include "symtab.h"
#include "gram.h"
#include "complain.h"
#include "reduce.h"
#include "reader.h"
#include "getargs.h"
#include "bitset.h"

typedef short *rule;


/* Set of all nonterminals which are not useless.  */
static bitset N;

/* Set of all rules which have no useless nonterminals in their RHS.  */
static bitset P;

/* Set of all accessible symbols.  */
static bitset V;

/* Set of symbols used to define rule precedence (so they are
   `useless', but no warning should be issued).  */
static bitset V1;

static int nuseful_productions;
static int nuseless_productions;
static int nuseful_nonterminals;
int nuseless_nonterminals;

/*-------------------------------------------------------------------.
| Another way to do this would be with a set for each production and |
| then do subset tests against N0, but even for the C grammar the    |
| whole reducing process takes only 2 seconds on my 8Mhz AT.         |
`-------------------------------------------------------------------*/

static bool
useful_production (int i, bitset N0)
{
  rule r;
  short n;

  /* A production is useful if all of the nonterminals in its appear
     in the set of useful nonterminals.  */

  for (r = rules[i].rhs; *r >= 0; r++)
    if (ISVAR (n = *r) && !bitset_test (N0, n - ntokens))
      return FALSE;
  return TRUE;
}


/*---------------------------------------------------------.
| Remember that rules are 1-origin, symbols are 0-origin.  |
`---------------------------------------------------------*/

static void
useless_nonterminals (void)
{
  bitset Np, Ns;
  int i;

  /* N is set as built.  Np is set being built this iteration. P is
     set of all productions which have a RHS all in N.  */

  Np = bitset_create (nvars, BITSET_FIXED);


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
      bitset_copy (Np, N);
      for (i = 1; i < nrules + 1; i++)
	if (!bitset_test (P, i)
	    && useful_production (i, N))
	  {
	    bitset_set (Np, rules[i].lhs - ntokens);
	    bitset_set (P, i);
	  }
      if (bitset_equal_p (N, Np))
	break;
      Ns = Np;
      Np = N;
      N = Ns;
    }
  bitset_free (N);
  N = Np;
}


static void
inaccessable_symbols (void)
{
  bitset Vp, Vs, Pp;
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

  Vp = bitset_create (nsyms, BITSET_FIXED);
  Pp = bitset_create (nrules + 1, BITSET_FIXED);

  /* If the start symbol isn't useful, then nothing will be useful. */
  if (bitset_test (N, start_symbol - ntokens))
    {
      bitset_set (V, start_symbol);

      while (1)
	{
	  bitset_copy (Vp, V);
	  for (i = 1; i < nrules + 1; i++)
	    {
	      if (!bitset_test (Pp, i)
		  && bitset_test (P, i)
		  && bitset_test (V, rules[i].lhs))
		{
		  for (r = rules[i].rhs; *r >= 0; r++)
		    if (ISTOKEN (t = *r) || bitset_test (N, t - ntokens))
		      bitset_set (Vp, t);
		  bitset_set (Pp, i);
		}
	    }
	  if (bitset_equal_p (V, Vp))
	    break;
	  Vs = Vp;
	  Vp = V;
	  V = Vs;
	}
    }

  bitset_free (V);
  V = Vp;

  /* Tokens 0, 1, and 2 are internal to Bison.  Consider them useful. */
  bitset_set (V, 0);		/* end-of-input token */
  bitset_set (V, 1);		/* error token */
  bitset_set (V, 2);		/* some undefined token */

  bitset_free (P);
  P = Pp;

  nuseful_productions = bitset_count (P);
  nuseless_productions = nrules - nuseful_productions;

  nuseful_nonterminals = 0;
  for (i = ntokens; i < nsyms; i++)
    if (bitset_test (V, i))
      nuseful_nonterminals++;
  nuseless_nonterminals = nvars - nuseful_nonterminals;

  /* A token that was used in %prec should not be warned about.  */
  for (i = 1; i < nrules + 1; i++)
    if (rules[i].precsym != 0)
      bitset_set (V1, rules[i].precsym);
}


/*-------------------------------------------------------------------.
| Put the useless productions at the end of RULES, and adjust NRULES |
| accordingly.                                                       |
`-------------------------------------------------------------------*/

static void
reduce_grammar_tables (void)
{
  /* Flag useless productions.  */
  {
    int pn;
    for (pn = 1; pn < nrules + 1; pn++)
      rules[pn].useful = bitset_test (P, pn);
  }

  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    int useful = 1;
    int useless = nrules + 1 - nuseless_productions;
    rule_t *rules_sorted = XMALLOC (rule_t, nrules + 1) - 1;
    int i;
    for (i = 1; i < nrules + 1; ++i)
      rules_sorted[rules[i].useful ? useful++ : useless++] = rules[i];
    free (rules + 1);
    rules = rules_sorted;

    /* Also reorder ritems. */
    {
      short *ritems_sorted = XCALLOC (short, nitems + 1);
      short *ritemsp = ritems_sorted;
      for (i = 1; i < nrules + 1; ++i)
	{
	  short *rhsp = rules[i].rhs;
	  rules[i].rhs = ritemsp;
	  for (/* Nothing. */; *rhsp >= 0; ++rhsp)
	    *ritemsp++ = *rhsp;
	  *ritemsp++ = -i;
	}
      *ritemsp++ = 0;
      free (ritem);
      ritem = ritems_sorted;
    }
    nrules -= nuseless_productions;
  }

  /* Adjust NRITEMS and NITEMS.  */
  {
    int r;
    int length;
    for (r = nrules + 1; r < nrules + 1 + nuseless_productions; ++r)
      {
	length = rule_rhs_length (&rules[r]);
	nritems -= length + 1;
	nitems -= length + 1;
      }
  }
}


/*------------------------------.
| Remove useless nonterminals.  |
`------------------------------*/

static void
nonterminals_reduce (void)
{
  int i, n;

  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */

  short *nontermmap = XCALLOC (short, nvars) - ntokens;
  n = ntokens;
  for (i = ntokens; i < nsyms; i++)
    if (bitset_test (V, i))
      nontermmap[i] = n++;
  for (i = ntokens; i < nsyms; i++)
    if (!bitset_test (V, i))
      nontermmap[i] = n++;


  /* Shuffle elements of tables indexed by symbol number.  */
  {
    bucket **symbols_sorted = XMALLOC (bucket *, nvars) - ntokens;

    for (i = ntokens; i < nsyms; i++)
      symbols_sorted[nontermmap[i]] = symbols[i];
    for (i = ntokens; i < nsyms; i++)
      symbols[i] = symbols_sorted[i];
    free (symbols_sorted + ntokens);
  }

  /* Replace all symbol numbers in valid data structures.  */

  for (i = 1; i < nrules + 1; i++)
    {
      rules[i].lhs = nontermmap[rules[i].lhs];
      if (ISVAR (rules[i].precsym))
	/* Can this happen?  */
	rules[i].precsym = nontermmap[rules[i].precsym];
    }

  for (i = 0; i < nritems; ++i)
    if (ISVAR (ritem[i]))
      ritem[i] = nontermmap[ritem[i]];

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
	fprintf (out, "   %s\n", symbols[nsyms + i]->tag);
      fputs ("\n\n", out);
    }

  {
    bool b = FALSE;
    int i;
    for (i = 0; i < ntokens; i++)
      if (!bitset_test (V, i) && !bitset_test (V1, i))
	{
	  if (!b)
	    fprintf (out, "%s\n\n", _("Terminals which are not used:"));
	  b = TRUE;
	  fprintf (out, "   %s\n", symbols[i]->tag);
	}
    if (b)
      fputs ("\n\n", out);
  }

  if (nuseless_productions > 0)
    {
      int i;
      fprintf (out, "%s\n\n", _("Useless rules:"));
      for (i = nrules + 1; i < nuseless_productions + nrules + 1; i++)
	{
	  rule r;
	  fprintf (out, "#%-4d  ", rules[i].number - 1);
	  fprintf (out, "%s:", symbols[rules[i].lhs]->tag);
	  for (r = rules[i].rhs; *r >= 0; r++)
	    fprintf (out, " %s", symbols[*r]->tag);
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
    fprintf (out, "%5d  %5d   %5d  %s\n",
	     i,
	     symbols[i]->prec, symbols[i]->assoc, symbols[i]->tag);
  fprintf (out, "\n\n");
  fprintf (out, "Rules\n-----\n\n");
  fprintf (out, "Num (Prec, Assoc, Useful, Ritem Range) Lhs -> Rhs (Ritem range) [Num]\n");
  for (i = 1; i < nrules + nuseless_productions + 1; i++)
    {
      int rhs_count = 0;
      /* Find the last RHS index in ritems. */
      for (r = rules[i].rhs; *r >= 0; ++r)
	++rhs_count;
      fprintf (out, "%3d (%2d, %2d, %2d, %2d-%2d)   %2d ->",
	       i - 1,
	       rules[i].prec, rules[i].assoc, rules[i].useful,
	       rules[i].rhs - ritem, rules[i].rhs - ritem + rhs_count - 1,
	       rules[i].lhs);
      /* Dumped the RHS. */
      for (r = rules[i].rhs; *r >= 0; r++)
	fprintf (out, "%3d", *r);
      fprintf (out, "  [%d]\n", -(*r) - 1);
    }
  fprintf (out, "\n\n");
  fprintf (out, "Rules interpreted\n-----------------\n\n");
  for (i = 1; i < nrules + nuseless_productions + 1; i++)
    {
      fprintf (out, "%-5d  %s :", i, symbols[rules[i].lhs]->tag);
      for (r = rules[i].rhs; *r >= 0; r++)
	fprintf (out, " %s", symbols[*r]->tag);
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

  N = bitset_create (nvars, BITSET_FIXED);
  P =  bitset_create (nrules + 1, BITSET_FIXED);
  V = bitset_create (nsyms, BITSET_FIXED);
  V1 = bitset_create (nsyms, BITSET_FIXED);

  useless_nonterminals ();
  inaccessable_symbols ();

  reduced = (bool) (nuseless_nonterminals + nuseless_productions > 0);
  if (!reduced)
    return;

  reduce_print ();

  if (!bitset_test (N, start_symbol - ntokens))
    fatal (_("Start symbol %s does not derive any sentence"),
	   symbols[start_symbol]->tag);

  if (nuseless_productions > 0)
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
  bitset_free (N);
  bitset_free (V);
  bitset_free (V1);
  bitset_free (P);
}
