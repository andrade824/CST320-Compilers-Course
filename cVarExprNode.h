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
            if(base->GetDecl()->GetType()->isStruct())
            {
                cStructDeclNode *struct_decl = dynamic_cast<cStructDeclNode *>(base->GetDecl()->GetType());

                // Check if the symbol is a field of the struct
                cSymbol *elem = struct_decl->GetField(symbol->GetName());
                if(elem == nullptr)
                    SemanticError(symbol->GetName() + " is not a field of " + GetName());
                else
                {
                    // We don't want the dummy symbol created by the scanner
                    // Use the symbol in the struct's decls instead
                    delete symbol;
                    AddChild(elem);
                }
            }
            else
                SemanticError(GetName() + " is not a struct");
        }

        // Get the full name of this variable declaration (useful for struct variables)
        string GetName()
        {
            string temp = "";

            for(int i = 0; i < NumChildren() - 1; ++i)
                temp += dynamic_cast<cSymbol *>(GetChild(i))->GetName() + ".";

            temp += dynamic_cast<cSymbol *>(GetChild(NumChildren() - 1))->GetName();

            return temp;
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
