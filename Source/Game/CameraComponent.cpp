#include "Game/CameraComponent.hpp"

#include "Engine/EngineGame.hpp"
#include "Engine/Entity.hpp"

void CameraComponent::Start()
{
	Component::Start();
	myWindow = &Game::GetCurrentGame().gameWindow;
	if(main == nullptr)
	{
		main = this;
	}
}

void CameraComponent::Update()
{
	Component::Update();
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
	if(main == this)
	{
		main = nullptr;
	}
	Component::OnDestroy();
}

void CameraComponent::RegisterDrawCall(sf::Sprite& sprite)
{
	drawCalls.push_back(sprite);
}

CameraComponent::~CameraComponent()
{
}
