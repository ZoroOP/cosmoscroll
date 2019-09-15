#include "Tileset.hpp"


bool Tileset::loadTexture(const std::string& filename)
{
    return m_texture.loadFromFile(filename);
}

const sf::Texture& Tileset::getTexture() const
{
    return m_texture;
}

void Tileset::setTileSize(const sf::Vector2u& size)
{
    m_tileSize = size;
}

const sf::Vector2u& Tileset::getTileSize() const
{
    return m_tileSize;
}
