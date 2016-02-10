#pragma once
//**************************************
// cVarExprNode.h
//
// Variable reference node
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//
#include <string>
#include "cExprNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"
#include "cStructDeclNode.h"
#include "semantic.h" 
using std::string;

/**
 * 1. Do i need weird temp string for error messages
 * 2. If statement in VarDeclNode
 * 3. Should cStructDeclNode have isVar return true
 */

class cVarExprNode : public cExprNode
{
    public:
        // Variable declaration node
        cVarExprNode(cSymbol* name) : cExprNode() 
        {
            if(g_SymbolTable.Find(name->GetName()) == nullptr)
                SemanticError("Symbol " + name->GetName() + " not defined");
            
            AddChild(name);
        }

        // Insert a child into this ASTNode
        void Insert(cSymbol *symbol)
        {
            // Get the cSymbol for the base of this "struct"
            cSymbol *base = dynamic_cast<cSymbol*>(GetChild(NumChildren() - 1));
            
            // Make sure the base is a struct
            if(base->GetDecl()->isStruct())
            {
                cStructDeclNode *struct_decl = dynamic_cast<cStructDeclNode *>(base->GetDecl());

                // Check if the symbol is a field of the struct
                cSymbol *elem = g_SymbolTable.FindInTable(struct_decl->GetSymbolTable(), symbol->GetName());
                if(elem == nullptr)
                {
                    string temp = "";

                    for(int i = 0; i < NumChildren() - 1; ++i)
                        temp += dynamic_cast<cSymbol *>(GetChild(i))->GetName() + ".";

                    temp += base->GetName();
                    SemanticError(symbol->GetName() + " is not a field of " + temp);
                }
                else
                {
                    // We don't want the dummy symbol created by the scanner
                    // Use the symbol in the struct's decls
                    delete symbol;
                    AddChild(elem);
                }
            }
            else
            {
                string temp = "";

                for(int i = 0; i < NumChildren() - 1; ++i)
                    temp += dynamic_cast<cSymbol *>(GetChild(i))->GetName() + ".";

                temp += base->GetName();
                SemanticError(temp + " is not a struct");
            }
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
