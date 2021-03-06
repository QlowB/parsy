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
#include <fstream>
#include <cstdio>
#include "ParserAST.h"
#include "Structure.h"

extern FILE* yyin;
extern std::unique_ptr<parsy::DescriptionFile> root;
extern int yyparse();


int main(int argc, char** argv)
{
    bool openedFile = false;
    yyin = stdin;
    if (argc > 1) {
        FILE* f = fopen(argv[1], "r");
        if (f) {
            yyin = f;
            openedFile = true;
        }
    }
    yyparse();

    if (root.get() == nullptr)
        return 1;

    if (openedFile)
        fclose(yyin);

    using parsy::ParserUnit;
    ParserUnit* pu = ParserUnit::createUnit(root);
    if (argc > 1 && argv[1][0] == 'y') {
        std::ofstream h("P.h");
        std::ofstream s("P.cpp");
        pu->generateParserCode(h, s, "P.h");
        h.close();
        s.close();
    }
    else
        pu->generateParserCode(std::cout, std::cout, "Parser.h");
    
}


