#include "Engine/Core/AssetManager.hpp"
#include <filesystem>
#include <stdexcept>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <fstream>

std::unordered_map<std::string, std::shared_ptr<Asset>> AssetManager::assets;

std::shared_ptr<TextureAsset> AssetManager::LoadTexture(const std::filesystem::path& path)
{
    std::string key = path.string();
    size_t pos = key.find("Assets");
    if (pos != std::string::npos)
    {
        key = key.substr(pos + std::string("Assets/").length());
    }


    auto it = assets.find(key);
    if (it != assets.end())
        return std::dynamic_pointer_cast<TextureAsset>(it->second);

    auto textureAsset = std::make_shared<TextureAsset>();
    if (!textureAsset->texture.loadFromFile(key))
        throw std::runtime_error("Failed to load texture: " + key);

    textureAsset->Type = AssetType::Texture;
    assets[key] = textureAsset;
    return textureAsset;
}

std::shared_ptr<SpriteAsset> AssetManager::LoadSprite(const std::string& name, const sf::IntRect& rect)
{
    auto textureAsset = LoadTexture(GetAssetsPath() / name);

    std::string key = name + std::to_string(rect.left) + std::to_string(rect.top) + std::to_string(rect.width) + std::to_string(rect.height);
    size_t pos = key.find("Assets");
    if (pos != std::string::npos)
    {
        key = key.substr(pos + std::string("Assets/").length());
    }

    auto it = assets.find(key);
    if (it != assets.end())
        return std::dynamic_pointer_cast<SpriteAsset>(it->second);

    auto spriteAsset = std::make_shared<SpriteAsset>(textureAsset, rect);
    spriteAsset->Type = AssetType::Sprite;
    assets[key] = spriteAsset;
    return spriteAsset;
}

const nlohmann::json& AssetManager::GetEntityTemplate(const std::string& name)
{
    size_t pos = name.find("Assets");
    auto key = name;

    if (pos != std::string::npos)
    {
        key = name.substr(pos + std::string("Assets/").length());
    }

    if (!assets.contains(key))
    {
        const auto tempName = std::string(GetAssetsPath().string()) + '\\' + key;
        std::ifstream inputFile(tempName);

        if (inputFile.is_open())
        {
            nlohmann::json jsonData;
            inputFile >> jsonData;
            std::unique_ptr<EntityTemplateAsset> entityAsset = std::make_unique<EntityTemplateAsset>(jsonData);
            assets[key] = std::move(entityAsset);

            inputFile.close();
        }
        else
        {
            std::cerr << "Error opening the file:" << tempName << std::endl;
        }

    }

    auto asset = assets[key]->GetAsset();
    const auto json = std::any_cast<const nlohmann::json*>(asset);
    return *json;
}

AI::FiniteStateMachine& AssetManager::GetAnimationFSM(const std::string& name)
{
    size_t pos = name.find("Assets");
    auto key = name;
    if (pos != std::string::npos)
    {
        key = name.substr(pos + std::string("Assets/").length());
    }

    if (!assets.contains(key))
    {
        const auto tempName = std::string(GetAssetsPath().string()) + '\\' + key;
        std::ifstream inputFile(tempName);

        if (inputFile.is_open())
        {
            nlohmann::json jsonData;
            inputFile >> jsonData;
            std::unique_ptr<AnimationFSM> animationFSM = std::make_unique<AnimationFSM>(jsonData);
            assets[key] = std::move(animationFSM);

            inputFile.close();
        }
        else
        {
            std::cerr << "Error opening the file:" << tempName << std::endl;
        }
    }

    auto asset = assets[key]->GetAsset();
    const auto json = std::any_cast<AI::FiniteStateMachine*>(asset);
    return *json;
}

std::filesystem::path AssetManager::GetAssetsPath()
{
    std::filesystem::path assetsPath1 = std::filesystem::current_path() / "Assets";
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
