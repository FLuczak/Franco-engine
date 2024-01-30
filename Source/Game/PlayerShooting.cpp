#include "Game/PlayerShooting.h"

#include "Engine/Engine.hpp"
#include "Engine/Physics/DiskCollider.hpp"
#include "Game/Bullet.h"
#include "Game/SpriteRenderComponent.hpp"
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
		auto& bullet =Engine.world.InstantiateTemplate("Templates/Bullet.ent");
		const auto mousePos = sf::Mouse::getPosition(Engine.window);
		sf::Vector2f direction = sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) - GetTransform().position;
		direction = sf::normalize(direction);
		bullet.GetTransform().position = GetTransform().position + sf::Vector2f(32,32) + direction * distanceFromPlayer;
		bullet.GetComponent<PhysicsBody>()->velocity = direction * bulletSpeed;

		timer = shootingSpeed;
	}
}

void PlayerShooting::OnDestroy()
{
	Component::OnDestroy();
}
