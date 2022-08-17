#include "../../pch.h"
#include "KeyMappingConfig.h"

KeyMappingConfig::KeyMappingConfig(std::string configPath)
{
    YAML::Node keyMappingNode = YAML::LoadFile(configPath);

    m_exit = getIfDefined(keyMappingNode, "exit");

    m_moveUp = getIfDefined(keyMappingNode, "moveUp");
    m_moveDown = getIfDefined(keyMappingNode, "moveDown");
    m_moveLeft = getIfDefined(keyMappingNode, "moveLeft");
    m_moveRight = getIfDefined(keyMappingNode, "moveRight");

    m_inventory = getIfDefined(keyMappingNode, "inventory");
    m_use = getIfDefined(keyMappingNode, "use");
    m_hitYourself = getIfDefined(keyMappingNode, "hitYourself");
    m_torch = getIfDefined(keyMappingNode, "torch");
}

Key KeyMappingConfig::getExitKey()
{
    return m_exit;
}

Key KeyMappingConfig::getMoveUpKey()
{
    return m_moveUp;
}

Key KeyMappingConfig::getMoveDownKey()
{
    return m_moveDown;
}

Key KeyMappingConfig::getMoveLeftKey()
{
    return m_moveLeft;
}

Key KeyMappingConfig::getMoveRightKey()
{
    return m_moveRight;
}

Key KeyMappingConfig::getInventoryKey()
{
    return m_inventory;
}

Key KeyMappingConfig::getUseKey()
{
    return m_use;
}

Key KeyMappingConfig::getHitYourselfKey()
{
    return m_hitYourself;
}

Key KeyMappingConfig::getTorchKey()
{
    return m_torch;
}

Key KeyMappingConfig::getIfDefined(YAML::Node node, const std::string& name)
{
    if (node[name].IsDefined())
    {
        return m_stringKeyMapper.map(node[name].as<std::string>());
    }
    else
    {
        return Key::Unknown;
    }
}
