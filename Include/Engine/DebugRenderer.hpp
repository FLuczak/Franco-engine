#pragma once
#include <string>

#include "Game/CameraComponent.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/System/Vector2.hpp"

struct DebugCategory
{
    enum Enum
    {
        General = 1 << 0,
        Gameplay = 1 << 1,
        Physics = 1 << 2,
        Sound = 1 << 3,
        Rendering = 1 << 4,
        AINavigation = 1 << 5,
        AIDecision = 1 << 6,
        Editor = 1 << 7,
        AccelStructs = 1 << 8,
        All = 0xFFFFFFFF
    };
};

class DebugRenderer
{
public:
    DebugRenderer();
    ~DebugRenderer();

    void GetMainCamera();
    void AddLine(DebugCategory::Enum category, const sf::Vector2f& from, const sf::Vector2f& to, const sf::Color& color);

    void AddText(DebugCategory::Enum category, const sf::Vector2f& position, const std::string& text, const sf::Color& color);

    void AddCircle(DebugCategory::Enum category, const sf::Vector2f& center, float radius, const sf::Color& color);

    void AddSquare(DebugCategory::Enum category, const sf::Vector2f& center, float size, const sf::Color& color);

    void SetCategoryFlags(unsigned int flags) { categoryFlags = flags; }
    unsigned int GetCategoryFlags() const { return categoryFlags; }

private:
    sf::Font debugFont;
    sf::RenderWindow& window;
    CameraComponent* mainCam = nullptr;
    unsigned int categoryFlags;
};

