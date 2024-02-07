#include "Game/DoorComponent.h"

#include "Engine/Engine.hpp"
#include "Engine/Entity.hpp"
#include "Engine/Physics/Collision.hpp"
#include "Game/BaseCollider.h"
#include "Game/PlayerMovement.hpp"

void DoorComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);
	Engine.debugRenderer.AddLine(DebugCategory::Gameplay, GetTransform().position, GetTransform().position + doorDirection * doorModifier, sf::Color::Red);
	Engine.debugRenderer.AddText(DebugCategory::Gameplay, GetTransform().position + sf::Vector2f(32, 32), std::to_string(roomTo), sf::Color::Red);
}

void DoorComponent::OnCollisionEnter(CollisionEvent& event)
{
	Component::OnCollisionEnter(event);
	auto& entity = event.otherCollider.GetEntity();
	if (entity.GetComponent<PlayerMovement>() == nullptr)return;
	entity.GetTransform().position = GetTransform().position+ doorDirection * doorModifier;
	doorOpened(*this);
}
