#include "lex.h"
#include "cComputeSize.h"
#include "astnodes.h"

void cComputeSize::VisitAllNodes(cAstNode *node)
{
    VisitAllChildren(node);
}

void cComputeSize::Visit(cBlockNode *node)
{
    int temp = m_offset;

    VisitAllChildren(node);

    m_highwater = temp;
    m_offset = 0;
}

void cComputeSize::Visit(cVarDeclNode *node)
{
    node->SetOffset(m_offset + m_highwater);

    // Align to 4 byte boundaries
    // Zero out the bottom two bits and then add 4 if its not aligned
    if(node->GetSize() % 4 != 0)
        m_offset += (node->GetSize() & ~3) + 4;
    else
        m_offset += node->GetSize();
}