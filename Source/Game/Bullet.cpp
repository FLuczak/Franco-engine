#include "Game/Bullet.h"

#include "Engine/Core/Engine.hpp"

void Bullet::Start()
{
	Component::Start();
	timer = lifeTime;
}

void Bullet::Update(float deltaTime)
{
	Component::Update(deltaTime);
	timer -= deltaTime;

	if(timer <= 0.0f)
	{
		Engine.world.Destroy(GetEntity());
	}
}

void Bullet::OnDestroy()
{
	Component::OnDestroy();
}

void Bullet::OnCollisionEnter(CollisionEvent& event)
{
	Component::OnCollisionEnter(event);
	if (event.otherCollider.GetEntity().GetComponent<Bullet>())return;
	Engine.world.Destroy(GetEntity());
}

void Bullet::OnCollisionStay(CollisionEvent& event)
{
	Component::OnCollisionStay(event);
	if (event.otherCollider.GetEntity().GetComponent<Bullet>())return;
	Engine.world.Destroy(GetEntity());
}

void Bullet::OnCollisionLeave(CollisionEvent& event)
{
	Component::OnCollisionLeave(event);
}
