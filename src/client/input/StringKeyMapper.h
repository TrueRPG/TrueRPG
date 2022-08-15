#ifndef RPG_STRINGKEYMAPPER_H
#define RPG_STRINGKEYMAPPER_H

#include <string>
#include <unordered_map>
#include "IMapper.h"
#include "Key.h"

class StringKeyMapper : public IMapper<std::string, Key>
{
    std::unordered_map<std::string, Key> m_keyMap = {
        {"unknown", Key::Unknown},
        {"space", Key::Space},

        {"`", Key::GraveAccent},
        {"0", Key::Num0},
        {"1", Key::Num1},
        {"2", Key::Num2},
        {"3", Key::Num3},
        {"4", Key::Num4},
        {"5", Key::Num5},
        {"6", Key::Num6},
        {"7", Key::Num7},
        {"8", Key::Num8},
        {"9", Key::Num9},
        {"-", Key::Minus},
        {"=", Key::Equal},

        {"a", Key::A},
        {"b", Key::B},
        {"c", Key::C},
        {"d", Key::D},
        {"e", Key::E},
        {"f", Key::F},
        {"g", Key::G},
        {"h", Key::H},
        {"i", Key::I},
        {"j", Key::J},
        {"k", Key::K},
        {"l", Key::L},
        {"m", Key::M},
        {"n", Key::N},
        {"o", Key::O},
        {"p", Key::P},
        {"q", Key::Q},
        {"r", Key::R},
        {"s", Key::S},
        {"t", Key::T},
        {"u", Key::U},
        {"v", Key::V},
        {"w", Key::W},
        {"x", Key::X},
        {"y", Key::Y},
        {"z", Key::Z},

        {"[", Key::LBracket},
        {"]", Key::RBracket},
        {"\\", Key::Backslash},
        {";", Key::Semicolon},
        {"'", Key::Apostrophe},
        {",", Key::Comma},
        {".", Key::Period},
        {"/", Key::Slash},

        {"esc", Key::Esc},
        {"enter", Key::Enter},
        {"tab", Key::Tab},
        {"backspace", Key::Backspace},
        {"insert", Key::Insert},
        {"delete", Key::Delete},
        {"right", Key::Right},
        {"left", Key::Left},
        {"down", Key::Down},
        {"up", Key::Up},
        {"pageup", Key::PageUp},
        {"pagedown", Key::PageDown},
        {"home", Key::Home},
        {"end", Key::End},
        {"capslock", Key::CapsLock},
        {"scrolllock", Key::ScrollLock},
        {"numlock", Key::NumLock},
        {"printscreen", Key::PrintScreen},
        {"pause", Key::Pause},

        {"f1", Key::F1},
        {"f2", Key::F2},
        {"f3", Key::F3},
        {"f4", Key::F4},
        {"f5", Key::F5},
        {"f6", Key::F6},
        {"f7", Key::F7},
        {"f8", Key::F8},
        {"f9", Key::F9},
        {"f10", Key::F10},
        {"f11", Key::F11},
        {"f12", Key::F12},

        {"lshift", Key::LShift},
        {"lctrl", Key::LCtrl},
        {"lalt", Key::LAlt},
        {"lsuper", Key::LSuper},

        {"rshift", Key::RShift},
        {"rctrl", Key::RCtrl},
        {"ralt", Key::RAlt},
        {"rsuper", Key::RSuper},

        {"menu", Key::Menu}
    };

public:
    Key map(std::string key) override;
};

#endif // RPG_STRINGKEYMAPPER_H
