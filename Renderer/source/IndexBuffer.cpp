#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
    : m_Count(count){
    glGenBuffers(1, &m_RendereId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendereId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}


IndexBuffer::IndexBuffer(unsigned int count)
    : m_Count(count) {
    glGenBuffers(1, &m_RendereId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendereId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_RendereId);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendereId);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}