#include "../../pch.h"
#include "GlfwKeyMapper.h"

int GlfwKeyMapper::map(Key key)
{
    auto item = m_keyMap.find(key);
    if (m_keyMap.find(key) != m_keyMap.end()) {
        return item->second;
    }
    return GLFW_KEY_UNKNOWN;
}
