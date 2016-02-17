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
#include "semantic.h"

class cAssignNode : public cStmtNode
{
    public:
        // Assign Node Constructor
        cAssignNode(cVarExprNode *lval, cExprNode *rval) : cStmtNode() 
        {
            // Check to make sure the types match
            if(!lval->isCompatibleWith(rval))
                SemanticError("Cannot assign " + rval->GetType()->GetTypeName() + " to " + lval->GetType()->GetTypeName());

            AddChild(lval);
            AddChild(rval);
        }

        virtual string NodeType() { return string("assign"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
