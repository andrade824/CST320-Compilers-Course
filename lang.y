%{
//**************************************
// lang.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Devon Andrade 
// devon.andrade@oit.edu
//
// Date: Jan. 26, 2015
//

#include <iostream>
#include "lex.h"
#include "astnodes.h"

%}

%locations

 /* define the type used for returning lexical values */
%union{
    int             int_val;
    double          float_val;
    cAstNode*       ast_node;
    cProgramNode*   program_node;
    cBlockNode*     block_node;
    cStmtsNode*     stmts_node;
    cStmtNode*      stmt_node;
    cExprNode*      expr_node;
    cSymbol*        symbol;
    cSymbolTable::symbolTable_t*   sym_table;
    cDeclNode*      decl_node;
    cDeclsNode*     decls_node;
    cVarExprNode*   var_ref;
    cFuncDeclNode*  func_decl;
    cParamsNode*    args_node;
    cParamListNode* params_node;
    }

%{
    // function to call when errors happen
    int yyerror(const char *msg);

    // Root of the AST node. Returned by the parser on success.
    cAstNode *yyast_root;
%}

%start  program

%token <symbol>    IDENTIFIER
%token <symbol>    TYPE_ID
%token <int_val>   CHAR_VAL
%token <int_val>   INT_VAL
%token <float_val> FLOAT_VAL

%token  PRINT
%token  WHILE IF ELSE ENDIF
%token  STRUCT
%token  RETURN
%token  AND OR EQUALS
%token  JUNK_TOKEN

 /* NOTE: you will have to change some of these as you complete your lab */
%type <program_node> program
%type <block_node> block
%type <sym_table> open
%type <sym_table> close
%type <decls_node> decls
%type <decl_node> decl
%type <decl_node> var_decl
%type <decl_node> struct_decl
%type <decl_node> func_decl
%type <func_decl>  func_header
%type <func_decl>  func_prefix
%type <expr_node> func_call
%type <args_node> paramsspec
%type <decl_node> paramspec
%type <stmts_node> stmts
%type <stmt_node> stmt
%type <var_ref> lval
%type <params_node> params
%type <expr_node> param
%type <expr_node> expr
%type <expr_node> term
%type <expr_node> fact
%type <var_ref> varref
%type <symbol> varpart

%%

program: block                  { $$ = new cProgramNode($1);
                                  yyast_root = $$;
                                  if (yynerrs == 0) 
                                      YYACCEPT;
                                  else
                                      YYABORT;
                                }
block:  open decls stmts close  { $$ = new cBlockNode($2, $3); }
    |   open stmts close        { $$ = new cBlockNode(nullptr, $2); }

open:   '{'                     { $$ = g_SymbolTable.IncreaseScope(); }

close:  '}'                     { $$ = g_SymbolTable.DecreaseScope(); }

decls:      decls decl          {
                                    $1->Insert($2);
                                    $$ = $1;
                                }
        |   decl                { $$ = new cDeclsNode($1); }

decl:       var_decl ';'        { $$ = $1; }
        |   struct_decl ';'     { $$ = $1; }
        |   func_decl           {}
        |   error ';'           {}

var_decl:   TYPE_ID IDENTIFIER  { $$ = new cVarDeclNode($1, $2); }
var_decl:   STRUCT IDENTIFIER IDENTIFIER { $$ = new cVarDeclNode($2, $3); }

struct_decl:  STRUCT open decls close IDENTIFIER    
                                { $$ = new cStructDeclNode($2, $3, $5); }
func_decl:  func_header ';'
                                {
                                    g_SymbolTable.DecreaseScope();
                                    $$ = $1;
                                }
        |   func_header  '{' decls stmts '}'
                                {
                                    $1->Insert($3);
                                    $1->Insert($4);
                                    g_SymbolTable.DecreaseScope();
                                    $$ = $1;
                                }
        |   func_header  '{' stmts '}'
                                {
                                    $1->Insert($3);
                                    g_SymbolTable.DecreaseScope();
                                    $$ = $1;
                                }
func_header: func_prefix paramsspec ')'
                                {
                                    $1->Insert($2); 
                                    $$ = $1; 
                                }
        |    func_prefix ')'    { $$ = $1; }
func_prefix: TYPE_ID IDENTIFIER '('
                                {
                                    $$ = new cFuncDeclNode($1, $2);
                                    g_SymbolTable.IncreaseScope();
                                }
paramsspec:     
            paramsspec',' paramspec 
                                { $1->Insert($3); }
        |   paramspec           { $$ = new cParamsNode($1); }

paramspec:  var_decl            { $$ = $1; }

stmts:      stmts stmt          {
                                    $1->Insert($2);
                                    $$ = $1;
                                }
        |   stmt                { $$ = new cStmtsNode($1); }

stmt:       IF '(' expr ')' stmts ENDIF ';'
                                { $$ = new cIfNode($3, $5); }
        |   IF '(' expr ')' stmts ELSE stmts ENDIF ';'
                                { $$ = new cIfNode($3, $5, $7); }
        |   WHILE '(' expr ')' stmt
                                { $$ = new cWhileNode($3, $5); }
        |   PRINT '(' expr ')' ';'
                                { $$ = new cPrintNode($3); }
        |   lval '=' expr ';'   { $$ = new cAssignNode($1, $3); }
        |   lval '=' func_call ';'   { $$ = new cAssignNode($1, $3); }
        |   func_call ';'       { $$ = $1; }
        |   block               { $$ = $1; }
        |   RETURN expr ';'     { $$ = new cReturnNode($2); }
        |   error ';'           {}

func_call:  IDENTIFIER '(' params ')' { $$ = new cFuncExprNode($1, $3); }
        |   IDENTIFIER '(' ')'  { $$ = new cFuncExprNode($1, nullptr); }

varref:   varref '.' varpart    { $1->AddChild($3); }
        | varpart               { $$ = new cVarExprNode($1); }

varpart:  IDENTIFIER            { $$ = $1; }

lval:     varref                { $$ = $1; }

params:     params',' param     { $1->Insert($3); }
        |   param               { $$ = new cParamListNode($1); }

param:      expr                { $$ = $1; }

expr:       expr '+' term       { $$ = new cBinaryExprNode($1, new cOpNode('+'), $3); }
        |   expr '-' term       { $$ = new cBinaryExprNode($1, new cOpNode('-'), $3); }
        |   term                { $$ = $1; }

term:       term '*' fact       { $$ = new cBinaryExprNode($1, new cOpNode('*'), $3); }
        |   term '/' fact       { $$ = new cBinaryExprNode($1, new cOpNode('/'), $3); }
        |   term '%' fact       { $$ = new cBinaryExprNode($1, new cOpNode('%'), $3); }
        |   fact                { $$ = $1; }

fact:       '(' expr ')'        { $$ = $2; }
        |   INT_VAL             { $$ = new cIntExprNode($1); }
        |   FLOAT_VAL           { $$ = new cFloatExprNode($1); }
        |   varref              { $$ = $1; }

%%

// function for formatting error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}
