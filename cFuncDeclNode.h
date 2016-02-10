#pragma once
//**************************************
// cFuncDeclNode.h
//
// Function declaration node
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cDeclsNode.h"
#include "cSymbolTable.h"
#include "cSymbol.h"
#include "cParamsNode.h"
#include "cStmtsNode.h"

class cFuncDeclNode : public cDeclNode
{
    public:
        // Function declaration node
        cFuncDeclNode(cSymbol *returntype, cSymbol* name) : cDeclNode() 
        {
            // Create a new symbol for the innermost scope
            // if a symbol already exists in an outer scope somewhere
            if(g_SymbolTable.Find(name->GetName()))
                name = new cSymbol(name->GetName());

            AddChild(returntype);
            AddChild(name);
            g_SymbolTable.Insert(name);
        }

        // Add the paramaters to the list
        void Insert(cParamsNode *params)
        {
            AddChild(params);
        }

        // Add local variable declarations to the list
        void Insert(cDeclsNode *decls)
        {
            AddChild(decls);
        }

        // Add the function statements to the list
        void Insert(cStmtsNode *stmts)
        {
            AddChild(stmts);
        }

        // Return back this func decl as the type
        virtual cDeclNode * GetType() { return this; }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
