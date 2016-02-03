#pragma once
//**************************************
// cWhileNode.h
//
// While statement node
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cStmtNode.h"
#include "cExprNode.h"

class cWhileNode : public cStmtNode
{
    public:
        // While Node Constructor
        cWhileNode(cExprNode *val, cStmtNode *stmt) : cStmtNode() 
        {
            AddChild(val);
            AddChild(stmt);
        }

        virtual string NodeType() { return string("while"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
