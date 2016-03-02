#pragma once
//**************************************
// cIfNode.h
//
// If statement node
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cStmtsNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"

class cIfNode : public cStmtNode
{
    public:
        // If Node without an ELSE clause Constructor
        cIfNode(cExprNode *cond, cStmtsNode *stmts) : cStmtNode() 
        {
            AddChild(cond);
            AddChild(stmts);
        }

        // If Node with ELSE Constructor
        cIfNode(cExprNode *cond, cStmtsNode *ifstmts, cStmtsNode *elsestmts) : cStmtNode() 
        {
            AddChild(cond);
            AddChild(ifstmts);
            AddChild(elsestmts);
        }

        // Getters
        cExprNode * GetCond() { return (cExprNode*)GetChild(0); }
        cStmtsNode * GetStmts() { return (cStmtsNode*)GetChild(1); }
        cStmtsNode * GetElseStmts() { return (cStmtsNode*)GetChild(2); }

        virtual string NodeType() { return string("if"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
