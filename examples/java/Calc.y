%language "Java"

%define api.parser.class {Calc}
%define api.parser.public

%define parse.error verbose
%locations

%code imports {
  import java.io.IOException;
  import java.io.InputStream;
  import java.io.InputStreamReader;
  import java.io.Reader;
  import java.io.StreamTokenizer;
}

%code {
  public static void main (String args[]) throws IOException
  {
    CalcLexer l = new CalcLexer (System.in);
    Calc p = new Calc (l);
    p.parse ();
  }
}

/* Bison Declarations */
%token <Integer> NUM "number"
%type  <Integer> exp

%nonassoc '='       /* comparison            */
%left '-' '+'
%left '*' '/'
%precedence NEG     /* negation--unary minus */
%right '^'          /* exponentiation        */

/* Grammar follows */
%%
input:
  line
| input line
;

line:
  '\n'
| exp '\n'           { System.out.println ($exp); }
| error '\n'
;

exp:
  NUM                { $$ = $1;                                             }
| exp '=' exp
  {
    if ($1.intValue () != $3.intValue ())
      yyerror (@$, "calc: error: " + $1 + " != " + $3);
  }
| exp '+' exp        { $$ = new Integer ($1.intValue () + $3.intValue ());  }
| exp '-' exp        { $$ = new Integer ($1.intValue () - $3.intValue ());  }
| exp '*' exp        { $$ = new Integer ($1.intValue () * $3.intValue ());  }
| exp '/' exp        { $$ = new Integer ($1.intValue () / $3.intValue ());  }
| '-' exp  %prec NEG { $$ = new Integer (-$2.intValue ());                  }
| exp '^' exp        { $$ = new Integer ((int)
                                         Math.pow ($1.intValue (),
                                                   $3.intValue ()));        }
| '(' exp ')'        { $$ = $2;                                             }
| '(' error ')'      { $$ = new Integer (1111);                             }
| '!'                { $$ = new Integer (0); return YYERROR;                }
| '-' error          { $$ = new Integer (0); return YYERROR;                }
;


%%
class CalcLexer implements Calc.Lexer {

  StreamTokenizer st;

  public CalcLexer (InputStream is)
  {
    st = new StreamTokenizer (new InputStreamReader (is));
    st.resetSyntax ();
    st.eolIsSignificant (true);
    st.whitespaceChars (9, 9);
    st.whitespaceChars (32, 32);
    st.wordChars (48, 57);
  }


  Position yypos = new Position (1, 0);

  public Position getStartPos () {
    return yypos;
  }

  public Position getEndPos () {
    return yypos;
  }

  public void yyerror (Calc.Location l, String s)
  {
    if (l == null)
      System.err.println (s);
    else
      System.err.println (l + ": " + s);
  }


  Integer yylval;

  public Object getLVal () {
    return yylval;
  }

  public int yylex () throws IOException {
    int ttype = st.nextToken ();
    yypos = new Position (yypos.lineno (), yypos.token () + 1);
    if (ttype == st.TT_EOF)
      return EOF;
    else if (ttype == st.TT_EOL)
      {
        yypos = new Position (yypos.lineno () + 1, 0);
        return (int) '\n';
      }
    else if (ttype == st.TT_WORD)
      {
        yylval = new Integer (st.sval);
        return NUM;
      }
    else
      return st.ttype;
  }
}


class Position {
  public int line;
  public int token;

  public Position ()
  {
    line = 0;
    token = 0;
  }

  public Position (int l, int t)
  {
    line = l;
    token = t;
  }

  public boolean equals (Position l)
  {
    return l.line == line && l.token == token;
  }

  public String toString ()
  {
    return Integer.toString (line) + "." + Integer.toString(token);
  }

  public int lineno ()
  {
    return line;
  }

  public int token ()
  {
    return token;
  }
}
