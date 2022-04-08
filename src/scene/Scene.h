#ifndef RPG_SCENE_H
#define RPG_SCENE_H

#include <entt.hpp>
#include "ISystem.h"

class Entity;

class Scene
{
    entt::registry m_registry;
    std::vector<ISystem*> m_systems;

public:
    ~Scene();

    Entity createEntity(const std::string& name = "");

    void destroyEntity(Entity entity);

    template<typename T>
    decltype(auto) addSystem()
    {
        m_systems.push_back(new T(m_registry));
        return (T&)*m_systems.back();
    }

    void create();

    void update(float deltaTime);

    void destroy();
};

#endif //RPG_SCENE_H
