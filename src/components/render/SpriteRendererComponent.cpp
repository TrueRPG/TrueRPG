#include "../../pch.h"
#include "SpriteRendererComponent.h"

SpriteRendererComponent::SpriteRendererComponent(ITexture *texture)
        : texture(texture),
          textureRect(0, 0, texture->getWidth(), texture->getHeight()) {}
