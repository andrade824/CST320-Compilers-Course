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

#include "cDeclsNode.h"
#include "cSymbolTable.h"
#include "cSymbol.h"

class cStructDeclNode : public cDeclNode
{
    public:
        // Variable declaration node
        cStructDeclNode(cSymbolTable::symbolTable_t *scope, cDeclsNode *decls, cSymbol* name) 
            : cDeclNode(), table(scope)
        {
            // Create a new symbol for the innermost scope
            // if a symbol already exists in an outer scope somewhere
            if(g_SymbolTable.Find(name->GetName()))
                name = new cSymbol(name->GetName());

            AddChild(decls);
            AddChild(name);
            g_SymbolTable.Insert(name);
        }

        // Return back the symbol table that includes the structure member variable symbols
        cSymbolTable::symbolTable_t * GetSymbolTable() const { return table; }

        virtual string NodeType() { return string("struct_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

protected:
    cSymbolTable::symbolTable_t *table;   // The symbol table that contains the structure member symbols
};
