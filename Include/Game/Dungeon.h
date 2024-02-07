#pragma once
#include <random>

#include "Engine/Component.hpp"
#include "Visit struct/visit_struct.hpp"
#include "Engine/EditorVariables.h"

class Dungeon : public Component
{
public:
	explicit Dungeon(Entity& entityToSet): Component(entityToSet)
	{
        minRooms = 4;
        maxRooms = 12;
        dis = std::uniform_real_distribution<>(0.0, 1.0);
        gen= std::mt19937(rd());
	}

	void SpawnRooms();
	bool GenerateRooms();

	void SpawnDoors();
    void TrySpawnDoor(int roomTo, int roomFrom);

	void Start() override;
	void Update(float deltaTime) override;
	bool Visit(int i);
    int ncount(int i)
	{
        return floorPlan[i - 10] + floorPlan[i - 1] + floorPlan[i + 1] + floorPlan[i + 10];
    }



	SERIALIZE_FIELD(int, cellWidth);
    SERIALIZE_FIELD(int, cellHeight);
    SERIALIZE_FIELD(int, maxRooms);
    SERIALIZE_FIELD(int, minRooms);
private:
    std::vector<std::reference_wrapper<Entity>> roomsVector{};

	std::random_device rd;
    std::uniform_real_distribution<> dis;
    std::mt19937 gen;

    bool started;
    bool placedSpecial;
    std::vector<int> rooms;
    std::vector<int> floorPlan;
    std::vector<int> cellQueue;
    std::vector<int> endRooms;
    int floorPlanCount;

    int bossID;
};
REGISTER_COMPONENT(Dungeon);

