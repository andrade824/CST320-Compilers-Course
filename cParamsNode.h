#pragma once
//**************************************
// cParamsNode.h
//
// Defines a class to represent a list of arguments.
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cParamsNode : public cAstNode
{
    public:
        // Constructor for a list of arguments node
        cParamsNode(cDeclNode *decl) : cAstNode()
        {
            AddChild(decl);
        }

        // Add an argument to the list
        void Insert(cDeclNode *decl)
        {
            AddChild(decl);
        }

        virtual string NodeType() { return string("args"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
