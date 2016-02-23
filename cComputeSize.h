#pragma once
#include "cVisitor.h"

class cComputeSize : public cVisitor
{
public:
    cComputeSize() : cVisitor(), m_offset(0), m_highwater(0) {}

    virtual void VisitAllNodes(cAstNode *node);

    // Visit functions
    virtual void Visit(cBlockNode *node);
    virtual void Visit(cVarDeclNode *node);

private:
    int m_offset;
    int m_highwater;
};