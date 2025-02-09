#pragma once
#include "Engine/Core/Component.hpp"

class DoorComponent;

class RoomComponent : public Component
{
public:
	explicit RoomComponent(Entity& entityToSet): Component(entityToSet)
	{
		numberEnemies = rand() % 5;
	}

	void EnteredRoom(DoorComponent& door);
	void AddDoor(Entity& doorEntity);
	void Start() override;
	void Update(float deltaTime) override;
	int id = 0;

private:
	int numberEnemies = 0;
	int enemiesAlive = 0;
	std::vector<std::reference_wrapper<Entity>> doorEntities{};
	bool completed = false;
};
REGISTER_COMPONENT(RoomComponent);
