#include "../../pch.h"
#include "StringKeyMapper.h"

#include <algorithm>

Key StringKeyMapper::map(std::string key)
{
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    auto item = m_keyMap.find(key);
    if (m_keyMap.find(key) != m_keyMap.end()) {
        return item->second;
    }
    return Key::Unknown;
}
