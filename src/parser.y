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


%{

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include "ParserAST.h"
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

    std::vector<parsy::Rule*>* ruleList;
    parsy::Rule* rule;

    std::vector<parsy::Pattern*>* patternList;
    parsy::Pattern* pattern;
    std::string* string;

    int token;
};


%token <string> IDENTIFIER
%token <token> NEW_LINE SEMICOLON
%token <token> ASSIGN PIPE

%type <descriptionFile> descriptionFile
%type <rule> rule
%type <ruleList> ruleList
%type <patternList> patternList
%type <pattern> pattern


%start descriptionFile

%%

descriptionFile:
    ruleList {
        $$ = new DescriptionFile();
        $$->rules = *$1;
        delete $1;
        root = $$;
    };

    ruleList:
    /* empty */
    {
        $$ = new std::vector<parsy::Rule*>();
    }
    |
    ruleList rule {
        $1->push_back($2);
    };

rule:
    IDENTIFIER ASSIGN patternList SEMICOLON {
        $$ = new Rule();
        $$->nonterminal = *$1;
        delete $1;
        $$->patterns = *$3;
        delete $3;
    };

patternList:
    pattern {
        $$ = new std::vector<parsy::Pattern*>();
        $$->push_back($1);
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


