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
        cDeclNode() : cAstNode() {}

        // Declaration information getters
    	virtual bool isVar() { return false; }
    	virtual bool isFloat() { return false; }
    	virtual int GetSize() { return 0; }
    	virtual bool isFunc() { return false; }
    	virtual bool isStruct() { return false; }
};
