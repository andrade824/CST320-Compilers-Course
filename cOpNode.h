#pragma once
//**************************************
// cOpNode.h
//
// Class for an operator (e.g. + - etc.)
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 5, 2016
//

#include <string>
#include "cAstNode.h"
using std::string;

class cOpNode : public cAstNode
{
    public:
        cOpNode(string op) : cAstNode()
        {
            m_op = op;
        }

        virtual string NodeType()       { return "op"; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString()   
        { 
            return string(" value='") + m_op + "'";
        }
        
    protected:
        string m_op;      // the operand
};

