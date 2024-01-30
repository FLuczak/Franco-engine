#include "Game/Bullet.h"

#include "Engine/Engine.hpp"

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
	Engine.world.Destroy(GetEntity());
}

void Bullet::OnCollisionStay(CollisionEvent& event)
{
	Component::OnCollisionStay(event);
	Engine.world.Destroy(GetEntity());
}

void Bullet::OnCollisionLeave(CollisionEvent& event)
{
	Component::OnCollisionLeave(event);
}
