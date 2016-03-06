/* =============================================================================
//
// This file is part of the parsy parser generator.
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

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include "Description.h"
extern int yylex();

int yyerror(const char* msg)
{
    fprintf(stderr, "error: %s\n", msg);
    //throw ParserException("syntax error");
}

using namespace parsy;

parsy::DescriptionFile* root = 0;


%}

%union {
    parsy::DescriptionFile* descriptionFile;

    std::vector<parsy::Nonterminal*>* nonterminalList;
    parsy::Nonterminal* nonterminal;

    std::vector<parsy::Pattern*>* patternList;
    parsy::Pattern* pattern;
    std::string* string;

    int token;
};


%token <string> IDENTIFIER
%token <token> NEW_LINE
%token <token> ASSIGN PIPE

%type <descriptionFile> descriptionFile
%type <nonterminal> nonterminal
%type <nonterminalList> nonterminalList
%type <patternList> patternList
%type <pattern> pattern


%start descriptionFile

%%

descriptionFile:
    nonterminalList {
        $$ = new DescriptionFile();
        $$->nonterminals = *$1;
        delete $1;
        root = $$;
    };

    nonterminalList:
    /* empty */
    {
        $$ = new std::vector<parsy::Nonterminal*>();
    }
    |
    nonterminalList nonterminal {
        $1->push_back($2);
    };

nonterminal:
    IDENTIFIER ASSIGN patternList {
        $$ = new Nonterminal();
        $$->patterns = *$3;
        delete $3;
    };

patternList:
    pattern {
        $$ = new std::vector<parsy::Pattern*>();
    }
    |
    patternList PIPE pattern {
        $1->push_back($3);
    };

pattern:
    IDENTIFIER {
        $$ = new parsy::Pattern();
        $$->pieces.push_back(*$1);
        delete $1;
        $1 = 0;
    }
    |
    pattern IDENTIFIER {
        $1->pieces.push_back(*$2);
        delete $2;
        $2 = 0;
    }
    ;



%%


