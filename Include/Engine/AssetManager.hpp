#pragma once
#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include "nlohmann/json.hpp"

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

enum class AssetType
{
	None,
	Sprite,
	EntityTemplate
};

struct Asset
{
	virtual ~Asset() = default;
	AssetType Type = AssetType::None;
	virtual std::any GetAsset() = 0;
};

struct TextureAsset : Asset
{
	std::any GetAsset() override
	{
		return &texture;
	}

	sf::Texture texture;
};

struct EntityTemplateAsset : Asset
{
	EntityTemplateAsset(const nlohmann::json json) : templateJson(json)
	{

	}

	std::any GetAsset() override
	{
		return std::any( &templateJson );
	}

	const nlohmann::json templateJson;
};

class AssetManager
{
public:
	static std::unique_ptr<sf::Sprite> GetTexture(std::string name, sf::IntRect size);
	static const nlohmann::json& GetEntityTemplate(std::string name);
private:
	static std::filesystem::path GetAssetsPath();
	static std::unordered_map<std::string, std::unique_ptr<Asset>> assets;
};

