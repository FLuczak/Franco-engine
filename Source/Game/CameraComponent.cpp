#include "Game/CameraComponent.hpp"

#include "Engine/EngineGame.hpp"
#include "Engine/Entity.hpp"

CameraComponent::CameraComponent(Entity& entityToSet): Component(entityToSet)
{
	if (main == nullptr)
	{
		main = this;
	}
	myWindow = &Game::GetCurrentGame().gameWindow;
	tickInEditor = true;
}

void CameraComponent::Start()
{
	Component::Start();
}

void CameraComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);
	if (myWindow == nullptr)return;
	for (auto& element : drawCalls)
	{
		for (auto call : element.second)
		{
			call .setPosition(GetTransform().position - call.getPosition());
			myWindow->draw(call);
		}
	}

	drawCalls.clear();
}

void CameraComponent::OnDestroy()
{
	Component::OnDestroy();
}

void CameraComponent::RegisterDrawCall(const sf::Sprite& sprite, int layer)
{
	drawCalls[layer].push_back(sprite);
}

CameraComponent::~CameraComponent()
{
	if (main == this)
	{
		main = nullptr;
	}
}
