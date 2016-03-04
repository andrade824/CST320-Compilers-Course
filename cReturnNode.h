#pragma once
//**************************************
// cReturnNode.h
//
// Return statement node
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cStmtsNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"

class cReturnNode : public cStmtNode
{
    public:
        // Return Node Constructor
        cReturnNode(cExprNode *val) : cStmtNode() 
        {
            AddChild(val);
        }

        // Getters
        cExprNode * GetValue() { return (cExprNode*)GetChild(0); }

        virtual string NodeType() { return string("return"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
