#include "Shader.h"

Shader::Shader(unsigned int id) : ID(id) { }

void Shader::use() const
{
    glUseProgram(ID);
}

unsigned int Shader::getId() const noexcept
{
    return ID;
}

void Shader::destroy()
{
    glDeleteProgram(ID);
    ID = 0;
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
                                    void (*GLget)(unsigned int, unsigned int, int*),
                                    void (*GLinfoLog)(unsigned int, int, int*, char*))
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
