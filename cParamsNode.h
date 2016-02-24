#pragma once
//**************************************
// cParamsNode.h
//
// Defines a class to represent a list of arguments.
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cParamsNode : public cAstNode
{
    public:
        // Constructor for a list of arguments node
        cParamsNode(cDeclNode *decl) : cAstNode(), m_size(0)
        {
            AddChild(decl);
        }

        // Add an argument to the list
        void Insert(cDeclNode *decl)
        {
            AddChild(decl);
        }

        // Getters and Setters
        int GetSize() { return m_size; }
        void SetSize(int size) { m_size = size; }

        virtual string NodeType() { return string("args"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    
        virtual string AttributesToString() 
        {
            if(m_size)
                return " size=\"" + std::to_string(m_size) + "\"";
            else
                return "";
        }

    private:
        int m_size;
};
