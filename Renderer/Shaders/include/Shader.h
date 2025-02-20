#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

struct SourceShader {
    std::string SourceVertex;
    std::string SourceFragment;
};

class Shader {
public:
    //caching system?
    Shader(const std::string& _filePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // set uniforms
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniform1i(const std::string& name, int value);
    void setUniformMatrix4fv(const std::string& name, glm::mat4 matrix);
    void setUniform3f(const std::string& name, float v0, float v1, float v2);
private:
    unsigned int compileShaderID(unsigned int type, const std::string& source);
    unsigned int createShader(const std::string& vertexSourceCode, const std::string& fragmentSourceCode);
    SourceShader getSourceShader();
    int getUniformLocation(const std::string& name);
private:
    std::string filePath;
    unsigned int ID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
};

#endif