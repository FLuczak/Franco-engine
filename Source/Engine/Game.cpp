
#include <fstream>
#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"

#include "Engine/EngineGame.hpp"
#include "Engine/Inspector.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "Engine/Engine.hpp"

int main()
{
    sf::RenderWindow& window = Engine.window;sf::Clock clock;
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    Inspector& inspector = Engine.inspector;
    Game& game = *Engine.game;

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

