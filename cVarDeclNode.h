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
#include "semantic.h"

class cVarDeclNode : public cDeclNode
{
    public:
        // Variable declaration node
        cVarDeclNode(cSymbol *type, cSymbol* name) : cDeclNode() 
        {
            if(g_SymbolTable.FindLocal(name->GetName()))
            {
                SemanticError("Symbol " + name->GetName() + " already defined in current scope");
            }
            else
            {
                // Create a new symbol for the innermost scope
                // if a symbol already exists in an outer scope somewhere
                if(g_SymbolTable.Find(name->GetName()))
                    name = new cSymbol(name->GetName());

                // If the type is a struct, pass the declaration on
                if(type->GetDecl()->isStruct())
                    name->SetDecl(type->GetDecl());
                else
                    name->SetDecl(this);

                g_SymbolTable.Insert(name);
            }

            AddChild(type);
            AddChild(name);
        }

        // Declaration information getters
        virtual bool isVar() { return true; }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
