#include "Game/PlayerShooting.h"

#include "Engine/Core/Engine.hpp"
#include "Engine/Physics/DiskCollider.hpp"
#include "Game/Bullet.h"
#include "SFML/Window/Mouse.hpp"

void PlayerShooting::Start()
{
	Component::Start();
}

void PlayerShooting::Update(float deltaTime)
{
	Component::Update(deltaTime);

	timer -= deltaTime;
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && timer <= 0.0f)
	{
		auto& bullet =Engine.world.InstantiateTemplate(bulletPrefab.path.string());
		const sf::Vector2f direction = { 0,-1 };
		bullet.GetTransform().position = GetTransform().position + sf::Vector2f(16, 16) + direction * distanceFromPlayer;
		bullet.GetComponent<PhysicsBody>()->velocity = direction * bulletSpeed;
		bullet.tag = Tag::Player;

		timer = shootingSpeed;
	}
}

void PlayerShooting::OnDestroy()
{
	Component::OnDestroy();
}
