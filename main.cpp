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
#include <string>
#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "cComputeSize.h"
#include "cCodeGen.h"
using std::string;

cSymbolTable g_SymbolTable;
bool g_semanticErrorHappened = false;
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
    {
        outfile_name = argv[2];
    } else {
        //outfile_name = "/dev/tty";
        outfile_name = "langout";
    }

    /*
    std::ofstream output(outfile_name);
    if (!output.is_open())
    {
        std::cerr << "ERROR: Unable to open file " << outfile_name << "\n";
        exit(-1);
    }

    // fixup cout so it redirects to output
    std::cout.rdbuf(output.rdbuf());
    */

    g_SymbolTable.InitRootTable();

    result = yyparse();
    if (yyast_root != nullptr)
    {
        if (result == 0)
        {
            cComputeSize sizer;
            sizer.VisitAllNodes(yyast_root);

            //output << yyast_root->ToString() << std::endl;

            cCodeGen coder(string(outfile_name) + ".c");
            coder.VisitAllNodes(yyast_root);

            /*
            string cmd = "gcc -g -O0 -o " + 
                outfile_name + " " + outfile_name + ".c";
            system(cmd.c_str());
            */
        } else {
            std::cerr << yynerrs << " Errors in compile\n";
        }
    }

    if (result == 0 && yylex() != 0)
    {
        std::cerr << "Junk at end of program\n";
    }

    /*
    // close output and fixup cout
    // If these aren't done, you may get a segfault on program exit
    output.close();
    std::cout.rdbuf(cout_buf);
    */

    return result;
}
