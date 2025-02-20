#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

//!ukloni posle

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized; 

    static unsigned int getSizeOfType(unsigned int type) {
        switch(type) {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        return 0;
    }
};

class VertexBufferLayout{
public:
    VertexBufferLayout() : m_Stride(0) {}
    ~VertexBufferLayout() = default;
    void Push(unsigned int type, unsigned int count);
    inline unsigned int getStride() const { return m_Stride; }
    inline const std::vector<VertexBufferElement>& getElements() const { return m_Elements; }

private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
};

#endif
