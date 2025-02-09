
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
	spriteAsset = AssetManager::LoadSprite(spritePath.path.string(), spriteSize);
	oldSpritePath = spritePath.path;
}

void SpriteRenderComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);

	if(oldSpritePath != spritePath.path)
	{
		spriteAsset = AssetManager::LoadSprite(spritePath.path.string(), spriteSize);
		spriteToDraw = spriteAsset->sprite;
		oldSpritePath = spritePath.path;
	}

	if(oldSpriteSize != spriteSize)
	{
		spriteAsset = AssetManager::LoadSprite(spritePath.path.string(), spriteSize);
		spriteToDraw = spriteAsset->sprite;
		oldSpriteSize = spriteSize;
	}

	if (spriteAsset == nullptr)return;
	spriteToDraw.setPosition(GetTransform().position);
	spriteToDraw.setRotation(GetTransform().rotation);
	spriteToDraw.setScale(GetTransform().scale);


	CameraComponent* mainCamera = CameraComponent::GetMainCamera();

	if (mainCamera == nullptr)return;
	mainCamera->RegisterDrawCall(spriteToDraw,layer);
}

void SpriteRenderComponent::OnDestroy()
{
	Component::OnDestroy();
}

SpriteRenderComponent::~SpriteRenderComponent()
{
}
