#ifndef RENDERER_H
#define RENDERER_H

//! obrisi posle ovo

#include <glad/glad.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "../Shaders/include/Shader.h"

class Renderer {
public:
    void clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawLines(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void resize(GLFWwindow *window) const;
private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif