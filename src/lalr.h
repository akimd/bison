/* Compute look-ahead criteria for bison,
   Copyright 1984, 1986, 1989, 2000 Free Software Foundation, Inc.

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

#ifndef LALR_H_
# define LALR_H_


/* Import the definition of CORE, SHIFTS and REDUCTIONS. */
# include "state.h"


/* Compute how to make the finite state machine deterministic; find
   which rules need lookahead in each state, and which lookahead
   tokens they accept.  */

void lalr PARAMS ((void));


/* lalr() builds these data structures. */

/* GOTO_MAP, FROM_STATE and TO_STATE -- record each shift transition
   which accepts a variable (a nonterminal).

   FROM_STATE[T] -- state number which a transition leads from.
   TO_STATE[T] -- state number it leads to.

   All the transitions that accept a particular variable are grouped
   together and GOTO_MAP[I - NTOKENS] is the index in FROM_STATE and
   TO_STATE of the first of them.  */

extern short *goto_map;
extern short *from_state;
extern short *to_state;

/* CONSISTENT[S] is nonzero if no lookahead is needed to decide what
   to do in state S.  */

extern char *consistent;


/* LARULENO is a vector which records the rules that need lookahead in
   various states.  The elements of LARULENO that apply to state S are
   those from LOOKAHEADS[S] through LOOKAHEADS[S+1]-1.  Each element
   of LARULENO is a rule number.

   If LR is the length of LAruleno, then a number from 0 to LR-1 can
   specify both a rule and a state where the rule might be applied.
   */

extern short *LAruleno;

/* LA is a lr by ntokens matrix of bits.  LA[l, i] is 1 if the rule
   LAruleno[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

extern unsigned *LA;


extern int tokensetsize;
extern short *lookaheads;
extern short *accessing_symbol;
extern core **state_table;
extern shifts **shift_table;
extern reductions **reduction_table;



#endif /* !LALR_H_ */
