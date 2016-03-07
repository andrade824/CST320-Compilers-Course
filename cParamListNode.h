#pragma once
//**************************************
// cParamList.h
//
// Defines a class to represent a list of parameters.
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cAstNode.h"
#include "cExprNode.h"

class cParamListNode : public cAstNode
{
    public:
        // Constructor for a list of parameters node
        cParamListNode(cExprNode *expr) : cAstNode()
        {
            AddChild(expr);
        }

        // Add a param to the list
        void Insert(cExprNode *expr)
        {
            AddChild(expr);
        }

        // Get the expressions stored within this parameter list
        cExprNode * GetExpr(int num) { return (cExprNode*) GetChild(num); }

        virtual string NodeType() { return string("params"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
