#include "KeyMappingConfig.h"
#include "yaml-cpp/yaml.h"

KeyMappingConfig::KeyMappingConfig(std::string configPath)
{
    YAML::Node keyMappingNode = YAML::LoadFile(configPath);

    m_exit = m_stringKeyMapper.map(keyMappingNode["exit"].as<std::string>());

    m_moveUp = m_stringKeyMapper.map(keyMappingNode["moveUp"].as<std::string>());
    m_moveDown = m_stringKeyMapper.map(keyMappingNode["moveDown"].as<std::string>());
    m_moveLeft = m_stringKeyMapper.map(keyMappingNode["moveLeft"].as<std::string>());
    m_moveRight = m_stringKeyMapper.map(keyMappingNode["moveRight"].as<std::string>());

    m_openInventory = m_stringKeyMapper.map(keyMappingNode["openInventory"].as<std::string>());
    m_use = m_stringKeyMapper.map(keyMappingNode["use"].as<std::string>());
    m_hitYourself = m_stringKeyMapper.map(keyMappingNode["hitYourself"].as<std::string>());
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

Key KeyMappingConfig::getOpenInventoryKey()
{
    return m_openInventory;
}

Key KeyMappingConfig::getUseKey()
{
    return m_use;
}

Key KeyMappingConfig::getHitYourselfKey()
{
    return m_hitYourself;
}
