#include "Engine/DebugRenderer.hpp"
#include "Engine/DebugRenderer.hpp"

#include "Engine/Engine.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

DebugRenderer::DebugRenderer() : window(Engine.window), mainCam(CameraComponent::GetMainCamera()),categoryFlags(DebugCategory::Enum::All)
{
    if (!debugFont.loadFromFile("Assets/arial.ttf"))
    {
    }
}

DebugRenderer::~DebugRenderer()
{
}

void DebugRenderer::AddLine(DebugCategory::Enum category, const sf::Vector2f& from, const sf::Vector2f& to,const sf::Color& color)
{
    if (!(categoryFlags & category)) return;

    const sf::Vector2f fromOffset = from + mainCam->GetTransform().position;
    const sf::Vector2f toOffset = to + mainCam->GetTransform().position;

    // Create a line drawable
    sf::Vertex line[] =
    {
        sf::Vertex(fromOffset, color),
        sf::Vertex(toOffset, color)
    };

    window.draw(line, 2, sf::Lines);
}

void DebugRenderer::AddText(DebugCategory::Enum category, const sf::Vector2f& position, const std::string& text,const sf::Color& color)
{
    if (!(categoryFlags & category)) return;
    sf::Text debugText;
    debugText.setFont(debugFont); // Assuming you have a debug font set
    debugText.setString(text);
    debugText.setCharacterSize(12);
    debugText.setFillColor(color);
    debugText.setPosition(position);

    window.draw(debugText);
}

void DebugRenderer::AddCircle(DebugCategory::Enum category, const sf::Vector2f& center, float radius,const sf::Color& color)
{
    if (!(categoryFlags & category)) return;

    sf::CircleShape debugCircle(radius);
    debugCircle.setFillColor(sf::Color::Transparent);
    debugCircle.setOutlineColor(color);
    debugCircle.setOutlineThickness(1.f);
    debugCircle.setOrigin(radius, radius);
    debugCircle.setPosition(center);

    window.draw(debugCircle);
}


void DebugRenderer::AddSquare(DebugCategory::Enum category, const sf::Vector2f& center, float size,const sf::Color& color)
{
    if (!(categoryFlags & category)) return;
    if (!(categoryFlags & category)) return;

    sf::RectangleShape debugSquare(sf::Vector2f(size, size));
    debugSquare.setFillColor(sf::Color::Transparent);
    debugSquare.setOutlineColor(color);
    debugSquare.setOutlineThickness(1.f);
    debugSquare.setOrigin(size / 2.f, size / 2.f);
    debugSquare.setPosition(center);

    window.draw(debugSquare);
}
