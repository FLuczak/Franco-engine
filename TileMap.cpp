#include "TileMap.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

bool TileMap::Load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned width, unsigned height)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
        return false;

    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}
