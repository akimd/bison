/* Simplified C++ Type and Expression Grammar.  */

%define api.pure
%header
%define api.header.include {"c++-types.h"}
%locations
%debug

/* Nice error messages with details. */
%define parse.error detailed

%code requires
{
  union Node {
    struct {
      int isNterm;
      int parents;
    } nodeInfo;
    struct {
      int isNterm; /* 1 */
      int parents;
      char const *form;
      union Node *children[3];
    } nterm;
    struct {
      int isNterm; /* 0 */
      int parents;
      char *text;
    } term;
  };
  typedef union Node Node;
}

%define api.value.type union

%code
{

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

  static Node *new_nterm (char const *, Node *, Node *, Node *);
  static Node *new_term (char *);
  static void free_node (Node *);
  static char *node_to_string (Node *);
  static Node *stmtMerge (YYSTYPE x0, YYSTYPE x1);

  static int location_print (FILE *yyo, YYLTYPE const * const yylocp);
  static void yyerror (YYLTYPE const * const llocp, const char *msg);
  static yytoken_kind_t yylex (YYSTYPE *lvalp, YYLTYPE *llocp);
}

%expect-rr 1

%token
  TYPENAME "typename"
  ID "identifier"

%right '='
%left '+'

%glr-parser

%type <Node*> stmt expr decl declarator TYPENAME ID
%destructor { free_node ($$); } <Node*>

%%

prog : %empty
     | prog stmt   {
                     char *output = node_to_string ($2);
                     printf ("%d.%d-%d.%d: %s\n",
                             @2.first_line, @2.first_column,
                             @2.last_line, @2.last_column,
                             output);
                     free (output);
                     free_node ($2);
                   }
     ;

stmt : expr ';'  %merge <stmtMerge>     { $$ = $1; }
     | decl      %merge <stmtMerge>
     | error ';'        { $$ = new_nterm ("<error>", NULL, NULL, NULL); }
     ;

expr : ID
     | TYPENAME '(' expr ')'
                        { $$ = new_nterm ("<cast>(%s,%s)", $3, $1, NULL); }
     | expr '+' expr    { $$ = new_nterm ("+(%s,%s)", $1, $3, NULL); }
     | expr '=' expr    { $$ = new_nterm ("=(%s,%s)", $1, $3, NULL); }
     ;

decl : TYPENAME declarator ';'
                        { $$ = new_nterm ("<declare>(%s,%s)", $1, $2, NULL); }
     | TYPENAME declarator '=' expr ';'
                        { $$ = new_nterm ("<init-declare>(%s,%s,%s)", $1,
                                          $2, $4); }
     ;

declarator
     : ID
     | '(' declarator ')' { $$ = $2; }
     ;

%%

int
main (int argc, char **argv)
{
  // Enable parse traces on option -p.
  if (1 < argc && strcmp (argv[1], "-p") == 0)
    yydebug = 1;
  return yyparse ();
}


/* Print *YYLOCP on YYO. */

static int
location_print (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += fprintf (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += fprintf (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += fprintf (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += fprintf (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += fprintf (yyo, "-%d", end_col);
    }
  return res;
}

/* A C error reporting function.  */
static
void yyerror (YYLTYPE const * const llocp, const char *msg)
{
  location_print (stderr, llocp);
  fprintf (stderr, ": %s\n", msg);
}

yytoken_kind_t
yylex (YYSTYPE *lvalp, YYLTYPE *llocp)
{
  static int lineNum = 1;
  static int colNum = 0;

  while (1)
    {
      int c;
      assert (!feof (stdin));
      c = getchar ();
      switch (c)
        {
        case EOF:
          return 0;
        case '\t':
          colNum = (colNum + 7) & ~7;
          break;
        case ' ': case '\f':
          colNum += 1;
          break;
        case '\n':
          lineNum += 1;
          colNum = 0;
          break;
        default:
          {
            yytoken_kind_t tok;
            llocp->first_line = llocp->last_line = lineNum;
            llocp->first_column = colNum;
            if (isalpha (c))
              {
                char buffer[256];
                unsigned i = 0;

                do
                  {
                    buffer[i++] = (char) c;
                    colNum += 1;
                    assert (i != sizeof buffer - 1);
                    c = getchar ();
                  }
                while (isalnum (c) || c == '_');

                ungetc (c, stdin);
                buffer[i++] = 0;
                if (isupper ((unsigned char) buffer[0]))
                  {
                    tok = TYPENAME;
                    lvalp->TYPENAME = new_term (strcpy (malloc (i), buffer));
                  }
                else
                  {
                    tok = ID;
                    lvalp->ID = new_term (strcpy (malloc (i), buffer));
                  }
              }
            else
              {
                colNum += 1;
                tok = c;
              }
            llocp->last_column = colNum-1;
            return tok;
          }
        }
    }
}

static Node *
new_nterm (char const *form, Node *child0, Node *child1, Node *child2)
{
  Node *res = malloc (sizeof *res);
  res->nterm.isNterm = 1;
  res->nterm.parents = 0;
  res->nterm.form = form;
  res->nterm.children[0] = child0;
  if (child0)
    child0->nodeInfo.parents += 1;
  res->nterm.children[1] = child1;
  if (child1)
    child1->nodeInfo.parents += 1;
  res->nterm.children[2] = child2;
  if (child2)
    child2->nodeInfo.parents += 1;
  return res;
}

static Node *
new_term (char *text)
{
  Node *res = malloc (sizeof *res);
  res->term.isNterm = 0;
  res->term.parents = 0;
  res->term.text = text;
  return res;
}

static void
free_node (Node *node)
{
  if (!node)
    return;
  node->nodeInfo.parents -= 1;
  /* Free only if 0 (last parent) or -1 (no parents).  */
  if (node->nodeInfo.parents > 0)
    return;
  if (node->nodeInfo.isNterm == 1)
    {
      free_node (node->nterm.children[0]);
      free_node (node->nterm.children[1]);
      free_node (node->nterm.children[2]);
    }
  else
    free (node->term.text);
  free (node);
}

static char *
node_to_string (Node *node)
{
  char *res;
  if (!node)
    {
      res = malloc (1);
      res[0] = 0;
    }
  else if (node->nodeInfo.isNterm == 1)
    {
      char *child0 = node_to_string (node->nterm.children[0]);
      char *child1 = node_to_string (node->nterm.children[1]);
      char *child2 = node_to_string (node->nterm.children[2]);
      res = malloc (strlen (node->nterm.form) + strlen (child0)
                    + strlen (child1) + strlen (child2) + 1);
      sprintf (res, node->nterm.form, child0, child1, child2);
      free (child2);
      free (child1);
      free (child0);
    }
  else
    res = strdup (node->term.text);
  return res;
}


static Node*
stmtMerge (YYSTYPE x0, YYSTYPE x1)
{
  return new_nterm ("<OR>(%s,%s)", x0.stmt, x1.stmt, NULL);
}
