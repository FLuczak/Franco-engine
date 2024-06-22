#include "Game/Dungeon.h"

#include "Engine/Engine.hpp"
#include "Engine/SpriteRenderComponent.hpp"
#include "Game/DoorComponent.h"
#include "Game/RoomComponent.h"

void Dungeon::SpawnRooms()
{
	for (int i = 0; i < rooms.size(); ++i) 
	{
		int x = rooms[i] % 10;
		int y = rooms[i] / 10;

		auto& entity = Engine.world.InstantiateTemplate("Templates/Map.ent");
		entity.GetComponent<RoomComponent>()->id = y*10+x;
		roomsVector.push_back({ entity });

		if (rooms[i] != 45)
		{
			entity.active = false;
		}

		entity.GetTransform().position =sf::Vector2f(x*cellWidth - cellWidth * 5, y*cellHeight - cellHeight * 4) + sf::Vector2f(30,10);
		entity.GetComponent<SpriteRenderComponent>()->layer = -10 + y;
	}
}

bool Dungeon::GenerateRooms()
{
	if (started) 
	{
		while (!cellQueue.empty())
		{
			int i = cellQueue.front();
			cellQueue.erase(cellQueue.begin());
			int x = i % 10;
			bool created = false;
			if (x > 1) created = created | Visit(i - 1);
			if (x < 9) created = created | Visit(i + 1);
			if (i > 20) created = created | Visit(i - 10);
			if (i < 70) created = created | Visit(i + 10);

			if (!created) 
			{
				endRooms.push_back(i);
			}
		}

		if(!placedSpecial)
		{
			if (floorPlanCount < minRooms) 
			{
				int i = endRooms.back();
				switch (rand()%4)
				{
				case 0:Visit(i - 1); break;
				case 1:Visit(i + 1); break;
				case 2:Visit(i - 10); break;
				case 3:Visit(i + 10); break;
				}
				return true;
			}

			if (floorPlanCount < minRooms) 
			{
				Start();
				return true;
			}

			placedSpecial = true;
			bossID = endRooms.back();
			endRooms.pop_back();
		}
	}
	return false;
}

void Dungeon::SpawnDoors()
{
	for (auto i : rooms)
	{
		if (i != 0) { TrySpawnDoor(i -1,i); }
		if (i < 99) { TrySpawnDoor(i + 1,i); }
		if (i > 11) { TrySpawnDoor(i - 10,i); }
		if (i < 90){ TrySpawnDoor(i + 10,i); }
	}
}

void Dungeon::TrySpawnDoor(int roomTo,int roomFrom) 
{
	if (std::ranges::find(rooms,roomTo) != rooms.end())
	{
		const int toX = roomTo % 10;
		const int toY = roomTo / 10;

		const int fromX = roomFrom % 10;
		const int fromY = roomFrom / 10;

		auto& door = Engine.world.InstantiateTemplate("Templates/Door.ent");
		auto& doorComponent = *door.GetComponent<DoorComponent>();

		const auto  roomToObj = std::find_if(roomsVector.begin(), roomsVector.end(), [roomTo](std::reference_wrapper<Entity>& entity)
		{
			return entity.get().GetComponent<RoomComponent>()->id == roomTo;
		});

		const auto  roomFromObj = std::find_if(roomsVector.begin(), roomsVector.end(), [roomFrom](std::reference_wrapper<Entity>& entity)
		{
			return entity.get().GetComponent<RoomComponent>()->id == roomFrom;
		});

		roomToObj->get().GetComponent<RoomComponent>()->AddDoor(door);
		roomFromObj->get().GetComponent<RoomComponent>()->AddDoor(door);

		const auto roomToPosition = sf::Vector2f(toX * cellWidth - cellWidth * 5, toY * cellHeight - cellHeight *4) + sf::Vector2f(cellWidth/2,cellHeight/2) + sf::Vector2f(30, 10);
		const auto roomFromPosition = sf::Vector2f(fromX * cellWidth - cellWidth * 5, fromY * cellHeight - cellHeight *4) + sf::Vector2f(cellWidth / 2, cellHeight / 2) + sf::Vector2f(30, 10);
		door.GetTransform().position = (roomToPosition + roomFromPosition) / 2.0f + sf::Vector2f(-32,64);

		if(roomFrom != 45)
		{
			door.active = false;
		}

		doorComponent.doorDirection = roomToPosition - roomFromPosition;
		doorComponent.doorDirection = sf::normalize(doorComponent.doorDirection);
		doorComponent.roomTo = roomTo;
		doorComponent.roomFrom = roomFrom;
	}
}

void Dungeon::Start()
{
	Component::Start();
    std::mt19937 gen(rd());
    started = true;
    placedSpecial = false;
    rooms.clear();
    floorPlan.assign(101, 0);
    floorPlanCount = 0;
    cellQueue.clear();
    endRooms.clear();
    Visit(45);

    if (GenerateRooms()) return;
    SpawnRooms();
	SpawnDoors();
}

bool Dungeon::Visit(int i)
{
    if (floorPlan[i])return false;

    int neighbours = ncount(i);

    if (neighbours > 1)return false;

    if (floorPlanCount >= maxRooms)return false;

    if (dis(gen) < 0.5 && i != 45)return false;

    cellQueue.push_back(i);
    floorPlan[i] = 1;
    floorPlanCount += 1;

    rooms.push_back(i); // Store the image ID or index
    return true;
}

void Dungeon::Update(float deltaTime)
{
	Component::Update(deltaTime);
}
