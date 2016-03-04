#include <string>
#include "lex.h"
#include "cCodeGen.h"
#include "astnodes.h"
#include "emit.h"

using std::to_string;

// Constructor that accepts the filename of the output code
cCodeGen::cCodeGen(const string filename) : cVisitor()
{
    InitOutput(filename);
}

cCodeGen::~cCodeGen()
{
    FinalizeOutput();
}

void cCodeGen::EmitSemicolon()
{
    EmitString(";\n");
}

/** Visit Functions for various node types **/
void cCodeGen::VisitAllNodes(cAstNode *node)
{
    node->Visit(this);
}

void cCodeGen::Visit(cProgramNode *node)
{
    EmitString("Stack_Pointer += " + to_string(node->GetBlock()->GetSize()));
    EmitSemicolon();

    VisitAllChildren(node);
}

void cCodeGen::Visit(cIntExprNode *node)
{
    EmitInt(node->GetValue());
}

void cCodeGen::Visit(cFloatExprNode *node)
{
    EmitFloat(node->GetValue());
}

void cCodeGen::Visit(cPrintNode *node)
{
    if(node->GetExpr()->GetType()->isFloat())
    {
        EmitString("Temp_F = ");
        node->GetExpr()->Visit(this);
        EmitSemicolon();
        EmitPrintTemp_F();
    }
    else
    {
        EmitString("Temp = ");
        node->GetExpr()->Visit(this);
        EmitSemicolon();
        EmitPrintTemp();
    }
}

void cCodeGen::Visit(cAssignNode *node)
{
    node->GetLval()->Visit(this);
    EmitString(" = ");
    node->GetRval()->Visit(this);
    EmitSemicolon();
}

void cCodeGen::Visit(cVarExprNode *node)
{
    if(node->GetType()->isFloat())
        EmitString("*(double *)&Memory[Frame_Pointer + " + to_string(node->GetOffset()) + "]");
    else if(node->GetSize() == 1)
        EmitString("Memory[Frame_Pointer + " + to_string(node->GetOffset()) + "]");
    else
        EmitString("*(int *)&Memory[Frame_Pointer + " + to_string(node->GetOffset()) + "]");
}

void cCodeGen::Visit(cBinaryExprNode *node)
{
    node->GetOperand1()->Visit(this);
    EmitString(node->GetOperator()->GetOp());
    node->GetOperand2()->Visit(this);
}

void cCodeGen::Visit(cFuncExprNode *node)
{
    
}

void cCodeGen::Visit(cIfNode *node)
{
    string endlabel = GenerateLabel();
    string elselabel;

    // Check if this IF statement has an else clause
    if(node->NumChildren() == 3)
    {
        elselabel = GenerateLabel();

        EmitString("if((");
        node->GetCond()->Visit(this);
        EmitString(") == 0) goto " + elselabel + ";\n");
        node->GetStmts()->Visit(this);
        EmitString("goto " + endlabel + ";\n");
        EmitString(elselabel + ":\n");
        node->GetElseStmts()->Visit(this);
        EmitString(endlabel + ":\n");
    }
    else
    {
        // No else clause
        EmitString("if((");
        node->GetCond()->Visit(this);
        EmitString(") == 0) goto " + endlabel + ";\n");
        node->GetStmts()->Visit(this);
        EmitString(endlabel + ":\n");
    }
}

void cCodeGen::Visit(cWhileNode *node)
{
    string looplabel = GenerateLabel();
    string endlabel = GenerateLabel();

    EmitString(looplabel + ":\n");
    EmitString("if((");
    node->GetCond()->Visit(this);
    EmitString(") == 0) goto " + endlabel + ";\n");
    node->GetStmt()->Visit(this);
    EmitString("goto " + looplabel + ";\n");
    EmitString(endlabel + ":\n");
}

void cCodeGen::Visit(cFuncDeclNode *node)
{
    // Maybe not do anything if stmts and decls are null (aka, this was a function prototype)?
    
    m_func_rtn_label = GenerateLabel();
    StartFunctionOutput(node->GetTypeName());

    // Increment Stack

    // Only print decls/stmts when they exist
    if(node->GetDecls())
        node->GetDecls()->Visit(this);

    if(node->GetStmts())
        node->GetStmts()->Visit(this);

    // Decrement stack

    EmitString(m_func_rtn_label + ":\n");
    EndFunctionOutput();
}

void cCodeGen::Visit(cReturnNode *node)
{
    EmitString("Temp = ");
    node->GetValue()->Visit(this);
    EmitSemicolon();
    EmitString("goto " + m_func_rtn_label + ";\n");
}