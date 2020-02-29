# bistromathic - all the bells and whistles
This example demonstrates the best practices when using Bison.
- Its interface is pure.
- It uses a custom syntax error with location tracking, lookahead correction
  and token internationalization.
- It supports debug traces with semantic values.
- It uses named references instead of the traditional $1, $2, etc.

It also uses Flex to generate the scanner.

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
