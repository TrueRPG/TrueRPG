#ifndef RPG_ENTITY_H
#define RPG_ENTITY_H

#include <entt.hpp>
#include "Scene.h"

class Entity
{
    entt::entity m_entity{entt::null};
    entt::registry *m_registry = nullptr;

public:
    Entity() = default;

    Entity(entt::entity entity, entt::registry *registry);

    template<typename T, typename... Args>
    decltype(auto) addComponent(Args &&... args)
    {
        return m_registry->emplace<T>(m_entity, std::forward<Args>(args)...);
    }

    template<typename T>
    T &getComponent()
    {
        return m_registry->get<T>(m_entity);
    }

    template<typename T>
    bool hasComponent()
    {
        return m_registry->all_of<T>(m_entity);
    }

    template<typename T>
    void removeComponent()
    {
        m_registry->remove<T>(m_entity);
    }

    operator bool() const { return m_entity != entt::null; }

    bool operator==(const Entity &entity)
    {
        return m_entity == entity.m_entity;
    }

    friend class Scene;
};

#endif //RPG_ENTITY_H
