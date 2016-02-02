#pragma once
//**************************************
// lex.h
//
// Defines global symbols used the the flex generated lexer and 
// bison generated parser.
//
// Author: Phil Howard phil.howard@oit.edu
//

#include "cSymbolTable.h"
#include "cAstNode.h"

// parse function generated by bison
int yyparse();

// lexer function generated by flex
int yylex();

extern cSymbolTable g_SymbolTable;
extern char *yytext;                    // text of current token
extern int yylineno;                    // line number of input file
extern int yynerrs;                     // Num of compile errors
extern FILE *yyin;                      // FILE pointer for input
extern cAstNode *yyast_root;            // root of AST
