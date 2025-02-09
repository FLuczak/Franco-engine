#pragma once
#include <ios>
#include <string>

#include "Component.hpp"
#include "Engine/Editor/EditorVariables.h"
#include "SFML/Graphics/Sprite.hpp"

struct SpriteAsset;
VISITABLE_STRUCT(sf::IntRect, left, top, width, height);
VISITABLE_STRUCT(sf::FloatRect, left, top, width,height);

class SpriteRenderComponent : public Component
{
public:
	SpriteRenderComponent(Entity& entityToSet);
	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;

	std::shared_ptr<SpriteAsset> spriteAsset{};
	SERIALIZE_FILE_PATH(.png, spritePath);
	SERIALIZE_FIELD(sf::IntRect, spriteSize)
	SERIALIZE_FIELD(int, layer)
	~SpriteRenderComponent() override;
	sf::Sprite spriteToDraw;
private:
	std::filesystem::path oldSpritePath;
	sf::IntRect oldSpriteSize;
};
REGISTER_COMPONENT(SpriteRenderComponent);