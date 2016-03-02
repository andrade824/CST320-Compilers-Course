#pragma once
#include <string>
#include "cVisitor.h"
#include "emit.h"

using std::string;

class cCodeGen : public cVisitor
{
public:
    // Constructor that accepts the filename of the output code
    cCodeGen(const string filename);

    virtual void VisitAllNodes(cAstNode *node);

    // Visit functions
    virtual void Visit(cIntExprNode *node);
    virtual void Visit(cFloatExprNode *node);
    virtual void Visit(cPrintNode *node);
    virtual void Visit(cProgramNode *node);
    virtual void Visit(cAssignNode *node);
    virtual void Visit(cVarExprNode *node);
    virtual void Visit(cBinaryExprNode *node);
    virtual void Visit(cFuncExprNode *node);
    virtual void Visit(cIfNode *node);

    // Destructor
    ~cCodeGen();

private:
    void EmitSemicolon();};
