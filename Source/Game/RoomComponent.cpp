#include "Game/RoomComponent.h"

#include "Engine/Engine.hpp"
#include "Engine/Entity.hpp"
#include "Game/CameraComponent.hpp"
#include "Game/DoorComponent.h"

void RoomComponent::EnteredRoom(DoorComponent& door)
{
	if (door.roomTo == id)
	{
		GetEntity().active = true;
		for (auto& doorEntity : doorEntities)
		{
			doorEntity.get().active = false;
		}
		CameraComponent::GetMainCamera()->GetTransform().position = GetTransform().position;
	}

	if(!completed)
	{
		if (id == 45)return;
		for(int i = 0; i < numberEnemies;i++)
		{
			auto& entity = Engine.world.InstantiateTemplate("Templates/Necromancer.ent");
			entity.GetTransform().position = GetTransform().position + sf::Vector2f(600, 300) + sf::Vector2f(rand() % 1000 - 500, rand() % 500 - 250); 
		}
	}
}

void RoomComponent::Start()
{
	Component::Start();
}

void RoomComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);
	Engine.debugRenderer.AddText(DebugCategory::Gameplay, GetTransform().position + sf::Vector2f(600,300), std::to_string(id), sf::Color::Red);
}

void RoomComponent::AddDoor(Entity& doorEntity)
{
	doorEntities.push_back({ doorEntity });
	doorEntity.GetComponent<DoorComponent>()->doorOpened.bind(this, &RoomComponent::EnteredRoom);
}
