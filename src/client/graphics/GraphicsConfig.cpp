#include "../../pch.h"
#include "GraphicsConfig.h"

namespace YAML
{

template <> struct convert<GraphicsAPI>
{
    static bool decode(const Node &node, GraphicsAPI &rhs)
    {
        if (!node.IsDefined() || node.IsNull())
        {
            return false;
        }

        rhs = static_cast<GraphicsAPI>(node.as<int>());
        return true;
    }
};

}

namespace
{
template <typename T>
T getIfDefined(const YAML::Node &node, const std::string& name)
{
    if (node[name].IsDefined())
    {
        return node[name].as<T>();
    }

    return T();
}
}

GraphicsConfig::GraphicsConfig(const std::string &path)
{
    YAML::Node graphicsNode = YAML::LoadFile(path);

    m_api = getIfDefined<GraphicsAPI>(graphicsNode, "api");
    m_width = getIfDefined<int>(graphicsNode, "width");
    m_height = getIfDefined<int>(graphicsNode, "height");
}

GraphicsAPI GraphicsConfig::getApi() const
{
    return m_api;
}

int GraphicsConfig::getWidth() const
{
    return m_width;
}

int GraphicsConfig::getHeight() const
{
    return m_height;
}
