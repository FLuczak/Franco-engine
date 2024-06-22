#include "Engine/AssetManager.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>

#include "Engine/EditorUtility.h"

std::unordered_map<std::string, std::unique_ptr<Asset>> AssetManager::assets{};

std::unique_ptr<sf::Sprite> AssetManager::GetTexture(std::string name, sf::IntRect size)
{
	size_t pos = name.find("Assets");
	if (pos != std::string::npos)
	{
		name = name.substr(pos + std::string("Assets/").length());
	}

	if (!assets.contains(name))
	{
		std::unique_ptr<TextureAsset> textureAsset = std::make_unique<TextureAsset>();
		const auto tempName = std::string(GetAssetsPath().string()) + '\\' + name;

		textureAsset->texture.loadFromFile(tempName);
		assets[name] = std::move(textureAsset);
	}

	const auto textureAsset = (std::any_cast<sf::Texture*>(assets[name]->GetAsset()));

	if (size.height == 0 || size.width == 0)
	{
		return std::make_unique<sf::Sprite>(*textureAsset);
	}
	else
	{
		return std::make_unique<sf::Sprite>(*textureAsset, size);
	}
}

std::unique_ptr<sf::Sprite> AssetManager::GetTexture(std::filesystem::path path, sf::IntRect size)
{
	return GetTexture(path.string(), size);
}

const nlohmann::json& AssetManager::GetEntityTemplate(std::string name)
{
	size_t pos = name.find("Assets");
	if (pos != std::string::npos)
	{
		name = name.substr(pos + std::string("Assets/").length());
	}

	if (!assets.contains(name))
	{
		const auto tempName = std::string(GetAssetsPath().string()) + '\\' + name;
		std::ifstream inputFile(tempName);

		if (inputFile.is_open())
		{
			nlohmann::json jsonData;
			inputFile >> jsonData;
			std::unique_ptr<EntityTemplateAsset> entityAsset = std::make_unique<EntityTemplateAsset>(jsonData);
			assets[name] = std::move(entityAsset);

			inputFile.close();
		}
		else
		{
			std::cerr << "Error opening the file:" << tempName << std::endl;
		}

	}

	auto asset = assets[name]->GetAsset();
	const auto json = std::any_cast<const nlohmann::json*>(asset);
	return *json;
}

AI::FiniteStateMachine& AssetManager::GetAnimationFSM(std::string name)
{
	size_t pos = name.find("Assets");
	if (pos != std::string::npos)
	{
		name = name.substr(pos + std::string("Assets/").length());
	}

	if (!assets.contains(name))
	{
		const auto tempName = std::string(GetAssetsPath().string()) + '\\' + name;
		std::ifstream inputFile(tempName);

		if (inputFile.is_open())
		{
			nlohmann::json jsonData;
			inputFile >> jsonData;
			std::unique_ptr<AnimationFSM> animationFSM = std::make_unique<AnimationFSM>(jsonData);
			assets[name] = std::move(animationFSM);

			inputFile.close();
		}
		else
		{
			std::cerr << "Error opening the file:" << tempName << std::endl;
		}
	}

	auto asset = assets[name]->GetAsset();
	const auto json = std::any_cast<AI::FiniteStateMachine*>(asset);
	return *json;
}

std::filesystem::path AssetManager::GetAssetsPath()
{
	std::filesystem::path assetsPath1 = std::filesystem::current_path() / "Assets" ;
	std::filesystem::path assetsPath2 = std::filesystem::current_path().parent_path().parent_path() / "Assets";

	if (std::filesystem::exists(assetsPath1)) 
	{
		return assetsPath1;
	}
	else 
	{
		return assetsPath2;
	}
}

