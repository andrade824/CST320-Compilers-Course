#include "lex.h"
#include "cComputeSize.h"
#include "astnodes.h"

// Align to 4 byte boundaries
// Zero out the bottom two bits and then add 4 if its not aligned
int cComputeSize::Align(int size)
{
    int temp = 0;

    if(size % 4 != 0)
        temp = (size & ~3) + 4;
    else
        temp = size;

    return temp;
}

/** Visit Functions for various node types **/
void cComputeSize::VisitAllNodes(cAstNode *node)
{
    VisitAllChildren(node);
}

void cComputeSize::Visit(cBlockNode *node)
{
    int old_offset = m_offset;
    int old_highwater = m_highwater;
    m_highwater = 0;

    VisitAllChildren(node);

    node->SetSize(m_highwater - old_offset);

    m_offset = old_offset;
    if(old_highwater > m_highwater)
        m_highwater = old_highwater;
}

void cComputeSize::Visit(cDeclsNode *node)
{
    int old_offset = m_offset;

    VisitAllChildren(node);

    node->SetSize(m_offset - old_offset);
}

void cComputeSize::Visit(cStructDeclNode *node)
{
    int old_offset = m_offset;
    m_offset = 0;

    VisitAllChildren(node);

    node->SetSize(m_offset);
    m_offset = old_offset;
}

void cComputeSize::Visit(cFuncDeclNode *node)
{
    int old_offset = m_offset;
    int old_highwater = m_highwater;
    m_offset = 0;
    m_highwater = 0;

    VisitAllChildren(node);

    node->SetSize(Align(m_highwater));
    m_offset = old_offset;
    m_highwater = old_highwater;
}

void cComputeSize::Visit(cParamsNode *node)
{
    int old_offset = m_offset;

    cAstNode::iterator it;
    for (it=node->FirstChild(); it!=node->LastChild(); it++)
    {
        if ((*it) != nullptr) (*it)->Visit(this);
        m_offset = Align(m_offset);
    }
    
    node->SetSize(m_offset - old_offset);
}

void cComputeSize::Visit(cVarDeclNode *node)
{
    node->SetSize(node->GetType()->GetSize());

    // Don't align chars
    if(node->GetType()->isNum() && node->GetType()->GetSize() == 1)
    {
        node->SetOffset(m_offset);
        m_offset = m_offset + node->GetSize();
    }
    else
    {
        node->SetOffset(Align(m_offset));
        m_offset = Align(m_offset) + node->GetSize();
    }

    if (m_offset > m_highwater) m_highwater = m_offset;
}

void cComputeSize::Visit(cVarExprNode *node)
{
    int old_offset = m_offset;
    m_offset = 0;

    VisitAllChildren(node);

    node->SetSize(node->GetDecl()->GetSize());
    node->SetOffset(m_offset);
    
    m_offset = old_offset;
}

void cComputeSize::Visit(cSymbol *node)
{
    m_offset += node->GetDecl()->GetOffset();
}
