#pragma once
//**************************************
// cDeclNode
//
// Defines base class for all declarations.
// Future labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2015
//
#include "cAstNode.h"

class cDeclNode : public cAstNode
{
    public:
        cDeclNode() : cAstNode(), m_size(0), m_offset(0) {}

        // Declaration information getters
    	virtual bool isVar() { return false; }
    	virtual bool isFloat() { return false; }
    	virtual int GetSize() {return m_size; }
        virtual void SetSize(int size) { m_size = size; }
        virtual int GetOffset() { return m_offset; }
        virtual void SetOffset(int offset) { m_offset = offset; }
    	virtual bool isFunc() { return false; }
    	virtual bool isStruct() { return false; }
        virtual bool isNum() { return (!isFunc() && !isStruct() && !isVar()); }

        // Get the symbolic name of the declaration (the type name)
        virtual string GetTypeName() = 0;

        // Get the type of this declaration
    	virtual cDeclNode * GetType() = 0;

        virtual string AttributesToString() 
        {
            if(!m_size && !m_offset)
                return "";
            else
                return " size=\"" + std::to_string(m_size) + "\" offset=\"" + std::to_string(m_offset) + "\"";
        }

    protected:
        int m_size;
        int m_offset;
};
