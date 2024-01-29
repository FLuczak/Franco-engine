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

	void Start();
	void DeleteDestroyedEntities();

	void Tick(float deltaTime);
	void TickInEditor();

	void Destroy(Entity& entity);
	void Inspect() override;
	nlohmann::json Serialize();
	void Deserialize(nlohmann::json& json);
private:
	EntityInspector entityInspector{};
	std::queue<unsigned int> entitiesToRemove{};
	int nextId = 0;
	std::list<Entity> entities{};
	std::vector<int> idsToSet{};
};

