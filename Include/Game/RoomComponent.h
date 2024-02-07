#pragma once
#include "Engine/Component.hpp"

class DoorComponent;

class RoomComponent : public Component
{
public:
	explicit RoomComponent(Entity& entityToSet): Component(entityToSet)
	{
	}

	void EnteredRoom(DoorComponent& door);
	void AddDoor(Entity& doorEntity);
	void Start() override;
	void Update(float deltaTime) override;
	int id = 0;

private:
	std::vector<std::reference_wrapper<Entity>> doorEntities{};
};
REGISTER_COMPONENT(RoomComponent);
