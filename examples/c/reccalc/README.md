# reccalc - recursive calculator with Flex and Bison

In this example the generated parser is pure and reentrant: it can be used
concurrently in different threads, or recursively.  As a proof of this
reentrancy, expressions in parenthesis are tokenized as strings, and then
recursively parsed from the parser:

```
exp: STR
  {
    result r = parse_string ($1);
    free ($1);
    if (r.nerrs)
      {
        res->nerrs += r.nerrs;
        YYERROR;
      }
    else
      $$ = r.value;
  }
```

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
--->
