#include "Engine/Core/Engine.hpp"

#include <fstream>
#include <iostream>

#include "Engine/Editor/EditorUtility.h"

class Engine Engine;

Engine::Engine() :Inspectable() , window(sf::VideoMode(1280, 720), "Game"), navMesh(0.75f)
{
    game = std::make_unique<Game>();
    inspector.RegisterInspectable(*this);
    debugRenderer.SetCategoryFlags(0);
}

void Engine::Inspect()
{
    ImGui::Begin("Engine");
    if(!game->IsPlaying())
    {
        if (ImGui::Button("Play"))
        {
            temporaryWorld = world.Serialize();
            game->Start();
        }
    }
	else
    {
        if (ImGui::Button("Stop"))
        {
            game->End();
            world.DestroyAll();
            world.Deserialize(temporaryWorld);
        }
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
        std::string path = Dialogs::OpenFileSaveDialog();

        if (path.empty())return;
        auto json = game->GetWorld().Serialize();
        std::ofstream outputFile(path);

        if (outputFile.is_open()) 
        {
            outputFile << std::setw(json.size()) << json << std::endl;
        	outputFile.close();std::cout << "JSON data has been written to 'output.json'." << std::endl;
        }
        else
        {
            std::cerr << "Error opening the file." << std::endl;
            return;
        }
    }

    btEditor.Update();
    fsmEditor.Update();

    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        std::string path = Dialogs::OpenFileLoadDialog(".ent");
        if (path.empty())return;
        std::ifstream inputFile(path);

        if (inputFile.is_open())
        {
            nlohmann::json jsonData;
            inputFile >> jsonData;

            game->GetWorld().Deserialize(jsonData);

            inputFile.close();
            std::cout << "JSON data has been loaded and deserialized." << std::endl;
        }
        else
        {
            std::cerr << "Error opening the file." << std::endl;
        }
    }

    navMesh.DebugDraw(debugRenderer);
    ImGui::SameLine();

    if (ImGui::BeginMenu("Debug Render",true))
    {
        auto debugRenderFlags = debugRenderer.GetCategoryFlags();
        bool changed = false;
        changed |= ImGui::CheckboxFlags("General", &debugRenderFlags, DebugCategory::General);
        changed |= ImGui::CheckboxFlags("Gameplay", &debugRenderFlags, DebugCategory::Gameplay);
        changed |= ImGui::CheckboxFlags("Physics", &debugRenderFlags, DebugCategory::Physics);
        changed |= ImGui::CheckboxFlags("AI Navigation", &debugRenderFlags, DebugCategory::AINavigation);
        changed |= ImGui::CheckboxFlags("AI Decision Making", &debugRenderFlags, DebugCategory::AIDecision);
        changed |= ImGui::CheckboxFlags("Sound", &debugRenderFlags, DebugCategory::Sound);
        changed |= ImGui::CheckboxFlags("Rendering", &debugRenderFlags, DebugCategory::Rendering);
        changed |= ImGui::CheckboxFlags("Editor", &debugRenderFlags, DebugCategory::Editor);
        changed |= ImGui::CheckboxFlags("Acceleration Struct", &debugRenderFlags, DebugCategory::AccelStructs);
        if (changed)
        {
            debugRenderer.SetCategoryFlags(debugRenderFlags);
        }
        ImGui::EndMenu();
    }


    explorer.Update(0.0f);

    ImGui::End();
}
