#ifndef RPG_ISPRITEBATCH_H
#define RPG_ISPRITEBATCH_H

#include <glm/glm.hpp>

class Sprite;
class IShader;

class ISpriteBatch
{
public:
    virtual ~ISpriteBatch() = default;

    virtual void begin() = 0;

    virtual void end() = 0;

    virtual void draw(const Sprite &sprite, int layer = 0, int order = 0) = 0;

    virtual void setShader(IShader *shader) = 0;

    virtual glm::mat4 getProjectionMatrix() = 0;

    virtual void setProjectionMatrix(glm::mat4 projMat) = 0;

    virtual glm::mat4 getViewMatrix() = 0;

    virtual void setViewMatrix(glm::mat4 viewMat) = 0;

    virtual void destroy() = 0;
};

#endif // RPG_ISPRITEBATCH_H
