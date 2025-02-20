#include "Shader.h"

SourceShader Shader::getSourceShader() {

    std::fstream file(this->filePath);
    if (!file.is_open()) {
        std::cout << "Ne moze da nadje filePath za SourceShader-ee: " << filePath << '\n';
        return { "" , ""};
    }

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream sourceKodovi[2];
    ShaderType type = ShaderType::NONE;
    
    while (getline(file, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            sourceKodovi[int(type)] << line << '\n';  
        }
    }
    return { sourceKodovi[0].str(), sourceKodovi[1].str() };
}

unsigned int Shader::compileShaderID(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* source_code = source.c_str();
    glShaderSource(shader, 1, &source_code, 0);
    glCompileShader(shader);

    int isSuccess;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "JEBE VERTEX SHADER " << infoLog << '\n';
        return -1;
    }

    return shader;
}

unsigned int Shader::createShader(const std::string& vertexSourceCode, const std::string& fragmentSourceCode) {
    unsigned int vertexShader = compileShaderID(GL_VERTEX_SHADER, vertexSourceCode);
    unsigned int fragmentShader = compileShaderID(GL_FRAGMENT_SHADER, fragmentSourceCode);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);
    //checking the linking
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "JEBE ME LINKING ZA SHADERE " << infoLog << '\n';
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
//* tjt

Shader::Shader(const std::string& _filePath)
    : filePath(_filePath) {
    SourceShader src = getSourceShader();    
    this->ID = createShader(src.SourceVertex, src.SourceFragment);

}

Shader::~Shader() {
    glDeleteProgram(ID);
    //po principu on bi zasigurno izgubio ID kad bi instanca izasla iz scope-a ali ne znam da li bi nesto u GLFW state masini.
}

void Shader::Bind() const {
    glUseProgram(ID);
}

void Shader::setUniform1i(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniformMatrix4fv(const std::string& name, glm::mat4 matrix) {
    glUniformMatrix4fv(this->getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::Unbind() const {
    glUseProgram(0);
}

int Shader::getUniformLocation(const std::string& name) {

    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];


    unsigned int location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) std::cout << "PAZI: uniform '" << name << "' ne postoji! \n";

    m_UniformLocationCache[name] = location;

    return location;
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(this->getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) {
    glUniform3f(this->getUniformLocation(name), v0, v1, v2);
}