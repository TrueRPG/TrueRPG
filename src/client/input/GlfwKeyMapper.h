#ifndef RPG_GLFWKEYMAPPER_H
#define RPG_GLFWKEYMAPPER_H

#include "../graphics/Graphics.h"
#include <unordered_map>
#include "IMapper.h"
#include "Key.h"

class GlfwKeyMapper : public IMapper<Key, int>
{
    std::unordered_map<Key, int> m_keyMap = {
        {Key::Unknown, GLFW_KEY_UNKNOWN},
        {Key::Space, GLFW_KEY_SPACE},

        {Key::GraveAccent, GLFW_KEY_GRAVE_ACCENT},
        {Key::Num0, GLFW_KEY_0},
        {Key::Num1, GLFW_KEY_1},
        {Key::Num2, GLFW_KEY_2},
        {Key::Num3, GLFW_KEY_3},
        {Key::Num4, GLFW_KEY_4},
        {Key::Num5, GLFW_KEY_5},
        {Key::Num6, GLFW_KEY_6},
        {Key::Num7, GLFW_KEY_7},
        {Key::Num8, GLFW_KEY_8},
        {Key::Num9, GLFW_KEY_9},
        {Key::Minus, GLFW_KEY_MINUS},
        {Key::Equal, GLFW_KEY_EQUAL},

        {Key::A, GLFW_KEY_A},
        {Key::B, GLFW_KEY_B},
        {Key::C, GLFW_KEY_C},
        {Key::D, GLFW_KEY_D},
        {Key::E, GLFW_KEY_E},
        {Key::F, GLFW_KEY_F},
        {Key::G, GLFW_KEY_G},
        {Key::H, GLFW_KEY_H},
        {Key::I, GLFW_KEY_I},
        {Key::J, GLFW_KEY_J},
        {Key::K, GLFW_KEY_K},
        {Key::L, GLFW_KEY_L},
        {Key::M, GLFW_KEY_M},
        {Key::N, GLFW_KEY_N},
        {Key::O, GLFW_KEY_O},
        {Key::P, GLFW_KEY_P},
        {Key::Q, GLFW_KEY_Q},
        {Key::R, GLFW_KEY_R},
        {Key::S, GLFW_KEY_S},
        {Key::T, GLFW_KEY_T},
        {Key::U, GLFW_KEY_U},
        {Key::V, GLFW_KEY_V},
        {Key::W, GLFW_KEY_W},
        {Key::X, GLFW_KEY_X},
        {Key::Y, GLFW_KEY_Y},
        {Key::Z, GLFW_KEY_Z},

        {Key::LBracket, GLFW_KEY_LEFT_BRACKET},
        {Key::RBracket, GLFW_KEY_RIGHT_BRACKET},
        {Key::Backslash, GLFW_KEY_BACKSLASH},
        {Key::Semicolon, GLFW_KEY_SEMICOLON},
        {Key::Apostrophe, GLFW_KEY_APOSTROPHE},
        {Key::Comma, GLFW_KEY_COMMA},
        {Key::Period, GLFW_KEY_PERIOD},
        {Key::Slash, GLFW_KEY_SLASH},

        {Key::Esc, GLFW_KEY_ESCAPE},
        {Key::Enter, GLFW_KEY_ENTER},
        {Key::Tab, GLFW_KEY_TAB},
        {Key::Backspace, GLFW_KEY_BACKSPACE},
        {Key::Insert, GLFW_KEY_INSERT},
        {Key::Delete, GLFW_KEY_DELETE},
        {Key::Right, GLFW_KEY_RIGHT},
        {Key::Left, GLFW_KEY_LEFT},
        {Key::Down, GLFW_KEY_DOWN},
        {Key::Up, GLFW_KEY_UP},
        {Key::PageUp, GLFW_KEY_PAGE_UP},
        {Key::PageDown, GLFW_KEY_PAGE_DOWN},
        {Key::Home, GLFW_KEY_HOME},
        {Key::End, GLFW_KEY_END},
        {Key::CapsLock, GLFW_KEY_CAPS_LOCK},
        {Key::ScrollLock, GLFW_KEY_SCROLL_LOCK},
        {Key::NumLock, GLFW_KEY_NUM_LOCK},
        {Key::PrintScreen, GLFW_KEY_PRINT_SCREEN},
        {Key::Pause, GLFW_KEY_PAUSE},

        {Key::F1, GLFW_KEY_F1},
        {Key::F2, GLFW_KEY_F2},
        {Key::F3, GLFW_KEY_F3},
        {Key::F4, GLFW_KEY_F4},
        {Key::F5, GLFW_KEY_F5},
        {Key::F6, GLFW_KEY_F6},
        {Key::F7, GLFW_KEY_F7},
        {Key::F8, GLFW_KEY_F8},
        {Key::F9, GLFW_KEY_F9},
        {Key::F10, GLFW_KEY_F10},
        {Key::F11, GLFW_KEY_F11},
        {Key::F12, GLFW_KEY_F12},

        {Key::LShift, GLFW_KEY_LEFT_SHIFT},
        {Key::LCtrl, GLFW_KEY_LEFT_CONTROL},
        {Key::LAlt, GLFW_KEY_LEFT_ALT},
        {Key::LSuper, GLFW_KEY_LEFT_SUPER},

        {Key::RShift, GLFW_KEY_RIGHT_SHIFT},
        {Key::RCtrl, GLFW_KEY_RIGHT_CONTROL},
        {Key::RAlt, GLFW_KEY_RIGHT_ALT},
        {Key::RSuper, GLFW_KEY_RIGHT_SUPER},

        {Key::Menu, GLFW_KEY_MENU}
    };

public:
    int map(Key key) override;
};

#endif // RPG_GLFWKEYMAPPER_H
