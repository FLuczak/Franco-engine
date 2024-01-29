#include "Engine/Engine.hpp"

#include <fstream>
#include <iostream>

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

    if (ImGui::Button("Save"))
    {
        auto json = game->GetWorld().Serialize();
        std::ofstream outputFile("output.json");

        if (outputFile.is_open()) {
            // Write the JSON data to the file
            outputFile << std::setw(json.size()) << json << std::endl;

            // Close the file stream
            outputFile.close();std::cout << "JSON data has been written to 'output.json'." << std::endl;
        }
        else
        {
            std::cerr << "Error opening the file." << std::endl;
            return;
        }
    }

    if (ImGui::Button("Load"))
    {
        std::ifstream inputFile("output.json");

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
