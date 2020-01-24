# pushcalc - push parser with Bison

This directory contains pushcalc, the traditional calculator, implemented as
a push parser.

Traditionally Bison is used to create so called "pull parsers": the user
invokes the parser once, which repeatedly calls (pulls) the scanner until
the input is drained.

This example demonstrates the "push parsers": the user calls scanner to
fetch the next token, passes (pushes) it to the parser, and repeats the
operation until the input is drained.

This example is a straightforward conversion of the 'calc' example to the
push-parser model.

<!---
Local Variables:
fill-column: 76
ispell-dictionary: "american"
End:

Copyright (C) 2020 Free Software Foundation, Inc.

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
