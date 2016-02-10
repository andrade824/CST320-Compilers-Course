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
        cBaseTypeNode(int size, bool isFloat) 
            : cDeclNode(), m_size(size), m_isFloat(isFloat)
        { }

        // Declaration information getters
        virtual bool isFloat() { return m_isFloat; }
        virtual int GetSize() { return m_size; }

        // Return back this base type as the type
        virtual cDeclNode * GetType() { return this; }

        virtual string NodeType() { return string(""); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_size;
        bool m_isFloat;
};
