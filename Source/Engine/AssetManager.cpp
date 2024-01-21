#include "Engine/AssetManager.hpp"

std::unordered_map<std::string, std::unique_ptr<Asset>> AssetManager::assets{};

std::unique_ptr<sf::Sprite> AssetManager::GetTexture(std::string name, sf::IntRect size)
{
	if (!assets.contains(name))
	{
		std::unique_ptr<TextureAsset> textureAsset = std::make_unique<TextureAsset>();
		textureAsset->texture.loadFromFile(name);
		assets[name] = std::move(textureAsset);
	}

	auto textureAsset = std::get<sf::Texture*>(assets[name]->GetAsset());

	if (size.height == 0 || size.width == 0)
	{
		return std::make_unique<sf::Sprite>(*textureAsset);
	}
	else
	{
		return std::make_unique<sf::Sprite>(*textureAsset, size);
	}
}