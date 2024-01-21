#pragma once
#include <vector>

#include "Engine/Component.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"

class CameraComponent : public Component
{
public:
	CameraComponent(Entity& entityToSet);

	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
	void RegisterDrawCall(sf::Sprite& sprite);
	static CameraComponent* GetMainCamera() { return main; }
	~CameraComponent() override;
private:
	std::vector<sf::Sprite> drawCalls{};
	sf::RenderWindow* myWindow = nullptr;
	inline static CameraComponent* main{};
};
REGISTER_COMPONENT(CameraComponent);