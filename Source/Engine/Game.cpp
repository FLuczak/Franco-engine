
#include <fstream>

#include "imgui.h"
#include "imgui-SFML.h"

#include "Engine/EngineGame.hpp"
#include "Engine/Inspector.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Game");
	Inspector inspector;

    sf::Clock clock;
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    Game game = Game(window);
    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        sf::Event event{};
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear();

    	ImGui::Begin("Engine");
        if (ImGui::Button("Play"))
        {
            game.Start();
        }

        if (ImGui::Button("Save"))
        {
            auto json = game.GetWorld().Serialize();
            std::ofstream outputFile("output.json");

            if (outputFile.is_open()) {
                // Write the JSON data to the file
                outputFile << std::setw(json.size()) << json << std::endl;

                // Close the file stream
                outputFile.close();
                std::cout << "JSON data has been written to 'output.json'." << std::endl;
            }
            else 
            {
                std::cerr << "Error opening the file." << std::endl;
                return 1; 
            }
        }

        if (ImGui::Button("Load"))
        {
            std::ifstream inputFile("output.json");

            if (inputFile.is_open()) 
            {
                nlohmann::json jsonData;
                inputFile >> jsonData;

                game.GetWorld().Deserialize(jsonData);

                inputFile.close();
                std::cout << "JSON data has been loaded and deserialized." << std::endl;
            }
            else 
            {
                std::cerr << "Error opening the file." << std::endl;
            }
        }

        ImGui::End();

        game.Tick(elapsed.asSeconds());
    	game.Render();
        inspector.InspectAll();
        ImGui::SFML::Render(window);
    	window.display();
    }

    game.End();
    ImGui::SFML::Shutdown();
	return 0;
}

