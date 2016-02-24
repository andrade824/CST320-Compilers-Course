#pragma once
//**************************************
// cBaseTypeNode.h
//
// Base type declaration node
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 9, 2016
//
#include <string>
#include "cDeclNode.h"
#include "cSymbolTable.h"
#include "cSymbol.h"
#include "semantic.h"

using std::string;

class cBaseTypeNode : public cDeclNode
{
    public:
        // Base type declaration node
        cBaseTypeNode(string name, int size, bool isFloat) 
            : cDeclNode(), m_name(name), m_isFloat(isFloat)
        {
            m_size = size;
        }

        // Declaration information getters
        virtual bool isFloat() { return m_isFloat; }

        // Return back the name of this declaration
        virtual string GetTypeName() { return m_name; }

        // Return back this base type as the type
        virtual cDeclNode * GetType() { return this; }

        virtual string NodeType() { return string(""); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        string m_name;
        bool m_isFloat;
};
