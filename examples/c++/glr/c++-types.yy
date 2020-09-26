/* Simplified -*- C++ -*- Type and Expression Grammar.  */

%glr-parser
%skeleton "glr2.cc"
%header
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

%define api.value.type {Node *}

%code
{

#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if __cplusplus < 201103L
# define nullptr 0
#endif

  static Node *new_nterm (char const *form, Node *child0 = nullptr, Node *child1 = nullptr, Node *child2 = nullptr);
  static Node *new_term (char *);
  static void free_node (Node *);
  static std::ostream& operator<< (std::ostream& o, const Node &node);
  static YYSTYPE stmtMerge (YYSTYPE x0, YYSTYPE x1);

  static int yylex (YYSTYPE *lvalp, YYLTYPE *llocp);
}

%expect-rr 1

%token
  TYPENAME "typename"
  ID "identifier"

%right '='
%left '+'

%destructor { free_node ($$); } stmt expr decl declarator TYPENAME ID

%%

prog : %empty
     | prog stmt   { std::cout << @2 << ": " << *$2 << '\n'; free_node ($2); }
     ;

stmt : expr ';'  %merge <stmtMerge>     { $$ = $1; }
     | decl      %merge <stmtMerge>
     | error ';'        { $$ = new_nterm ("<error>"); }
     | '@'              { $$ = $1; YYACCEPT; }
     ;

expr : ID
     | TYPENAME '(' expr ')'
                        { $$ = new_nterm ("<cast>", $3, $1); }
     | expr '+' expr    { $$ = new_nterm ("+", $1, $3); }
     | expr '=' expr    { $$ = new_nterm ("=", $1, $3); }
     ;

decl : TYPENAME declarator ';'
                        { $$ = new_nterm ("<declare>", $1, $2); }
     | TYPENAME declarator '=' expr ';'
                        { $$ = new_nterm ("<init-declare>", $1,
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
  yy::parser parser;
  return !!parser.parse ();
}


/* A C error reporting function.  */
void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}

int yylex (YYSTYPE *lvalp, YYLTYPE *llocp)
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
            int tok;
            llocp->begin.line = llocp->end.line = lineNum;
            llocp->begin.column = colNum;
            if (isalpha (c))
              {
                char buffer[256];
                unsigned i = 0;

                do
                  {
                    buffer[i++] = static_cast<char> (c);
                    colNum += 1;
                    assert (i != sizeof buffer - 1);
                    c = getchar ();
                  }
                while (isalnum (c) || c == '_');

                ungetc (c, stdin);
                buffer[i++] = 0;
                tok
                  = isupper (static_cast <unsigned char> (buffer[0]))
                  ? yy::parser::token::TYPENAME
                  : yy::parser::token::ID;
                *lvalp = new_term (strcpy (static_cast<char*> (malloc (i)), buffer));
              }
            else
              {
                colNum += 1;
                tok = c;
                *lvalp = nullptr;
              }
            llocp->end.column = colNum-1;
            return tok;
          }
        }
    }
}

static Node *
new_nterm (char const *form, Node *child0, Node *child1, Node *child2)
{
  Node *res = new Node;
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
  Node *res = new Node;
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
  delete node;
}

static std::ostream&
operator<< (std::ostream& o, const Node &node)
{
  if (node.nodeInfo.isNterm == 1)
    {
      o << node.nterm.form;
      if (node.nterm.children[0])
        {
          o << '(' << *node.nterm.children[0];
          if (node.nterm.children[1])
            o << ", " << *node.nterm.children[1];
          if (node.nterm.children[2])
            o << ", " << *node.nterm.children[2];
          o << ")";
        }
    }
  else
    o << node.term.text;
  return o;
}


static YYSTYPE
stmtMerge (YYSTYPE x0, YYSTYPE x1)
{
  return new_nterm ("<OR>", x0, x1);
}
