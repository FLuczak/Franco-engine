#pragma once
#include <iostream>

#include "Application.hpp"
#include "Game/World.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class Game final : public Application
{
public:
	explicit Game(sf::RenderWindow& window);

	void Tick(float dt) override;
	void Render() override;
	void Inspect() override;
	void Start() override;
	void End() override;

	World& GetWorld() const { return world; }
	static Game& GetCurrentGame() { return *currentGame; }
	sf::RenderWindow& gameWindow;
private:
	bool started = false;
	static inline Game* currentGame = nullptr;
	World& world;
};
