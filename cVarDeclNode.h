#pragma once
//**************************************
// cVarDeclNode.h
//
// Variable declaration node
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cDeclNode.h"
#include "cSymbolTable.h"
#include "cSymbol.h"

class cVarDeclNode : public cDeclNode
{
    public:
        // Variable declaration node
        cVarDeclNode(cSymbol *type, cSymbol* name) : cDeclNode() 
        {
            // Create a new symbol for the innermost scope
            // if a symbol already exists in an outer scope somewhere
            if(g_SymbolTable.Find(name->GetName()))
                name = new cSymbol(name->GetName());

            AddChild(type);
            AddChild(name);
            g_SymbolTable.Insert(name);
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
