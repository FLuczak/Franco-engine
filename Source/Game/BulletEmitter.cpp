#include "Game/BulletEmitter.h"

#include "Engine/Core/AssetManager.hpp"
#include "Engine/Core/Engine.hpp"
#include "Game/PlayerMovement.hpp"
#include "Engine/Core/SpriteRenderComponent.hpp"

sf::Vector2f BulletEmitter::GetDirectionToPlayer() const
{
	const auto players = Engine.world.GetEntitiesOfType<PlayerMovement>();
	if (players.empty())return {};
	auto vector =players[0].get().GetTransform().position - GetTransform().position;
	return  sf::normalize(vector);
}

void BulletEmitter::Start()
{
	Component::Start();
	timer = fireRate;
}

void BulletEmitter::Update(float deltaTime)
{
	Component::Update(deltaTime);

	timer -= deltaTime;
	if(timer <= 0)
	{
		Fire();
		timer = fireRate;
	}
}

void BulletEmitter::Fire()
{

}

void SingleEmitter::Fire()
{
	BulletEmitter::Fire();
	auto& bullet = Engine.world.InstantiateTemplate(bulletTemplate.path.string());
	sf::Vector2f direction = GetDirectionToPlayer();

	const auto sprite = GetEntity().GetComponent<SpriteRenderComponent>();
	if(sprite != nullptr)
	{
		const auto spriteSize = sprite->spriteSize;
		bullet.GetTransform().position = GetTransform().position + sf::Vector2f(static_cast<float>(spriteSize.width) / 2.0f, static_cast<float>(spriteSize.height) / 2.0f) +direction * 64;

	}

	const auto body = bullet.GetComponent<PhysicsBody>();
	if (body == nullptr)return;
	body->velocity = sf::normalize(direction) * bulletSpeed;
	bullet.tag = Tag::Enemy;
}

void RadialEmitter::Fire()
{
	BulletEmitter::Fire();
	for(int i = 0 ; i < angle; i+= step)
	{
		auto& bullet = Engine.world.InstantiateTemplate(bulletTemplate.path.string());
		sf::Vector2f direction = { 0,1 };
		direction = sf::getRotated(direction, i);
		direction *= radius;
		bullet.GetTransform().position = GetTransform().position + direction + offset ;
		auto body = bullet.GetComponent<PhysicsBody>();
		if (body == nullptr)return;
		body->velocity = sf::normalize(direction) * bulletSpeed;
		bullet.tag = Tag::Enemy;
	}
}

void SpiralEmitter::Fire()
{
	BulletEmitter::Fire();
	for (int i = 0; i < numberOfBullets; i++)
	{
		auto& bullet = Engine.world.InstantiateTemplate(bulletTemplate.path.string());
		float angle = i * sf::PI * 2 / numberOfBullets;
		float x = cos(angle) * radius;
		float y = sin(angle) * radius;
		sf::Vector2 radialOffset = sf::Vector2(x, y);
		bullet.GetTransform().position = GetTransform().position + offset + radialOffset;
		auto body = bullet.GetComponent<PhysicsBody>();
		if (body == nullptr)return;
		body->velocity = sf::normalize(direction) * bulletSpeed;
		bullet.tag = Tag::Enemy;
	}
}

void SpiralEmitter::Update(float deltaTime)
{
	BulletEmitter::Update(deltaTime);
	direction = sf::getRotated(direction, rotationSpeed * deltaTime);
}

void PatternEmitter::Start()
{
	BulletEmitter::Start();
	for (auto path : templates)
	{
		auto tex = AssetManager::GetTexture(path, { 0,0,0,0 });
		patterns.emplace_back(*tex->getTexture());
	}
}

void PatternEmitter::Fire()
{
	BulletEmitter::Fire();
	const auto& pattern = patterns[rand() % patterns.size()];

	for (int i = 0 ; i < 8; i++)
	{
		for(int j = 0; j < 8 ; j++)
		{
			if(pattern.bulletPattern[j][i])
			{
				auto& bullet = Engine.world.InstantiateTemplate(bulletTemplate.path.string());
				bullet.GetTransform().position = GetTransform().position + offset + sf::Vector2f(j,i)*scale;
				auto body = bullet.GetComponent<PhysicsBody>();
				if (body == nullptr)return;
				body->velocity = GetDirectionToPlayer();
				body->velocity = sf::normalize(body->velocity) * bulletSpeed;
				bullet.tag = Tag::Enemy;
			}
		}
	}
}

void PatternEmitter::Update(float deltaTime)
{
	BulletEmitter::Update(deltaTime);
}
