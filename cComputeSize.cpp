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
    int temp = m_offset;

    VisitAllChildren(node);

    node->SetSize(m_offset - temp);
}

void cComputeSize::Visit(cVarDeclNode *node)
{
    node->SetOffset(Align(m_offset));
    m_offset = Align(m_offset) + node->GetSize();
    if (m_offset > m_highwater) m_highwater = m_offset;
}
