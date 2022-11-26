#ifndef RPG_ITEXTURE_H
#define RPG_ITEXTURE_H

#include <string>

class ITexture
{
public:
    virtual ~ITexture() = default;

    virtual void bind(unsigned int slot = 0) const = 0;

    virtual void unbind() const = 0;

    virtual unsigned int getId() const noexcept = 0;

    virtual void destroy() = 0;

    virtual std::string getPath() const = 0;

    virtual int getWidth() const = 0;

    virtual int getHeight() const = 0;
};

#endif // RPG_ITEXTURE_H
