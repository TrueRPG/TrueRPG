#include "../../../pch.h"
#include "Allocator.h"

namespace vk
{

Allocator::Allocator(VmaAllocator allocator)
    : m_allocator(allocator)
{}

void Allocator::destroy()
{
    vmaDestroyAllocator(m_allocator);
}

Allocator::operator VmaAllocator() const
{
    return m_allocator;
}

}
