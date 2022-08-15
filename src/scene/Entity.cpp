#include "../pch.h"
#include "Entity.h"

Entity::Entity(entt::entity entity, entt::registry *registry)
        : m_entity(entity), m_registry(registry) {}
