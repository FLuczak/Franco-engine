#pragma once
#include "Engine/Core/Component.hpp"
#include "Engine/Editor/EditorVariables.h"
#include "SFML/SFMLMath.hpp"
#include "SFML/Graphics/Texture.hpp"

class BulletEmitter : public Component
{
public:
	SERIALIZE_FIELD(float, fireRate)
	SERIALIZE_FIELD(float, bulletSpeed)
	SERIALIZE_FIELD(float, delay)
	SERIALIZE_FILE_PATH(.ent, bulletTemplate)
	SERIALIZE_FIELD(bool, active)

	BulletEmitter(Entity& entityToSet): Component(entityToSet), fireRate(0), bulletSpeed(0), delay(0), active(true)
	{
	}

	sf::Vector2f GetDirectionToPlayer() const;
	void Start() override;
	void Update(float deltaTime) override;
	virtual void Fire();
protected:
	float timer = 0.0f;
};

class SingleEmitter :public BulletEmitter
{
public:
	explicit SingleEmitter(Entity& entityToSet) : BulletEmitter(entityToSet)
	{
	}

	void Fire() override;
};
REGISTER_COMPONENT(SingleEmitter);

class RadialEmitter : public BulletEmitter
{
public:
	SERIALIZE_FIELD(int, angle);
	SERIALIZE_FIELD(int, step);
	SERIALIZE_FIELD(float, radius);
	SERIALIZE_FIELD(sf::Vector2f, offset);

	explicit RadialEmitter(Entity& entityToSet): BulletEmitter(entityToSet)
	{
	}

	void Fire() override;
};
REGISTER_COMPONENT(RadialEmitter);

class SpiralEmitter : public BulletEmitter
{
public:
	SERIALIZE_FIELD(float, rotationSpeed);
	SERIALIZE_FIELD(int, numberOfBullets);
	SERIALIZE_FIELD(float, radius);
	SERIALIZE_FIELD(sf::Vector2f, offset);


	explicit SpiralEmitter(Entity& entityToSet): BulletEmitter(entityToSet)
	{
	}

	void Fire() override;
	void Update(float deltaTime) override;
private:
	sf::Vector2f direction {0,1};
};
REGISTER_COMPONENT(SpiralEmitter);

struct Pattern
{
	Pattern(const sf::Texture& tex)
	{
		sf::Vector2u texSize = tex.getSize();
		unsigned int texWidth = texSize.x;
		unsigned int texHeight = texSize.y;
		auto image = tex.copyToImage();

		for (unsigned int y = 0; y < texHeight; ++y)
		{
			for (unsigned int x = 0; x < texWidth; ++x)
			{
				const unsigned int pixelIndex = (y * texWidth + x) * 4; // 4 channels: RGBA

				sf::Color col = image.getPixel(y, x);

				if (col == sf::Color::Black)
				{
					const unsigned int patternX = x * 8 / texWidth;
					const unsigned int patternY = y * 8 / texHeight;
					bulletPattern[patternY][patternX] = true;
				}
			}
		}
	}

	bool bulletPattern[8][8] = { false };
};

class PatternEmitter : public BulletEmitter
{
public:
	SERIALIZE_FIELD(float, scale)
	SERIALIZE_FIELD(std::vector<std::string>, templates);
	SERIALIZE_FIELD(sf::Vector2f, offset)

	explicit PatternEmitter(Entity& entityToSet) : BulletEmitter(entityToSet)
	{
	}

	void Start() override;
	void Fire() override;
	void Update(float deltaTime) override;
private:
	std::vector<Pattern> patterns;
};
REGISTER_COMPONENT(PatternEmitter);