#pragma once
//**************************************
// cIntExprNode.h
//
// Defines an AST node for an integer constant (literals).
//
// Inherits from cExprNode so that integer constants can be used anywhere 
// expressions are used.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2015
//

#include <climits>
#include "cAstNode.h"
#include "cExprNode.h"
#include "cDeclNode.h"
#include "cSymbolTable.h"

class cIntExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cIntExprNode(int value) : cExprNode()
        {
            m_value = value;
        }

        virtual string AttributesToString() 
        {
            return " value=\"" + std::to_string(m_value) + "\"";
        }
        
        // Return the type of the variable this expression represents
        virtual cDeclNode * GetType() 
        {
            if(m_value >= CHAR_MIN && m_value <= CHAR_MAX)
                return g_SymbolTable.Find("char")->GetDecl();
            else
                return g_SymbolTable.Find("int")->GetDecl();
        }

        virtual string NodeType() { return string("int"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_value;        // value of integer constant (literal)
};
