
#include <fstream>
#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"

#include "Engine/Core/EngineGame.hpp"
#include "Engine/Editor/Inspector.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "Engine/Core/Engine.hpp"
#include "IconsFontAwesome.h"

int main()
{
    sf::RenderWindow& window = Engine.window;sf::Clock clock;
    ImGui::SFML::Init(window);
    auto& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    float baseFontSize = 13.0f; // 13.0f is the size of the default font. Change to the font size you use.
    float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = iconFontSize;
    io.Fonts->AddFontFromFileTTF((std::filesystem::current_path() / "Assets" / "fa-solid-900.ttf").string().c_str(), iconFontSize, &icons_config, icons_ranges);
	ImGui::SFML::UpdateFontTexture();

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
        window.setTitle(std::string("Engine") + std::to_string(elapsed.asMilliseconds()) + std::string("ms"));
    	game.Render();
        inspector.InspectAll();
        ImGui::SFML::Render(window);
    	window.display();
    }

    game.End();
    ImGui::SFML::Shutdown();
	return 0;
}

