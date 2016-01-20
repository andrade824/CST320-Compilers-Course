%{
//**************************************
// lang.l
//
// Scanner definition file for CST 320 project
//
// Author: Devon Andrade 
// devon.andrade@oit.edu
//
// Date: Jan. 5, 2016
//

#include "tokens.h"
#include "lex.h"

#define DO_RETURN(a) return (a)
//#define DO_RETURN(a) return Return(a)
int Return(int val);

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


    /* definitions go here. period is an example. */
digit   -?[0-9]
comment     "//".*$
whitespace [\t\n ]+
identifier [a-zA-Z_][a-zA-Z0-9_]*
%%

    // token definitions go here. "if" is an example.
"("                 DO_RETURN('(');
")"                 DO_RETURN(')');
"{"                 {
                        g_SymbolTable.IncreaseScope();
                        return yytext[0];
                    }
"}"                 {
                        g_SymbolTable.DecreaseScope();
                        return yytext[0];
                    }
"["                 DO_RETURN('[');
"]"                 DO_RETURN(']');
";"                 DO_RETURN(';');
","                 DO_RETURN(',');
"."                 DO_RETURN('.');
"+"                 DO_RETURN('+');
"-"                 DO_RETURN('-');
"*"                 DO_RETURN('*');
"/"                 DO_RETURN('/');
"%"                 DO_RETURN('%');
"="                 DO_RETURN('=');
"=="                DO_RETURN(EQUALS);
"&&"                DO_RETURN(AND);
"||"                DO_RETURN(OR);
"if"                DO_RETURN(IF);
"endif"             DO_RETURN(ENDIF);
"else"              DO_RETURN(ELSE);
"while"             DO_RETURN(WHILE);
"print"             DO_RETURN(PRINT);
"int"               DO_RETURN(INT);
"float"             DO_RETURN(FLOAT);
"char"              DO_RETURN(CHAR);
"struct"            DO_RETURN(STRUCT);
"return"            DO_RETURN(RETURN);
{identifier}        {
                        yylval.symbol = g_SymbolTable.Insert(yytext);
                        return IDENTIFIER;
                    }
{comment}           {}
{whitespace}        {}
{digit}+            DO_RETURN(INT_VAL);
{digit}+"."{digit}+ DO_RETURN(FLOAT_VAL);
.                   DO_RETURN(JUNK_TOKEN);
%%

// definition of global yylval
yylval_t yylval;

// This function allows us to do extra processing on each token
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

