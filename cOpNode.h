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
#include "langparse.h"
#include "cAstNode.h"
using std::string;

class cOpNode : public cAstNode
{
    public:
        cOpNode(int op) : cAstNode()
        {
            m_op = op;
        }

        // Returns back the string representation of the opcode
        string GetOp()
        {
            string temp;

            switch(m_op)
            {
                case EQUALS: temp = "=="; break;
                case NOTEQUALS: temp = "!="; break;

                default: temp = static_cast<char>(m_op); break;
            }

            return temp;
        }

        virtual string NodeType()       { return "op"; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString() { return " value='" + GetOp() + "'"; }
        
    protected:
        int m_op;      // the operand
};

