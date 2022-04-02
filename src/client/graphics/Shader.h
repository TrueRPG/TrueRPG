#ifndef RPG_SHADER_H
#define RPG_SHADER_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "IGLObject.h"

#include <string>
#include <iostream>

class Shader : public IGLObject
{
private:
    unsigned int m_id{};
public:
    Shader() = default;
    explicit Shader(unsigned int id);

    // Activate the shader
    void use() const;

    unsigned int getId() const noexcept override;

    template <typename T>
    void setArray(const std::string &name, GLsizei count, T *data)
    {
        if constexpr (std::is_floating_point_v<T>)
            glUniform1fv(glGetUniformLocation(m_id, name.c_str()), count, data);
        else if constexpr (std::is_integral_v<T>)
            glUniform1iv(glGetUniformLocation(m_id, name.c_str()), count, data);
        else if constexpr (std::is_unsigned_v<T>)
            glUniform1uiv(glGetUniformLocation(m_id, name.c_str()), count, data);
    }

    template <typename T>
    void setValue(const std::string &name, T data)
    {
        if constexpr (std::is_floating_point_v<T>)
            glUniform1f(glGetUniformLocation(m_id, name.c_str()), data);
        else if constexpr (std::is_integral_v<T>)
            glUniform1i(glGetUniformLocation(m_id, name.c_str()), data);
        else if constexpr (std::is_unsigned_v<T>)
            glUniform1ui(glGetUniformLocation(m_id, name.c_str()), data);
    }
    void setVector2(const std::string &name, const glm::vec2 &data);
    void setVector3(const std::string &name, const glm::vec3 &data);
    void setMatrix4(const std::string &name, const glm::mat4 &data);

    void destroy() override;

    static Shader createShader(const std::string& vertexPath, const std::string& fragmentPath);

private:
    // Some useful functions to check shader compilation/binding errors
    static unsigned int compileShader(const std::string& path, unsigned int type);

    static void checkCompileErrors(unsigned int glHandelr, unsigned int status, 
                                   void (*GLget)(unsigned int, unsigned int, int*),
                                   void (*GLinfoLog)(unsigned int, int, int*, char*));
};

#endif //RPG_SHADER_H
