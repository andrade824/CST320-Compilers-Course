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

class cVarExprNode : public cExprNode
{
    public:
        // Variable declaration node
        cVarExprNode(cSymbol* name) : cExprNode(), m_size(0), m_offset(0)
        {
            cSymbol *var = g_SymbolTable.Find(name->GetName());
            if(var == nullptr)
                SemanticError("Symbol " + name->GetName() + " not defined");
            else if(var->GetDecl()->isFunc())
                SemanticError(var->GetName() + " is not an lval");

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
                    AddChild(elem);
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

        // Return the cVarDeclNode of the variable this expression represents
        virtual cDeclNode * GetDecl() 
        {
            cSymbol *var = dynamic_cast<cSymbol*>(GetChild(NumChildren() - 1));
            return var->GetDecl();
        }

        // Return the type of the variable this expression represents
        virtual cDeclNode * GetType() 
        {
            cSymbol *var = dynamic_cast<cSymbol*>(GetChild(NumChildren() - 1));
            return var->GetDecl()->GetType();
        }

        // Return back a cSymbol child node
        cSymbol * GetSymbol(int num)
        {
            return dynamic_cast<cSymbol*>(GetChild(num));
        }

        // Size and offset getters/setters
    	virtual int GetSize() {return m_size; }
        virtual void SetSize(int size) { m_size = size; }
        virtual int GetOffset() { return m_offset; }
        virtual void SetOffset(int offset) { m_offset = offset; }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString() 
        {
            if(!m_size && !m_offset)
                return "";
            else
                return " size=\"" + std::to_string(m_size) + "\" offset=\"" + std::to_string(m_offset) + "\"";
        }

    private:
        int m_size;
        int m_offset;
};
