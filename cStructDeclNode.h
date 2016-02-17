#pragma once
//**************************************
// cStructDeclNode.h
//
// Structure declaration node
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include <string>
#include "cDeclsNode.h"
#include "cDeclNode.h"
#include "cSymbolTable.h"
#include "cSymbol.h"

using std::string;

class cStructDeclNode : public cDeclNode
{
    public:
        // Variable declaration node
        cStructDeclNode(cSymbolTable::symbolTable_t *scope, cDeclsNode *decls, cSymbol* name) 
            : cDeclNode(), m_name(name->GetName()), table(scope)
        {
            // Create a new symbol for the innermost scope
            // if a symbol already exists in an outer scope somewhere
            if(g_SymbolTable.Find(name->GetName()))
                name = new cSymbol(name->GetName());

            name->SetDecl(this);
            AddChild(decls);
            AddChild(name);
            g_SymbolTable.Insert(name);
        }

        // Return back a symbol (field) in the struct's symbol table
        cSymbol * GetField(const string& name)
        {
            return g_SymbolTable.FindInTable(table, name);
        }

        // Declaration information getters
        virtual bool isStruct() { return true; }

        // Return back the identifier for this struct declaration
        virtual string GetTypeName() { return m_name; }

        // Return back this struct decl as the type
        virtual cDeclNode * GetType() { return this; }

        virtual string NodeType() { return string("struct_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

protected:
    string m_name;
    cSymbolTable::symbolTable_t *table;   // The symbol table that contains the structure field symbols
};
