/* ============================================================================
 *
 * This file is part of the parsy parser generator.
 *
 * Copyright (C) 2016 Nicolas Winkler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ==========================================================================*/
#include <iostream>
#include <cstdio>
#include "ParserAST.h"
#include "Structure.h"

extern FILE* yyin;
extern parsy::DescriptionFile* root;
extern int yyparse();


int main(int argc, char** argv)
{
    yyin = stdin;
    yyparse();

    if (root == nullptr)
        return 1;

    using parsy::ParserUnit;
    ParserUnit* pu = ParserUnit::createUnit(root);
    pu->generateParserCode(std::cout, std::cout);
    
}


