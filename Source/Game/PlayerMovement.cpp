#include "Game/PlayerMovement.hpp"

#include "Engine/Entity.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/SFMLMath.hpp"

void PlayerMovement::Start()
{
	Component::Start();
}

void PlayerMovement::Update(float deltaTime)
{
	Component::Update(deltaTime);
	float vertical = sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? -1.0f : sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? 1.0f : 0.0f;
	float horizontal = sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? 1.0f : sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? -1.0f : 0.0f;

	inputs = { horizontal,vertical };

	if (horizontal == 0 && vertical == 0)return;
	inputs = sf::normalize(inputs) * moveSpeed;

	GetTransform().position += inputs*deltaTime;
}

void PlayerMovement::OnDestroy()
{
	Component::OnDestroy();
}
