#pragma once

#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "nlohmann/json.hpp"
#include "Engine/AI/FiniteStateMachines/FiniteStateMachine.hpp"

enum class AssetType
{
    None,
    Texture,
    Sprite,
    EntityTemplate,
    AnimationFSM
};

struct Asset
{
    virtual ~Asset() = default;
    AssetType Type = AssetType::None;
    virtual std::any GetAsset() = 0;
};

struct TextureAsset : Asset
{
    std::any GetAsset() override { return &texture; }
    sf::Texture texture;
};

struct SpriteAsset : Asset
{
    SpriteAsset(std::shared_ptr<TextureAsset> textureAsset, sf::IntRect rect)
    {
        sprite = sf::Sprite(textureAsset->texture);
        sprite.setTextureRect(rect);
    }
    std::any GetAsset() override { return &sprite; }
    sf::Sprite sprite;
};

struct AnimationFSM : Asset
{
    AnimationFSM(nlohmann::json json)
    {
        auto ss = std::stringstream(json.dump());
        fsm = AI::FiniteStateMachine::DeserializeFromStringStream(ss);
    }
    std::any GetAsset() override { return fsm.get(); }
    std::unique_ptr<AI::FiniteStateMachine> fsm;
};

struct EntityTemplateAsset : Asset
{
    EntityTemplateAsset(nlohmann::json json) : templateJson(json) {}
    std::any GetAsset() override { return &templateJson; }
    const nlohmann::json templateJson;
};

class AssetManager
{
public:
    static std::shared_ptr<TextureAsset> LoadTexture(const std::filesystem::path& path);
    static std::shared_ptr<SpriteAsset> LoadSprite(const std::string& name, const sf::IntRect& rect);
    static const nlohmann::json& GetEntityTemplate(const std::string& name);
    static AI::FiniteStateMachine& GetAnimationFSM(const std::string& name);

private:
    static std::unordered_map<std::string, std::shared_ptr<Asset>> assets;
    static std::filesystem::path GetAssetsPath();
};