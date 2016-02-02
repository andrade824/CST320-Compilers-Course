#pragma once
//**************************************
// cExprNode.h
//
// Defines base class for all expressions
//
// This is a pure virtual class because there is no definition for
// cAstNode::ToString()
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2015
//

#include "cStmtNode.h"
#include "cOpNode.h"

class cExprNode : public cStmtNode
{
    public:
    	// Default constructor
		cExprNode() : cStmtNode() {}

    	// Expression node that has parameters
        cExprNode(cExprNode *expr1, cOpNode *op, cExprNode *expr2) : cStmtNode() 
        {
        	AddChild(expr1);
        	AddChild(op);
        	AddChild(expr2);
        }

    virtual string NodeType() { return string("expr"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
