#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    glGenBuffers(1, &m_RendereId);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendereId);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(unsigned int size) {
    glGenBuffers(1, &m_RendereId);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendereId);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendereId);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendereId);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}