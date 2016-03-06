
#include <cstdio>
#include "Description.h"

extern FILE* yyin;
extern int yyparse();


int main(int argc, char** argv)
{
    yyin = stdin;
    yyparse();
}


