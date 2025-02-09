#pragma once
#include <any>
#include <memory>
#include <string>
#include <unordered_map>

#include "Engine/AI/FiniteStateMachines/FiniteStateMachine.hpp"
#include "nlohmann/json.hpp"

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

enum class AssetType
{
	None,
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
	std::any GetAsset() override
	{
		return &texture;
	}

	sf::Texture texture;
};

struct AnimationFSM : Asset
{
	AnimationFSM(nlohmann::json json)
	{
		auto ss = std::stringstream(json.dump());
		fsm = AI::FiniteStateMachine::DeserializeFromStringStream(ss);
	}

	std::any GetAsset() override
	{
		return std::any(fsm.get());
	}

	std::unique_ptr<AI::FiniteStateMachine> fsm;
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
	static std::unique_ptr<sf::Sprite> GetTexture(std::filesystem::path path, sf::IntRect size);
	static const nlohmann::json& GetEntityTemplate(std::string name);
	static AI::FiniteStateMachine& GetAnimationFSM(std::string name);
private:
	static std::filesystem::path GetAssetsPath();
	static std::unordered_map<std::string, std::unique_ptr<Asset>> assets;
};

