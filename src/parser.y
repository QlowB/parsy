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
    exit(1);
    //throw ParserException("syntax error");
}

using namespace parsy;

std::unique_ptr<parsy::DescriptionFile> root;


%}

%union {
    parsy::DescriptionFile* descriptionFile;

    std::vector<std::unique_ptr<parsy::TerminalRule>>* terminalList;
    parsy::TerminalRule* terminal;

    std::vector<std::unique_ptr<parsy::Rule>>* ruleList;
    parsy::Rule* rule;

    std::vector<std::unique_ptr<parsy::Pattern>>* patternList;
    parsy::Pattern* pattern;
    std::string* string;

    int token;
};


%token <token> TERMINAL
%token <string> IDENTIFIER REGEX_STRING
%token <token> NEW_LINE SEMICOLON COLON
%token <token> ASSIGN PIPE

%type <descriptionFile> descriptionFile
%type <terminal> terminal
%type <terminalList> terminalList
%type <rule> rule
%type <ruleList> ruleList
%type <patternList> patternList
%type <pattern> pattern


%start descriptionFile

%%

descriptionFile:
    terminalList ruleList {
        $$ = new DescriptionFile();
        $$->terminalRules = std::move(*$1);
        $$->rules = std::move(*$2);
        delete $1;
        delete $2;
        root = std::unique_ptr<DescriptionFile> ($$);
    };

terminalList:
    /* empty */
    {
        $$ = new std::vector<std::unique_ptr<parsy::TerminalRule>>();
    }
    |
    terminalList terminal {
        $1->push_back(std::unique_ptr<parsy::TerminalRule>($2));
    };

terminal:
    TERMINAL IDENTIFIER ASSIGN REGEX_STRING SEMICOLON {
        $$ = new parsy::TerminalRule {
            std::move(*$2), std::move(*$4)
        };
        delete $2; delete $4;
    };

ruleList:
    /* empty */
    {
        $$ = new std::vector<std::unique_ptr<parsy::Rule>>();
    }
    |
    ruleList rule {
        $1->push_back(std::unique_ptr<parsy::Rule>($2));
    };

rule:
    IDENTIFIER COLON patternList SEMICOLON {
        $$ = new Rule();
        $$->nonterminal = std::move(*$1);
        delete $1;
        $$->patterns = std::move(*$3);
        delete $3;
    };

patternList:
    pattern {
        $$ = new std::vector<std::unique_ptr<parsy::Pattern>>();
        $$->push_back(std::unique_ptr<parsy::Pattern>($1));
    }
    |
    patternList PIPE pattern {
        $1->push_back(std::unique_ptr<parsy::Pattern>($3));
    };

pattern:
    IDENTIFIER {
        $$ = new parsy::Pattern();
        $$->pieces.push_back(std::move(*$1));
        delete $1;
        $1 = 0;
    }
    |
    pattern IDENTIFIER {
        $1->pieces.push_back(std::move(*$2));
        delete $2;
        $2 = 0;
    }
    ;



%%


