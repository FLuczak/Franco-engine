#include "Game/BulletEmitter.h"

#include "Engine/AssetManager.hpp"
#include "Engine/Engine.hpp"

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

void RadialEmitter::Fire()
{
	BulletEmitter::Fire();
	for(int i = 0 ; i < angle; i+= step)
	{
		auto& bullet = Engine.world.InstantiateTemplate(bulletTemplate);
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
		auto& bullet = Engine.world.InstantiateTemplate(bulletTemplate);
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
				auto& bullet = Engine.world.InstantiateTemplate(bulletTemplate);
				bullet.GetTransform().position = GetTransform().position + offset + sf::Vector2f(j,i)*scale;
				auto body = bullet.GetComponent<PhysicsBody>();
				if (body == nullptr)return;
				body->velocity = sf::getRotated(sf::Vector2f(0, 1), GetTransform().rotation);
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
