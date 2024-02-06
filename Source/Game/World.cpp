#include "Game/World.hpp"

#include <fstream>
#include <iostream>

#include "Engine/AssetManager.hpp"
#include "Engine/EditorUtility.h"
#include "Engine/Entity.hpp"

World::World() : Inspectable()
{
    
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

Entity& World::InstantiateTemplate(std::string path)
{
    auto name = std::string();
    entities.emplace_back(GetId(), name);
    auto& entity = entities.back();

    const auto& json = AssetManager::GetEntityTemplate(path);
    auto& nonConst = const_cast<nlohmann::json&>(json);

    entity.Deserialize(nonConst);
    entity.Start(true);
    return entity;
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
        ImGui::OpenPopup("ThePopup");
    }

    if(!ImGui::IsMouseClicked(0) || ImGui::IsAnyItemHovered())
    {
        if (ImGui::BeginPopup("ThePopup"))
        {
            if (ImGui::Button("Empty entity"))
            {
                Instantiate("Entity");
            }

            if (ImGui::Button("From template"))
            {
                std::string path = Dialogs::OpenFileLoadDialog("(*.ent)");

                if (path.empty() || path.find(".ent") == std::string::npos)
                {
                    ImGui::EndPopup();
                    ImGui::End();
                	return;
                }
                std::ifstream inputFile(path);

                if (inputFile.is_open())
                {
                    nlohmann::json jsonData;
                    inputFile >> jsonData;

                    auto& entity = Instantiate("Entity");
                    entity.Deserialize(jsonData);

                    inputFile.close();
                }
                else
                {
                    std::cerr << "Error opening the file." << std::endl;
                }
            }

            ImGui::EndPopup();
        }
    }


    for (auto& element : entities)
    {
        std::string id = std::to_string(element.GetId());
        if(ImGui::Selectable( std::string(element.name + "(" + id + ")").c_str(), false, ImGuiSelectableFlags_AllowItemOverlap, ImVec2(200, 20)))
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
