#ifndef RPG_KEYMAPPINGCONFIG_H
#define RPG_KEYMAPPINGCONFIG_H

#include <string>
#include "Key.h"
#include "StringKeyMapper.h"
#include <yaml-cpp/yaml.h>

class KeyMappingConfig
{
private:

    Key m_exit;

    Key m_moveUp;
    Key m_moveDown;
    Key m_moveLeft;
    Key m_moveRight;

    Key m_openInventory;
    Key m_use;
    Key m_hitYourself;

    StringKeyMapper m_stringKeyMapper;

public:
    KeyMappingConfig(std::string configPath);

    Key getExitKey();

    Key getMoveUpKey();
    Key getMoveDownKey();
    Key getMoveLeftKey();
    Key getMoveRightKey();

    Key getOpenInventoryKey();
    Key getUseKey();
    Key getHitYourselfKey();

private:
    Key getIfDefined(YAML::Node node, const std::string& name);
};

#endif // RPG_KEYMAPPINGCONFIG_H
