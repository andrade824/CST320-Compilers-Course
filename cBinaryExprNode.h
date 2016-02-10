#pragma once
//**************************************
// cBinaryExprNode.h
//
// Defines base class for all expressions
//
// This is a pure virtual class because there is no definition for
// cAstNode::ToString()
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cStmtNode.h"
#include "cOpNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        // Expression node that has parameters
        cBinaryExprNode(cExprNode *expr1, cOpNode *op, cExprNode *expr2) : cExprNode() 
        {
            AddChild(expr1);
            AddChild(op);
            AddChild(expr2);
        }

        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
