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
		auto& bullet =Engine.world.Instantiate("Bullet");
		const auto mousePos = sf::Mouse::getPosition(Engine.window);

		sf::Vector2f direction =sf::Vector2f( static_cast<float>(mousePos.x),static_cast<float>(mousePos.y)) - GetTransform().position;
		direction = sf::normalize(direction);

		bullet.GetTransform().position = GetTransform().position + direction * 100.0f;
		bullet.GetTransform().scale = { 2,2 };

		auto& physicsBody = bullet.AddComponent<PhysicsBody>();

		physicsBody.velocity = direction * bulletSpeed;
		physicsBody.mass = 100.0f;
		physicsBody.bodyType = PhysicsBodyType::KINEMATIC;

		auto& diskCollider = bullet.AddComponent<DiskCollider>();
		diskCollider.radius = 8.0f;
		diskCollider.offset = { 4.0f,4.0f };

		auto& sprite = bullet.AddComponent<SpriteRenderComponent>();
		sprite.spritePath = "Assets/Bullets.png";
		sprite.spriteSize = sf::IntRect(180, 132, 8, 8);
		timer = shootingSpeed;

		bullet.AddComponent<Bullet>(3.0f);
	}
}

void PlayerShooting::OnDestroy()
{
	Component::OnDestroy();
}
