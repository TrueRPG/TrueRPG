#ifndef RPG_SRC_CLIENT_GRAPHICS_ISHADER_H
#define RPG_SRC_CLIENT_GRAPHICS_ISHADER_H

#include <string>
#include <glm/glm.hpp>

class IShader
{
public:

    virtual ~IShader() = default;

    virtual void setUniform(const std::string &val_name, glm::mat4 mat) = 0;
    virtual void setUniform(const std::string &val_name, glm::vec2 vec) = 0;
    virtual void setUniform(const std::string &val_name, glm::vec3 vec) = 0;
    virtual void setUniform(const std::string &val_name, glm::vec4 vec) = 0;
    virtual void setUniform(const std::string &val_name, float val) = 0;
    virtual void setUniform(const std::string &val_name, int val) = 0;
    virtual void setUniform(const std::string &val_name, size_t size, int *arr) = 0;
    virtual void use() const = 0;
    virtual void updateUbo() = 0;
    virtual void destroy() = 0;
};

#endif // RPG_SRC_CLIENT_GRAPHICS_ISHADER_H
