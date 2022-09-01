#ifndef RPG_SRC_CLIENT_GRAPHICS_GRAPHICSCONFIG_H
#define RPG_SRC_CLIENT_GRAPHICS_GRAPHICSCONFIG_H

enum class GraphicsAPI
{
    Undefined = -1,
    OpenGL,
    Vulkan
};

class GraphicsConfig
{
private:
    GraphicsAPI m_api;
    int m_width;
    int m_height;
public:
    explicit GraphicsConfig(const std::string &path);

    GraphicsAPI getApi() const;
    int getWidth() const;
    int getHeight() const;
};

#endif // RPG_SRC_CLIENT_GRAPHICS_GRAPHICSCONFIG_H
