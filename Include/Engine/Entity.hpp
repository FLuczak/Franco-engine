#pragma once
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include "Component.hpp"
#include "SFML/System/Vector2.hpp"
#include "nlohmann/json.hpp"

class Component;

struct Transform
{
	sf::Vector2f position{};
	sf::Vector2f scale{1,1};
	float rotation = 0.0f;
};

class Entity
{
public:
	Entity(unsigned int idToSet, std::string& nameToSet) : name(nameToSet), id(idToSet) {}
	Entity(unsigned int idToSet) : id(idToSet) {}
	Entity(Entity&& other) { other.components = std::move(components); }

	unsigned int GetId() const { return id; }
	Transform& GetTransform(){return transform;}

	template<typename ComponentType, typename ...Args>
	ComponentType& AddComponent(Args& ... args);

	void OnDestroy() const;
	void RemoveComponent(Component& component);

	void Update(bool isGame);
	nlohmann::json Serialize();
	void Deserialize(nlohmann::json& json);

	std::string name = {};
	std::vector<std::unique_ptr<Component>> components{};
private:
	std::queue<int> componentsToRemove{};
	Transform transform;
	unsigned int id = 0u;
};

template <typename ComponentType, typename ... Args>
ComponentType& Entity::AddComponent(Args& ... args)
{
	components.emplace_back(std::make_unique<ComponentType>( args... ));
	components[components.size() - 1]->Start();
	return dynamic_cast<ComponentType&>(*components[components.size() - 1]);
}

