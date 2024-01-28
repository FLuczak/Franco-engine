#include "Game/BaseCollider.h"

#include "Engine/Entity.hpp"
#include "Engine/Physics/PhysicsBody.hpp"

BaseCollider::BaseCollider(Entity& entityToSet): Component(entityToSet)
{
	PhysicsBody* body = entityToSet.GetComponent<PhysicsBody>();
	if (body == nullptr)return;
	OnColliderCreated(*this, *body);
}

void BaseCollider::Start()
{
	Component::Start();
}

void BaseCollider::Update(float deltaTime)
{
	Component::Update(deltaTime);
}

void BaseCollider::OnDestroy()
{
	Component::OnDestroy();
}
