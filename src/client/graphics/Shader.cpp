#include "Shader.h"

Shader::Shader(unsigned int id) : m_id(id) { }

void Shader::use() const
{
    glUseProgram(m_id);
}

void Shader::setUniform(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, const glm::vec3 &vec) const
{
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setUniform(const std::string &name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniform(const std::string &name, const int arr[], int size) const
{
    glUniform1iv(glGetUniformLocation(m_id, name.c_str()), size, arr);
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
    std::fstream shaderFile;
    std::string shaderCode;
    unsigned int shader = 0;
    const char* sCode = nullptr;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // Открываем файлы
        shaderFile.open(path);
        std::stringstream shaderStream;

        // Читаем содержимое файловых буферов
        shaderStream << shaderFile.rdbuf();

        // Закрываем файлы
        shaderFile.close();

        // Конвертируем в строковую переменную данные из потока
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

void Shader::checkCompileErrors(unsigned int glHandel, unsigned int status,
                                    void (*const GLget)(unsigned int, unsigned int, int*),
                                    void (*const GLinfoLog)(unsigned int, int, int*, char*))
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
