#pragma once
//**************************************
// cAssignNode.h
//
// Assign statement node
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cVarExprNode.h"
#include "cExprNode.h"

class cAssignNode : public cStmtNode
{
    public:
        // Assign Node Constructor
        cAssignNode(cVarExprNode *lval, cExprNode *rval) : cStmtNode() 
        {
            AddChild(lval);
            AddChild(rval);
        }

    virtual string NodeType() { return string("assign"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
