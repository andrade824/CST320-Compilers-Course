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

#include <string>
#include "cDeclsNode.h"
#include "cSymbolTable.h"
#include "cSymbol.h"
#include "cParamsNode.h"
#include "cStmtsNode.h"
#include "semantic.h"

using std::string;

class cFuncDeclNode : public cDeclNode
{
    public:
        // Function declaration node
        cFuncDeclNode(cSymbol *returntype, cSymbol* name) 
            : cDeclNode(), m_name(name), m_returntype(returntype), m_params(nullptr), m_other(nullptr), m_isDefined(false)
        {   
            // Make sure another symbol with the same name in the current scope doesn't exist
            cSymbol *other = g_SymbolTable.FindLocal(name->GetName());
            if(other != nullptr)
            {
                // Make sure that other symbol is a function
                if(other->GetDecl()->isFunc())
                {
                    cFuncDeclNode *otherFunc = dynamic_cast<cFuncDeclNode*>(other->GetDecl());
                    m_other = otherFunc;

                    if(otherFunc->GetType() != GetType())
                        SemanticError(name->GetName() + " previously defined with different return type");
                    else if(!otherFunc->isDefined())
                        name->SetDecl(this);
                }
                else
                    SemanticError(name->GetName() + " previously defined as other than a function");
            }
            else
            {
                // Create a new symbol for the innermost scope
                // if a symbol already exists in an outer scope somewhere
                if(g_SymbolTable.Find(name->GetName()))
                    name = new cSymbol(name->GetName());

                name->SetDecl(this);
                g_SymbolTable.Insert(name);
            }
            
            AddChild(returntype);
            AddChild(name);
        }
        
        // Add the paramaters to the list
        void InsertParams(cParamsNode *params)
        {
            // Check if there's another functon with the same name (and we have parameters)
            if(m_other && !(m_other->GetParams() == nullptr && params == nullptr))
            {
                // Check for same number of params, and whether the params are different (if same number)
                if((m_other->GetParams() == nullptr && params != nullptr) ||
                   (m_other->GetParams() != nullptr && params == nullptr) ||
                   (m_other->GetParams()->NumChildren() != params->NumChildren()))
                    SemanticError(m_name->GetName() + " redeclared with a different number of parameters");
                else
                {
                    bool different = false;

                    for(int i = 0; i < params->NumChildren() && !different; ++i)
                    {
                        cDeclNode *child = dynamic_cast<cDeclNode*>(params->GetChild(i));
                        cDeclNode *otherChild = dynamic_cast<cDeclNode*>(m_other->GetParams()->GetChild(i));

                        if(child->GetType() != otherChild->GetType())
                            different = true;
                    }

                    if(different)
                        SemanticError(m_name->GetName() + " previously defined with different parameters");
                }
            }

            AddChild(params);
            m_params = params;
        }

        // Add local variable declarations to the list
        void InsertDecls(cDeclsNode *decls)
        {
            AddChild(decls);
        }

        // Add the function statements to the list
        void InsertStmts(cStmtsNode *stmts)
        {
            cFuncDeclNode *func = dynamic_cast<cFuncDeclNode*>(m_name->GetDecl());
            if(stmts && !func->isDefined())
                m_isDefined = true;
            else if(stmts && func->isDefined())
                SemanticError(m_name->GetName() + " already has a definition");

            AddChild(stmts);
        }

        // Return back whether this declaration includes statements or not
        bool isDefined() { return m_isDefined; }

        // Return back this function's parameters
        cParamsNode * GetParams() { return m_params; }

        // Return back this function's declarations
        cDeclsNode * GetDecls() { return (cDeclsNode *)GetChild(3); }

        // Return back this function's statements
        cStmtsNode * GetStmts() { return (cStmtsNode *)GetChild(4); }

        // Return back the name of this function
        virtual string GetTypeName() { return m_name->GetName(); }

        // This is indeed a function
        virtual bool isFunc() { return true; }

        // Return back this func decl as the type
        virtual cDeclNode * GetType() { return m_returntype->GetDecl(); }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    private:
        cSymbol *m_name;
        cSymbol *m_returntype;
        cParamsNode *m_params;
        cFuncDeclNode *m_other; // The last function with the same name
        bool m_isDefined;
};
