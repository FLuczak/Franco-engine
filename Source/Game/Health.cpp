#include "Game/Health.h"

#include "Engine/BaseCollider.h"
#include "Engine/Entity.hpp"
#include "Engine/Physics/Collision.hpp"
#include "Game/Bullet.h"

void Health::ChangeHealth(int amount)
{
	health += amount;

	if (health <= 0)
	{
		OnDied();
	}
	else
	{
		OnHealthChanged(health, maxHealth);
	}
}

void Health::SetHealth(int toSet)
{
	health = toSet;

	if(health <= 0)
	{
		OnDied();
	}
	else
	{
		OnHealthChanged(health, maxHealth);
	}
}

void Health::OnCollisionEnter(CollisionEvent& event)
{
	Component::OnCollisionEnter(event);
	auto bullet = event.otherCollider.GetEntity().GetComponent<Bullet>();
	if (bullet == nullptr)return;
	if (event.otherCollider.GetEntity().tag == GetEntity().tag)return;
	ChangeHealth(-1);
}

void Health::Start()
{
	Component::Start();
	health = maxHealth;
}
