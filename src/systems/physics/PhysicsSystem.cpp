#include "../../pch.h"
#include "PhysicsSystem.h"

#include "../../components/physics/RigidbodyComponent.h"
#include "../../components/basic/TransformComponent.h"
#include "../../client/graphics/Rect.h"
#include "../../components/physics/RectColliderComponent.h"

PhysicsSystem::PhysicsSystem(entt::registry &registry)
        : m_registry(registry) {}

void PhysicsSystem::update(float deltaTime)
{
    auto view = m_registry.view<RigidbodyComponent>();
    for (auto entity : view)
    {
        auto &rigidbody = view.get<RigidbodyComponent>(entity);
        auto &transform = m_registry.get<TransformComponent>(entity);

        glm::vec2 nextPos = transform.position + rigidbody.velocity * deltaTime;

        if (m_registry.all_of<RectColliderComponent>(entity))
        {
            auto &rectCollider = m_registry.get<RectColliderComponent>(entity);

            auto otherView = m_registry.view<RectColliderComponent>();
            for (auto otherEntity : otherView)
            {
                if (entity == otherEntity) continue;
                auto &otherTransform = m_registry.get<TransformComponent>(otherEntity);
                auto &otherRectCollider = m_registry.get<RectColliderComponent>(otherEntity);

                bool collide = Rect{
                    nextPos.x + rectCollider.offset.x,
                    nextPos.y + rectCollider.offset.y,
                    rectCollider.size.x, rectCollider.size.y
                }.intersects({
                       otherTransform.position.x + otherRectCollider.offset.x,
                    otherTransform.position.y + otherRectCollider.offset.y,
                    otherRectCollider.size.x, otherRectCollider.size.y
                });

                if (collide)
                {
                    nextPos = transform.position;
                }
            }
        }
        transform.position = nextPos;
    }
}
