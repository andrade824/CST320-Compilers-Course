#pragma once
//**************************************
// cFuncExprNode.h
//
// Function reference node
//
// Author: Devon Andrade <devon.andrade@oit.edu>
//
// Date: Feb. 2, 2016
//

#include "cExprNode.h"
#include "cParamListNode.h"
#include "cSymbol.h"
#include "cDeclNode.h"
#include "semantic.h"

class cFuncExprNode : public cExprNode
{
    public:
        // Function reference node
        cFuncExprNode(cSymbol* name, cParamListNode* params) 
            : cExprNode(), m_funcdecl(name->GetDecl())
        {
            // Make sure it's a function we're calling
            if(!name->GetDecl()->isFunc())
                SemanticError(name->GetName() + " is not a function");
            else
            {
                cFuncDeclNode *decl = dynamic_cast<cFuncDeclNode*>(name->GetDecl());

                if((decl->GetParams() != nullptr && params == nullptr) ||
                   (decl->GetParams() == nullptr && params != nullptr))
                {
                    SemanticError(name->GetName() + " called with wrong number of arguments");
                }
                else if(params != nullptr && (decl->GetParams()->NumChildren() != params->NumChildren()))
                {
                    SemanticError(name->GetName() + " called with wrong number of arguments");
                }
                else if(decl->GetParams() != nullptr && params != nullptr)
                {
                    bool different = false;

                    for(int i = 0; i < params->NumChildren() && !different; ++i)
                    {
                        cExprNode *child = dynamic_cast<cExprNode*>(params->GetChild(i));
                        cDeclNode *otherChild = dynamic_cast<cDeclNode*>(decl->GetParams()->GetChild(i));

                        if(!child->isCompatibleWith(otherChild))
                            different = true;
                    }

                    if(different)
                        SemanticError(name->GetName() + " called with incompatible argument");
                }
            }

            AddChild(name);
            AddChild(params);
        }

        // Return back the return type of the function
        virtual cDeclNode * GetType() { return m_funcdecl->GetType(); }

        virtual string NodeType() { return string("funcCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    private:
        cDeclNode *m_funcdecl;
};
