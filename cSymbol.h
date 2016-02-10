#pragma once
//**************************************
// cSymbol.h
//
// Defines class used to represent symbols.
// Later labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2015
//

#include <string>

using std::string;

#include "cAstNode.h"
#include "cDeclNode.h"

class cSymbol : public cAstNode
{
    public:
        // param is name of symbol
        cSymbol(string name) : cAstNode()
        {
            m_id = ++nextId;        // get next available ID
            m_name = name;
            m_decl = nullptr;
        }

        // return name of symbol
        string GetName() { return m_name; }

        // Change the symbols name
        void SetName(const string& name) { m_name = name; }

        // Getter and setter for declaration
        cDeclNode * GetDecl() { return m_decl; }
        void SetDecl(cDeclNode *decl) { m_decl = decl; }

        virtual string AttributesToString()
        {
            string result(" id=\"");
            result += std::to_string(m_id);
            result += "\" name=\"" + m_name + "\"";
            return result;
        }

        virtual string NodeType() { return string("sym"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual ~cSymbol() { }
    protected:
        static long long nextId;        // Next avail symbol ID
        long long m_id;                 // Unique ID for this symbol
        string m_name;                  // name of symbol
        cDeclNode *m_decl;              // Declaration of the symbol
};
