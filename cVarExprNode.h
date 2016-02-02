#pragma once
//**************************************
// cVarExprNode.h
//
// Variable reference node
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cExprNode.h"
#include "cSymbol.h"

class cVarExprNode : public cExprNode
{
    public:
        // Variable declaration node
        cVarExprNode(cSymbol* name) : cExprNode() 
        {
            AddChild(name);
        }

    virtual string NodeType() { return string("varref"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
