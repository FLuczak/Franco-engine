#pragma once
#include <string>

#include "Engine/AssetManager.hpp"
#include "Engine/Component.hpp"
#include "SFML/Graphics/Sprite.hpp"

VISITABLE_STRUCT(sf::IntRect, left, top, width,height);
VISITABLE_STRUCT(sf::FloatRect, left, top, width,height);

class SpriteRenderComponent : public Component
{
public:
	SpriteRenderComponent(Entity& entityToSet);
	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;

	std::unique_ptr<sf::Sprite> sprite{};
	SERIALIZE_FIELD(std::string, spritePath)
	SERIALIZE_FIELD(sf::IntRect, spriteSize)
	SERIALIZE_FIELD(int, layer)
	~SpriteRenderComponent() override;
private:
	std::string oldSpritePath;
	sf::IntRect oldSpriteSize;
};
REGISTER_COMPONENT(SpriteRenderComponent);