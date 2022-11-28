#include "../../pch.h"
#include "SpriteRendererComponent.h"
#include "../../client/graphics/ITexture.h"

SpriteRendererComponent::SpriteRendererComponent(ITexture *texture)
        : texture(texture),
          textureRect(0, 0, texture->getWidth(), texture->getHeight()) {}
