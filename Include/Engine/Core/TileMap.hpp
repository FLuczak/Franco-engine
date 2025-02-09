#pragma once
#include "Engine/Core/Component.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/VertexArray.hpp"

class TileMap : public Component, public sf::Drawable, public sf::Transformable
{
public:
	bool Load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

