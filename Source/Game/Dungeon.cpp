#include "Game/Dungeon.h"

#include "Engine/Engine.hpp"
#include "Game/SpriteRenderComponent.hpp"

void Dungeon::Start()
{
	Component::Start();
    std::mt19937 gen(rd());
    started = true;
    placedSpecial = false;
    images.clear();
    floorPlan.assign(101, 0);
    floorPlanCount = 0;
    cellQueue.clear();
    endRooms.clear();
    Visit(45);

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
                return;
            }

            if (floorPlanCount < minRooms) 
            {
                Start();
                return;
            }

            placedSpecial = true;
            bossID = endRooms.back();
            endRooms.pop_back();
        }
    }

    for (int i = 0; i < images.size(); ++i) 
    {
        int x = images[i] % 10;
        int y = images[i] / 10;
        auto& entity = Engine.world.InstantiateTemplate("Templates/Map.ent");
        entity.GetTransform().position =sf::Vector2f(x*cellWidth - cellWidth * 5, y*cellHeight - cellHeight * 5);
        entity.GetComponent<SpriteRenderComponent>()->layer = y;
    }
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

    images.push_back(i); // Store the image ID or index
    return true;
}

void Dungeon::Update(float deltaTime)
{
	Component::Update(deltaTime);
}
