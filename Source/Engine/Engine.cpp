#include "Engine/Engine.hpp"

#include <fstream>
#include <iostream>

#include "Engine/EditorUtility.h"

class Engine Engine;

Engine::Engine() :Inspectable() , window(sf::VideoMode(1280, 720), "Game")
{
    game = std::make_unique<Game>();
    inspector.RegisterInspectable(*this);
}

void Engine::Inspect()
{
    ImGui::Begin("Engine");
    if (ImGui::Button("Play"))
    {
        game->Start();
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

    ImGui::End();
}
