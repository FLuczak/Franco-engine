#pragma once

#include <list>
#include <memory>
#include <queue>
#include <string>

#include "Engine/Entity.hpp"
#include "Engine/EntityInspector.hpp"
#include "Engine/Inspectable.hpp"
#include "nlohmann/json.hpp"

class World : public Inspectable 
{
public:
	World();
	unsigned int GetId();
	Entity& Instantiate(std::string nameToSet);
	Entity& InstantiateTemplate(std::string path);

	void Start();
	void DeleteDestroyedEntities();

	void Tick(float deltaTime);
	void TickInEditor();

	void Destroy(Entity& entity);
	void Inspect() override;

	template<typename ComponentType>
	std::vector<std::reference_wrapper<Entity>> GetEntitiesOfType();
	std::vector<std::reference_wrapper<Entity>> GetEntitiesWithTag(const Tag& tag);

	nlohmann::json Serialize();
	void Deserialize(nlohmann::json& json);
	void DestroyAll();

private:
	EntityInspector entityInspector{};
	std::queue<unsigned int> entitiesToRemove{};
	int nextId = 0;
	std::list<Entity> entities{};
	std::vector<entity_id> idsToSet{};
};

template <typename ComponentType>
std::vector<std::reference_wrapper<Entity>> World::GetEntitiesOfType()
{
	std::vector<std::reference_wrapper<Entity>> toReturn;
	for (auto& element : entities)
	{
		if(element.GetComponent<ComponentType>() == nullptr)continue;
		toReturn.emplace_back(element);
	}

	return toReturn;
}

