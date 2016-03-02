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