#include <map>
#include <list>
#include <string>
#include "cSymbolTable.h"
#include "cSymbol.h"
using std::map;
using std::list;
using std::string;

/**
 * @brief Default Constructor
 */
cSymbolTable::cSymbolTable() : m_scopes()
{ }

/**
 * @brief Copy Constructor
 * 
 * @param copy The other symbol table to copy
 */
cSymbolTable::cSymbolTable(const cSymbolTable & copy) : m_scopes()
{
    *this = copy;
}

/**
 * @brief Assignment Op overload
 * 
 * @param rhs The right hand side of the assignment
 * @return A reference to the current object (to chain assignments)
 */
cSymbolTable & cSymbolTable::operator=(const cSymbolTable & rhs)
{
    if(this != &rhs)
        m_scopes = rhs.m_scopes;

    return *this;
}

/**
 * @brief Increase the scope of the symbol table
 */
void cSymbolTable::IncreaseScope()
{
    m_scopes.push_front(map<string, cSymbol *>());
}

/**
 * @brief Decrease the scope of the symbol table
 */
void cSymbolTable::DecreaseScope()
{
    if(!m_scopes.empty())
        m_scopes.pop_front();
}

/**
 * @brief Insert a symbol into the current scope if it doesn't already exist
 * 
 * @param name The name of the symbol to insert into the current scope
 * @return A pointer to the symbol that was created or the symbol that already exists
 */
cSymbol * cSymbolTable::Insert(const string & name)
{
    cSymbol * symbol = LookupInner(name);

    if(symbol == nullptr)
    {
        symbol = new cSymbol(name);
        m_scopes.front()[name] = symbol;
    }

    return symbol;
}

/**
 * @brief Lookup a symbol only within the current symbol table
 * 
 * @param name  The symbol to lookup
 * @return A pointer to the symbol if found, otherwise nullptr
 */
cSymbol * cSymbolTable::LookupInner(const string & name) const
{
    cSymbol * retval = nullptr;

    // Make sure there's a scope to search
    if(!m_scopes.empty())
    {
        auto map_iter = m_scopes.front().find(name);

        if(map_iter != m_scopes.front().end())
            retval = map_iter->second;
    }

    return retval;
}

/**
 * @brief Lookup a symbol within every symbol table (every scope).
 * 
 * @param name  The symbol to lookup
 * @return A pointer to the symbol if found, otherwise nullptr
 */
cSymbol * cSymbolTable::Lookup(const string & name) const
{
    // Make sure there's a scope to search
    if(!m_scopes.empty())
    {
        for(auto list_iter = m_scopes.begin(); list_iter != m_scopes.end(); ++list_iter)
        {
            auto map_iter = list_iter->find(name);

            if(map_iter != list_iter->end())
                return map_iter->second;
        }
    }

    return nullptr;
}