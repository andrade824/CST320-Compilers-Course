#pragma once
//**************************************
// cBlockNode.h
//
// Defines AST node for a block of code (stuff inside {})
//
// Since blocks can take the place of statements, this class inherits from 
// cStmtNode
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2015
//

#include "cDeclsNode.h"
#include "cStmtNode.h"
#include "cStmtsNode.h"

class cBlockNode : public cStmtNode
{
    public:
        // params are the decls and statements contained in the block
        cBlockNode(cDeclsNode *decls, cStmtsNode *statements)
            : cStmtNode(), m_size(decls->GetSize())
        {
            AddChild(decls);
            AddChild(statements);
        }

        // Getters and setters
        int GetSize() { return m_size; }

        virtual string NodeType() { return string("block"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString() 
        {
            if(m_size)
                return " size=\"" + std::to_string(m_size) + "\"";
            else
                return "";
        }

    private:
        int m_size;
};
