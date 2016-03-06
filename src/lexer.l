/* =============================================================================
//
// This file is part of the parsy parser generator
//
// Copyright (C) 2016 Nicolas Winkler
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ===========================================================================*/

%{
#include "Description.h"
#include "parser.h"


#define SET_TOKEN(t) (yylval.token = t)
#define SET_STRING (yylval.string = new std::string(yytext, yyleng))

extern "C" int yywrap()
{
    return 1; /* do not continue on EOF */
}
/*\n                      return SET_TOKEN(NEW_LINE);*/

%}


%%



[\t ]                   ; // Space or tab ignored
\/\*[.\n]*\*\/          ; // comment



=                       return SET_TOKEN(ASSIGN);
\|                      return SET_TOKEN(PIPE);



[a-zA-Z_][a-zA-Z0-9_]*  SET_STRING; return IDENTIFIER;
.                       printf("Unknown token!\n"); yyterminate();

%%

