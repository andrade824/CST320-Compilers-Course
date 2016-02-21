#pragma once
//**************************************
// cExprNode.h
//
// Defines base class for all expressions
//
// This is a pure virtual class because there is no definition for
// cAstNode::ToString()
//
// Author: Devon Andrade <devon.andrade@oit.edu> 
//
// Date: Feb. 10,2016
//

#include "cStmtNode.h"
#include "cDeclNode.h"

class cExprNode : public cStmtNode
{
    public:
        // Default constructor
        cExprNode() : cStmtNode() {}

        // Returns true if the passed in expression can be upcasted to this expressions's type
        bool isCompatibleWith(cExprNode *other)
        {
            bool isCompat = true;

            // One side is a struct and the other isn't
            // The weird "!"s everywhere are a fancy way of doing an XOR logically
            if(!GetType()->isStruct() != !other->GetType()->isStruct())
                isCompat = false;

            // If they are both structs, check they're the same type of struct
            if((GetType()->isStruct() && other->GetType()->isStruct()) &&
               (GetType() != other->GetType()) )
                isCompat = false;

            // One side is numeric, but the other isn't
            if(!GetType()->isNum() != !other->GetType()->isNum())
                isCompat = false;

            // This is a char/int/float, make sure other expression is char/int/float and smaller/equal
            if((GetType()->isNum() && other->GetType()->isNum()) &&
               (GetType()->GetSize() < other->GetType()->GetSize()) )
                isCompat = false;

            // This isn't a float, but the other expression is
            if(!GetType()->isFloat() && other->GetType()->isFloat())
                isCompat = false;

            return isCompat;
        }

        // Return back the type of the expression
        virtual cDeclNode * GetType() = 0;
};