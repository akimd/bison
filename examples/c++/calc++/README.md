# calc++ - A Flex+Bison calculator

This directory contains calc++, a Bison grammar file in C++.  If you never
saw the traditional implementation in C, please first read
examples/c/lexcalc, which can be seen as a C precursor of this example.

Read the corresponding chapter in the documentation: "A Complete C++
Example".  It is also available on line (maybe with a different version of
Bison):
https://www.gnu.org/software/bison/manual/html_node/A-Complete-C_002b_002b-Example.html

To use it, copy this directory into some work directory, and run `make` to
compile the executable, and try it.  It is a simple calculator which accepts
several variable definitions, one per line, and then a single expression to
evaluate.

The program calc++ expects the file to parse as argument; pass `-` to read
the standard input (and then hit <Ctrl-d>, control-d, to end your input).

```
$ ./calc++ -
one := 1
two := 2
three := 3
(one + two * three) * two * three
<Ctrl-d>
42
```

You may pass `-p` to activate the parser debug traces, and `-s` to activate
the scanner's.

<!---
Local Variables:
fill-column: 76
ispell-dictionary: "american"
End:

Copyright (C) 2018-2019 Free Software Foundation, Inc.

This file is part of Bison, the GNU Compiler Compiler.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

# LocalWords:  calc parsers yy MERCHANTABILITY Ctrl ispell american
--->
