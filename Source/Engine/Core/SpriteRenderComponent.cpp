
#include "Engine/Core/SpriteRenderComponent.hpp"

#include "Engine/Core/AssetManager.hpp"
#include "Game/CameraComponent.hpp"
#include "Engine/Core/Entity.hpp"

SpriteRenderComponent::SpriteRenderComponent(Entity& entityToSet): Component(entityToSet) 
{
	tickInEditor = true;
}

void SpriteRenderComponent::Start()
{
	Component::Start();
	sprite = AssetManager::GetTexture(spritePath.path, spriteSize);
	oldSpritePath = spritePath.path;
}

void SpriteRenderComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);

	if(oldSpritePath != spritePath.path)
	{
		sprite = AssetManager::GetTexture(spritePath.path, spriteSize);
		oldSpritePath = spritePath.path;
	}

	if(oldSpriteSize != spriteSize)
	{
		sprite = AssetManager::GetTexture(spritePath.path, spriteSize);
		oldSpriteSize = spriteSize;
	}

	if (sprite == nullptr)return;
	sprite->setPosition(GetTransform().position);
	sprite->setRotation(GetTransform().rotation);
	sprite->setScale(GetTransform().scale);


	CameraComponent* mainCamera = CameraComponent::GetMainCamera();

	if (mainCamera == nullptr)return;
	mainCamera->RegisterDrawCall(*sprite,layer);
}

void SpriteRenderComponent::OnDestroy()
{
	Component::OnDestroy();
}

SpriteRenderComponent::~SpriteRenderComponent()
{
}
