#include "../../pch.h"
#include "SpriteRendererComponent.h"

SpriteRendererComponent::SpriteRendererComponent(Texture texture)
        : texture(texture),
          textureRect(0, 0, texture.getWidth(), texture.getHeight()) {}
