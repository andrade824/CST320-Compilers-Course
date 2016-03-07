#pragma once
//**************************************
// cBinaryExprNode.h
//
// Defines base class for all expressions
//
// This is a pure virtual class because there is no definition for
// cAstNode::ToString()
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cStmtNode.h"
#include "cOpNode.h"
#include "cDeclNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        // Expression node that has parameters
        cBinaryExprNode(cExprNode *expr1, cOpNode *op, cExprNode *expr2) : cExprNode(), m_type(nullptr) 
        {
            AddChild(expr1);
            AddChild(op);
            AddChild(expr2);

            // if expr2 can be upcasted to expr1, then do so
            if(expr1->isCompatibleWith(expr2))
                m_type = expr1->GetType();
            else
                m_type = expr2->GetType();
        }

        // Return the type of the variable this expression represents
        virtual cDeclNode * GetType() { return m_type; }
        virtual cDeclNode * GetDecl() { return nullptr; }
        
        // Getters
        cExprNode * GetOperand1() { return (cExprNode*)GetChild(0); }
        cOpNode * GetOperator() { return (cOpNode*)GetChild(1); }
        cExprNode * GetOperand2() { return (cExprNode*)GetChild(2); }

        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    private:
        cDeclNode *m_type;
};
