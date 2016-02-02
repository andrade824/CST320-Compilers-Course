#pragma once

#include <list>
#include <map>
#include <string>
#include "cSymbol.h"
using std::list;
using std::map;
using std::string;

class cSymbolTable
{
public:
    // Default Constructor
    cSymbolTable();

    // Copy Constructor
    cSymbolTable(const cSymbolTable & copy);

    // Assignment Op overload
    cSymbolTable & operator=(const cSymbolTable & rhs);

    // Increase the scope of the symbol table
    void IncreaseScope();

    // Decreaset he scope of the symbol table
    void DecreaseScope();

    // Insert a symbol into the current scope if it doesn't already exist
    cSymbol * Insert(const string & name);

    // Initialize the global table
    void InitGlobalTable();

    // Lookup a symbol only within the current symbol table
    cSymbol * LookupInner(const string & name) const;

    // Lookup a symbol within every symbol table (every scope).
    cSymbol * Lookup(const string & name) const;
private:
    list<map<string, cSymbol *>> m_scopes;
};