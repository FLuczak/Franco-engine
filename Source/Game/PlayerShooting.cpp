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
		const auto mousePos = sf::Mouse::getPosition(Engine.window);
		auto& bullet =Engine.world.InstantiateTemplate(bulletPrefab.path.string());
		sf::Vector2f direction = sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) - GetTransform().position + CameraComponent::GetMainCamera()->GetTransform().position;
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
