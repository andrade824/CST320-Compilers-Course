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

// Push an expression onto the stack
void cCodeGen::PushExpression(cExprNode *node)
{
    if(node->GetType()->isFloat())
    {
        EmitString("*(double *)&Memory[Stack_Pointer] = ");
        node->Visit(this);
        EmitSemicolon();
        EmitString("Stack_Pointer += 8;\n");
    }
    else
    {
        EmitString("*(int *)&Memory[Stack_Pointer] = ");
        node->Visit(this);
        EmitSemicolon();
        EmitString("Stack_Pointer += 4;\n");
    }
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
    if(node->GetRval()->GetDecl() != nullptr && node->GetRval()->GetDecl()->isFunc())
    {
        node->GetRval()->Visit(this);
        node->GetLval()->Visit(this);

        if(node->GetRval()->GetType()->isFloat())
            EmitString(" = Temp_F;\n");
        else
            EmitString(" = Temp;\n");
    }
    else
    {
        node->GetLval()->Visit(this);
        EmitString(" = ");
        node->GetRval()->Visit(this);
        EmitSemicolon();
    }
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
    EmitString("*(int *)&Memory[Stack_Pointer] = Frame_Pointer;\n");
    EmitString("Stack_Pointer += 4;\n");
    EmitString("Frame_Pointer = Stack_Pointer;\n");

    // Push parameters onto stack
    if(node->GetParams() != nullptr)
    {
        for(int i = 0; i < node->GetParams()->NumChildren(); ++i)
            PushExpression(node->GetParams()->GetExpr(i));
    }

    EmitString(node->GetName() + "();\n");

    if(node->GetParams() != nullptr)
        EmitString("Stack_Pointer -= " + to_string(node->GetFuncDecl()->GetParams()->GetSize()) + ";\n");
    
    EmitString("Stack_Pointer -= 4;\n");
    EmitString("Frame_Pointer = *(int *)&Memory[Stack_Pointer];\n");
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
    // Don't do anything if stmts and decls are null (aka, this was a function prototype)
    if(node->GetDecls() || node->GetStmts())
    {
        m_func_rtn_label = GenerateLabel();
        StartFunctionOutput(node->GetTypeName());

        // Increment Stack
        if(node->GetDecls())
            EmitString("Stack_Pointer += " + to_string(node->GetDecls()->GetSize()) + ";\n");

        // Only print decls/stmts when they exist
        if(node->GetDecls())
            node->GetDecls()->Visit(this);

        if(node->GetStmts())
            node->GetStmts()->Visit(this);

        // Decrement stack
        if(node->GetDecls())
            EmitString("Stack_Pointer -= " + to_string(node->GetDecls()->GetSize()) + ";\n");

        EmitString(m_func_rtn_label + ":\n");
        EndFunctionOutput();
    }
}

void cCodeGen::Visit(cReturnNode *node)
{
    if(node->GetValue()->GetType()->isFloat())
        EmitString("Temp_F = ");
    else
        EmitString("Temp = ");
    
    node->GetValue()->Visit(this);
    EmitSemicolon();
    EmitString("goto " + m_func_rtn_label + ";\n");
}