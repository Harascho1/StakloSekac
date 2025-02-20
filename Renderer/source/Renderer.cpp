#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();
    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawLines(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();
    glDrawElements(GL_LINES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::clear() const {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Renderer::resize(GLFWwindow * window) const {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);
}