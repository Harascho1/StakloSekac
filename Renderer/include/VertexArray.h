#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl);

    void Bind() const;
    void Unbind() const;

private:
    unsigned int m_RendererID;
};

#endif