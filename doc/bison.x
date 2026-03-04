[name]
bison \- GNU Project parser generator (yacc replacement)

[description]
The
.B bison
parser generator inputs grammars in the style of
.BR yacc (1)
and outputs parsers for those grammers.
.PP
Input files should follow the
.B yacc
convention of ending in
.IR .y .
Unlike
.BR yacc ,
the generated files do not have fixed names, but instead use the prefix
of the input file.
Moreover, if you need to put C++
code in the input file, you can end its name by a C++-like extension
.RI ( .ypp
or
.IR .y++ ),
and then
.B bison
will follow your extension to name the
output file
.RI ( .cpp
or
.IR .c++ ).
For instance, a grammar description file named
.I parse.yxx
would produce the generated parser in a file named
.IR parse.tab.cxx ,
instead of
.BR yacc 's
.IR y.tab.c .
.PP
This description of the options that can be given to
.B bison
is adapted from the node
.B Invocation
in the Bison manual, which should be taken as authoritative.
.PP
The
.B bison
command supports both traditional single-letter options and mnemonic long
option names.  Long option names are indicated with
.B \-\-
instead of
.BR \- .
Abbreviations for option names are allowed as long as they
are unique.  When a long option takes an argument, like
.BR \-\-file-prefix ,
connect the option name and the argument with
.BR = .

[see also]
.BR lex (1),
.BR flex (1),
.BR yacc (1).
