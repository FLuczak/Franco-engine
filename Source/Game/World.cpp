#include "Game/World.hpp"
#include "Engine/Entity.hpp"

std::unique_ptr<World>  World::instance = nullptr;

World::World() : Inspectable()
{

}

World& World::GetInstance()
{
    if (instance == nullptr)
    {
        instance = std::make_unique<World>();
    }

    return *instance;
}

unsigned World::GetId()
{
    if(idsToSet.empty())
    {
        return nextId++;
    }

    std::sort(idsToSet.begin(),idsToSet.end());
    return idsToSet[0];
}

Entity& World::Instantiate(std::string nameToSet)
{
    entities.emplace_back(GetId(), nameToSet);
    return entities.back();
}

void World::Start()
{
    for (auto& entity : entities)
    {
        for (const auto& component : entity.components)
        {
            component->Start();
        }
    }
}

void World::DeleteDestroyedEntities()
{
	int removed = 0;

	while (!entitiesToRemove.empty())
	{
		entities.remove_if([this](Entity& entity)
		{
			if( entity.GetId() == entitiesToRemove.front())
			{
                entity.OnDestroy();
                return true;
			}
            return false;
		});

		entitiesToRemove.pop();
		removed++;
	}
}

void World::Tick(float deltaTime)
{
    DeleteDestroyedEntities();

    for (auto& element : entities)
    {
        element.Update(deltaTime, true);
    }
}

void World::TickInEditor()
{
    DeleteDestroyedEntities();

    for (auto& element : entities)
    {
        element.Update(0, false);
    }
}

void World::Destroy(Entity& entity)
{
    entitiesToRemove.push(entity.GetId());
}

void World::Inspect()
{
    ImGui::Begin("Hierarchy");

    if (ImGui::Button("+ ##entity", ImVec2(40, 40)))
    {
        Instantiate("Entity");
    }

    for (auto& element : entities)
    {
        if(ImGui::Selectable((element.name + "##"+ std::to_string(element.GetId())).c_str(), false, 0, ImVec2(100, 20)))
        {
            entityInspector.InspectEntity(&element);
        }

        ImGui::SameLine();
        if(ImGui::Button(std::string("-##"+std::to_string(element.GetId())).c_str()))
        {
            Destroy(element);
            entityInspector.InspectEntity(nullptr);
        }
    }

    entityInspector.Update();

	ImGui::End();
}

nlohmann::json World::Serialize()
{
    nlohmann::json toReturn = nlohmann::json();
    toReturn["Entities"] = {};
	for (auto& entity : entities)
	{
        toReturn["Entities"].push_back(entity.Serialize());
	}
    return toReturn;
}

void World::Deserialize(nlohmann::json& json)
{
    entities.clear();

	for (auto& element : json["Entities"])
	{
       auto& temp = Instantiate("entity");
       temp.Deserialize(element);
	}
}
