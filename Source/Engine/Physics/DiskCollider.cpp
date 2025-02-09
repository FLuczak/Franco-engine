#include "Engine/Physics/DiskCollider.hpp"

#include <Engine/Core/Engine.hpp>

void DiskCollider::Start()
{
	BaseCollider::Start();
	colliderShape = ColliderShape::DISK;
}

void DiskCollider::Update(float deltaTime)
{
	BaseCollider::Update(deltaTime);
	Engine.debugRenderer.AddCircle(DebugCategory::Physics, GetTransform().position + offset, radius, sf::Color::Red);
}

void DiskCollider::OnDestroy()
{
	BaseCollider::OnDestroy();
}

geometry2d::AABB DiskCollider::AABB()
{
	return { GetEntity().GetTransform().position - sf::Vector2f(radius,radius) + offset, GetEntity().GetTransform().position + sf::Vector2f(radius,radius) + offset};
}
