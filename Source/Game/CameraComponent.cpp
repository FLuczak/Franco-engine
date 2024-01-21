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
		element.setPosition(GetTransform().position + element.getPosition());
		myWindow->draw(element);
	}

	drawCalls.clear();
}

void CameraComponent::OnDestroy()
{
	Component::OnDestroy();
}

void CameraComponent::RegisterDrawCall(sf::Sprite& sprite)
{
	drawCalls.push_back(sprite);
}

CameraComponent::~CameraComponent()
{
	if (main == this)
	{
		main = nullptr;
	}
}
