#include "Game/SpriteRenderComponent.hpp"

#include "Engine/Entity.hpp"
#include "Game/CameraComponent.hpp"

SpriteRenderComponent::SpriteRenderComponent(Entity& entityToSet): Component(entityToSet) 
{
	tickInEditor = true;
}

void SpriteRenderComponent::Start()
{
	Component::Start();
	sprite = AssetManager::GetTexture(spritePath, spriteSize);
	oldSpritePath = spritePath;
}

void SpriteRenderComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);

	if(oldSpritePath != spritePath)
	{
		sprite = AssetManager::GetTexture(spritePath, spriteSize);
		oldSpritePath = spritePath;
	}

	if(oldSpriteSize != spriteSize)
	{
		sprite = AssetManager::GetTexture(spritePath, spriteSize);
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
