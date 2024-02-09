#pragma once
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include "Component.hpp"
#include "SFML/System/Vector2.hpp"
#include "nlohmann/json.hpp"
#include "Physics/Collision.hpp"

class Component;

struct Transform
{
	sf::Vector2f position{};
	sf::Vector2f scale{1,1};
	float rotation = 0.0f;
};


using entity_id = size_t;

enum class Tag
{
	Untagged,
	Player,
	Enemy,
	UI
};

class Entity
{
public:
	Entity(unsigned int idToSet, std::string& nameToSet) : name(nameToSet), id(idToSet) {}
	Entity(unsigned int idToSet) : id(idToSet) {}
	Entity(Entity&& other) { other.components = std::move(components); }

	entity_id GetId() const { return id; }
	Transform& GetTransform(){return transform;}

	Tag tag;

	template<typename ComponentType, typename ...Args>
	ComponentType& AddComponent(Args&& ... args, bool startComponent);


	template<typename ComponentType>
	ComponentType* GetComponent();

	void Start(bool isGame) const;
	void OnDestroy() const;
	void RemoveComponent(Component& component);

	void Update(float deltaTime, bool isGame);

	void OnCollisionEnter(CollisionEvent& event) const;
	void OnCollisionStay(CollisionEvent& event) const;
	void OnCollisionLeave(CollisionEvent& event) const;

	nlohmann::json Serialize();
	void Deserialize(nlohmann::json& json);

	bool active = true;
	std::string name = {};
	std::vector<std::unique_ptr<Component>> components{};
private:
	std::queue<int> componentsToRemove{};
	Transform transform;
	entity_id id = 0u;
};

template <typename ComponentType, typename ... Args>
ComponentType& Entity::AddComponent(Args&& ... args,bool startComponent)
{
	components.emplace_back(std::make_unique<ComponentType>(*this,args... ));
	if(startComponent)components[components.size() - 1]->Start();
	return dynamic_cast<ComponentType&>(*components[components.size() - 1]);
}

template <typename ComponentType>
ComponentType* Entity::GetComponent()
{
	for (auto& component : components) 
	{
		auto derivedComponent = dynamic_cast<ComponentType*>(component.get());
		if (derivedComponent != nullptr) 
		{
			return static_cast<ComponentType*>(component.get());
		}
	}

	return nullptr; 
}
