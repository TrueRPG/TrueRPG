#ifndef RPG_SCRIPTSYSTEM_H
#define RPG_SCRIPTSYSTEM_H

#include "entt.hpp"
#include "../../scene/ISystem.h"

class ScriptSystem : public ISystem
{
    entt::registry &m_registry;
public:
    ScriptSystem(entt::registry &registry);

    void update(float deltaTime) override;

    void destroy() override;

private:
    void destroyScript(entt::registry &registry, entt::entity entity);
};

#endif //RPG_SCRIPTSYSTEM_H
