#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

class IndexBuffer {
private:
    unsigned int m_RendereId;
    unsigned int m_Count;
public:
    IndexBuffer(const void* data, unsigned int count);
    IndexBuffer(unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int getCount() const { return m_Count; }
};

#endif