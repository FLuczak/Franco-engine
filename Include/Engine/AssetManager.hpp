#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

enum class AssetType
{
	None,
	Sprite
};

struct Asset
{
	virtual ~Asset() = default;
	AssetType Type = AssetType::None;
	virtual std::variant<sf::Texture*> GetAsset() = 0;
};

struct TextureAsset : Asset
{
	std::variant<sf::Texture*> GetAsset() override
	{
		return &texture;
	}

	sf::Texture texture;
};

class AssetManager
{
public:
	static std::unique_ptr<sf::Sprite> GetTexture(std::string name, sf::IntRect size);
private:
	static std::unordered_map<std::string, std::unique_ptr<Asset>> assets;
};

