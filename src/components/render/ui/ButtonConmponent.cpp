#include "../../../pch.h"
#include <utility>

#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(Font *font, std::string text)
    : font(font),
      text(text)
{
}
