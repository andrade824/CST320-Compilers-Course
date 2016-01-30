//**************************************
// main.cpp
//
// Main routine for lang compiler.
// This version only runs the lexer
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov 23, 2015
// 
// Modified: Devon Andrade <devon.andrade@oit.edu>
//     Date: Jan 26, 2016
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"

cSymbolTable g_SymbolTable;
long long cSymbol::nextId = 0;

int main(int argc, char **argv)
{
    const char *outfile_name;
    int result = 0;

    std::cout << "Devon Andrade" << std::endl;

    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL)
        {
            std::cerr << "Unable to open file " << argv[1] << "\n";
            exit(-1);
        }
    }

    if (argc > 2)
        outfile_name = argv[2];
    else
        outfile_name = "/dev/tty";

    std::streambuf *cout_buf = std::cout.rdbuf();
    std::ofstream output(outfile_name);

    if (!output.is_open())
    {
        std::cerr << "Unable to open output file " << outfile_name << "\n";
        exit(-1);
    }
    std::cout.rdbuf(output.rdbuf());

    result = yyparse();
    if (yyast_root != nullptr)
    {
        if (result == 0)
        {
            output << yyast_root->ToString() << std::endl;
        } else {
            output << yynerrs << " Errors in compile\n";
        }
    }

    if (result == 0 && yylex() != 0)
    {
        std::cout << "Junk at end of program\n";
    }

    output.close();
    std::cout.rdbuf(cout_buf);

    return result;
}
