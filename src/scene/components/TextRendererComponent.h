#ifndef RPG_TEXTRENDERERCOMPONENT_H
#define RPG_TEXTRENDERERCOMPONENT_H

#include "../../client/graphics/Font.h"

enum class HorizontalAlign
{
    Left, Right, Center
};

enum class VerticalAlign
{
    Top, Bottom, Center
};

struct TextRendererComponent
{
    Font *font;
    std::string text;
    glm::vec4 color{1.f};
    HorizontalAlign horizontalAlign = HorizontalAlign::Left;
    VerticalAlign verticalAlign = VerticalAlign::Bottom;

    int layer{0};
    int order{0};

    TextRendererComponent(Font *font, std::string text = "");
};

#endif //RPG_TEXTRENDERERCOMPONENT_H
