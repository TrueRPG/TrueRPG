#include "../../pch.h"
#include "Shader.h"
#include <fstream>
#include <sstream>

Shader::Shader(unsigned int m_id) : m_id(m_id) { }

void Shader::use() const
{
    glUseProgram(m_id);
}

unsigned int Shader::getId() const noexcept
{
    return m_id;
}

void Shader::destroy()
{
    glDeleteProgram(m_id);
    m_id = 0;
}

template<typename F1, typename F2>
void checkCompileErrors(unsigned int glHandel, unsigned int status,
                                    F1 GLget,
                                    F2 GLinfoLog)
{
    char infoLog[1024];
    int success;

    GLget(glHandel, status, &success);

    if(!success)
    {
        GLinfoLog(glHandel, 1024, nullptr, infoLog);
        std::cout << "ERROR::SHADER: \n\t" << infoLog << std::endl;
    }
}

Shader Shader::createShader(const std::string& vertexPath, const std::string& fragmentPath)
{
    unsigned int shaderProgram = 0;

    unsigned int vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, GL_LINK_STATUS, glGetProgramiv, glGetProgramInfoLog);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return Shader(shaderProgram);
}

unsigned int Shader::compileShader(const std::string& path, unsigned int type)
{
    std::ifstream shaderFile;
    std::string shaderCode;
    unsigned int shader = 0;
    const char* sCode = nullptr;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // Open the file
        shaderFile.open(path);
        std::stringstream shaderStream;

        // Read the contents of file buffers
        shaderStream << shaderFile.rdbuf();

        // Close the file
        shaderFile.close();

        // Convert data from the stream into a string
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        exit(1);
    }

    sCode = shaderCode.c_str();
    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char* const*)&sCode, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, GL_COMPILE_STATUS, glGetShaderiv, glGetShaderInfoLog);

    return shader;
}

