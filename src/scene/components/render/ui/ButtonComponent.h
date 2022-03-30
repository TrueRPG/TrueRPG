#ifndef RPG_BUTTONCOMPONENT_H
#define RPG_BUTTONCOMPONENT_H

#include "../../../../client/graphics/Font.h"

struct ButtonComponent
{
    Font *font;
    std::string text;
    glm::vec2 size{100.f, 50.f};

    ButtonComponent(Font *font, std::string text = "");
};

#endif // RPG_BUTTONCOMPONENT_H
