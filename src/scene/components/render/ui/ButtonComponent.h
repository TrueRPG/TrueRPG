#ifndef RPG_BUTTONCOMPONENT_H
#define RPG_BUTTONCOMPONENT_H

#include "../../../../client/graphics/Font.h"

struct ButtonComponent
{
    Font *font;
    std::string text;
    glm::vec2 size{100.f, 50.f};

    glm::vec4 color{0.6f, 0.6f, 0.6f, 1.f};
    glm::vec4 highlightedColor{0.7f, 0.7f, 0.7f, 1.f};
    glm::vec4 pressedColor{0.5f, 0.5f, 0.5f, 1.f};

    bool enabled{true};

    ButtonComponent(Font *font, std::string text = "Button");
};

#endif // RPG_BUTTONCOMPONENT_H
