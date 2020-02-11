%language "Java"

%define api.parser.class {Calc}
%define api.parser.public

%define parse.error custom
%define parse.trace

%locations

%code imports {
  import java.io.BufferedReader;
  import java.io.IOException;
  import java.io.InputStream;
  import java.io.InputStreamReader;
  import java.io.Reader;
  import java.io.StreamTokenizer;
  import java.nio.CharBuffer;
}

%code {
  public static void main (String[] args) throws IOException
  {
    CalcLexer l = new CalcLexer (System.in);
    Calc p = new Calc (l);
    for (String arg : args)
      if (arg.equals ("-p"))
        p.setDebugLevel (1);
    if (!p.parse ())
      System.exit (1);
  }

  static String _ (String s)
  {
    return s;
  }
}

/* Bison Declarations */
%token
  '\n'   _("end of line")
  <Integer> NUM _("number")
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
  NUM                { $$ = $1; }
| exp '=' exp
  {
    if ($1.intValue () != $3.intValue ())
      yyerror (@$, "calc: error: " + $1 + " != " + $3);
  }
| exp '+' exp        { $$ = $1 + $3;  }
| exp '-' exp        { $$ = $1 - $3;  }
| exp '*' exp        { $$ = $1 * $3;  }
| exp '/' exp        { $$ = $1 / $3;  }
| '-' exp  %prec NEG { $$ = -$2; }
| exp '^' exp        { $$ = (int) Math.pow ($1, $3); }
| '(' exp ')'        { $$ = $2; }
| '(' error ')'      { $$ = 1111; }
| '!'                { $$ = 0; return YYERROR; }
| '-' error          { $$ = 0; return YYERROR; }
;

%%
class CalcLexer implements Calc.Lexer {

  StreamTokenizer st;
  PositionReader reader;

  public CalcLexer (InputStream is)
  {
    reader = new PositionReader (new InputStreamReader (is));
    st = new StreamTokenizer (reader);
    st.resetSyntax ();
    st.eolIsSignificant (true);
    st.wordChars ('0', '9');
  }

  Position start = new Position (1, 0);
  Position end = new Position (1, 0);

  public Position getStartPos () {
    return new Position (start);
  }

  public Position getEndPos () {
    return new Position (end);
  }

  public void yyreportSyntaxError (Calc.Context ctx)
  {
    final int ARGMAX = 10;
    int[] arg = new int[ARGMAX];
    int n = ctx.yysyntaxErrorArguments (arg, ARGMAX);
    System.err.print (ctx.getLocation () + ": syntax error");
    for (int i = 1; i < n; ++i)
      System.err.print ((i == 1 ? ": expected " : " or ")
                        + ctx.yysymbolName (arg[i]));
    if (n != 0)
      System.err.print (" before " + ctx.yysymbolName (arg[0]));
    System.err.println ("");
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
    start.set (reader.getPosition ());
    int ttype = st.nextToken ();
    end.set (reader.getPosition ());
    switch (ttype)
      {
      case StreamTokenizer.TT_EOF:
        return EOF;
      case StreamTokenizer.TT_EOL:
        end.line += 1;
        end.column = 0;
        return (int) '\n';
      case StreamTokenizer.TT_WORD:
        yylval = new Integer (st.sval);
        end.set (reader.getPreviousPosition ());
        return NUM;
      case ' ': case '\t':
        return yylex ();
      default:
        return ttype;
      }
  }
}


class Position {
  public int line = 1;
  public int column = 1;

  public Position ()
  {
    line = 1;
    column = 1;
  }

  public Position (int l, int t)
  {
    line = l;
    column = t;
  }

  public Position (Position p)
  {
    line = p.line;
    column = p.column;
  }

  public void set (Position p)
  {
    line = p.line;
    column = p.column;
  }

  public boolean equals (Position l)
  {
    return l.line == line && l.column == column;
  }

  public String toString ()
  {
    return Integer.toString (line) + "." + Integer.toString (column);
  }

  public int line ()
  {
    return line;
  }

  public int column ()
  {
    return column;
  }
}

class PositionReader extends BufferedReader {

  private Position position = new Position ();
  // Position before the latest call to "read", i.e. position
  // of the last character of the current token.
  private Position previousPosition = new Position ();

  public PositionReader (Reader reader) {
    super (reader);
  }

  public int read () throws IOException {
    previousPosition.set (position);
    int res = super.read ();
    if (res > -1) {
      char c = (char)res;
      if (c == '\r' || c == '\n') {
        position.line += 1;
        position.column = 1;
      } else {
        position.column += 1;
      }
    }
    return res;
  }

  public Position getPosition () {
    return position;
  }

  public Position getPreviousPosition () {
    return previousPosition;
  }
}
