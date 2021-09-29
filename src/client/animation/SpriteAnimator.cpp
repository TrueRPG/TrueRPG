#include "SpriteAnimator.h"

SpriteAnimatorBuilder::SpriteAnimatorBuilder()
{
    m_animator.nodes.push_back({"entry"});
}

SpriteAnimatorBuilder::Node SpriteAnimatorBuilder::entry()
{
    return {&m_animator, &m_animator.nodes.front()};
}

SpriteAnimatorBuilder::Node SpriteAnimatorBuilder::node(std::string name, std::vector<SpriteAnimationFrame> frames)
{
    auto *n = &m_animator.nodes.emplace_back(SpriteAnimatorNode{std::move(name), std::move(frames)});

    return {&m_animator, n};
}

SpriteAnimatorBuilder::Parameter SpriteAnimatorBuilder::parameter(std::string name, SpriteAnimatorParameterType type)
{
    auto *p = &m_animator.parameters.emplace_back(SpriteAnimatorParameter{std::move(name), type});

    return {&m_animator, p};
}

SpriteAnimator SpriteAnimatorBuilder::build()
{
    return std::move(m_animator);
}

SpriteAnimatorBuilder::Node::Node(SpriteAnimator *animator, SpriteAnimatorNode *node) :
    m_animator(animator), m_node(node)
{
}

void SpriteAnimatorBuilder::Node::transition(SpriteAnimatorBuilder::Node &destination,
                                             std::function<bool(const SpriteAnimatorParameterStorage &)> condition)
{
    SpriteAnimatorTransition *t = &m_animator->transitions.emplace_back(
        SpriteAnimatorTransition{m_node, destination.m_node, std::move(condition)});

    m_node->fromTransitions.push_back(t);
    destination.m_node->toTransitions.push_back(t);
}

SpriteAnimatorBuilder::Parameter::Parameter(SpriteAnimator *animator, SpriteAnimatorParameter *parameter) :
    m_animator(animator), m_parameter(parameter)
{
}
