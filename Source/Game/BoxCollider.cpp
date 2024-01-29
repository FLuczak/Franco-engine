#include "Game/BoxCollider.h"
#include "Engine/Engine.hpp"

void BoxCollider::Start()
{
	BaseCollider::Start();
}

void BoxCollider::Update(float deltaTime)
{
	BaseCollider::Update(deltaTime);
	Engine.debugRenderer.AddSquare(DebugCategory::Physics, GetTransform().position + sf::Vector2f(extents / 2.0f,extents/2.0f) + offset, extents, sf::Color::Red);
}

void BoxCollider::OnDestroy()
{
	BaseCollider::OnDestroy();
}
