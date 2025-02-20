#include "VertexBufferLayout.h"

void VertexBufferLayout::Push(unsigned type, unsigned int count) {
    m_Elements.push_back({type, count, GL_FALSE});
    this->m_Stride += count * VertexBufferElement::getSizeOfType(type);
}