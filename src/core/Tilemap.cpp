#include "Tilemap.hpp"
#include "Tileset.hpp"
#include <cmath>
#include <iostream>


Tilemap::Tilemap():
    m_width(0),
    m_height(0),
    m_tileset(nullptr)
{
}

bool Tilemap::load(const Tileset& tileset, const uint32_t* tiles, size_t width, size_t height)
{
    const sf::Vector2u& tileSize = tileset.getTileSize();

    // Resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    // Populate the vertex array, with one quad per tile
    for (size_t i = 0; i < width; ++i) {
        for (size_t j = 0; j < height; ++j) {
            // Get the current tile number
            int tileNumber = tiles[i + j * width];

            if (tileNumber > 0) {
                // Find its position in the tileset texture
                int tu = tileNumber % (tileset.getTexture().getSize().x / tileSize.x);
                int tv = tileNumber / (tileset.getTexture().getSize().x / tileSize.x);

                // Get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // Define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // Define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }
        }
    }
    m_tileset = &tileset;

    // Copy the tiles
    const size_t tile_count = width * height;
    m_tiles.reserve(tile_count);
    for (size_t i = 0; i < tile_count; ++i)
    {
        m_tiles[i] = tiles[i];
    }
    m_width = width;
    m_height = height;
    return true;
}


bool Tilemap::collides(const sf::FloatRect& rect, sf::Sprite& sprite) const
{
    const sf::Vector2f points[] = {
        {rect.left,              rect.top},
        {rect.left + rect.width, rect.top},
        {rect.left + rect.width, rect.top + rect.height},
        {rect.left,              rect.top + rect.height}
    };
    for (int i = 0; i < 4; ++i) {
        if (containsPoint(points[i])) {
            sf::Vector2i coords = convertPixelsToCoords(points[i]);
            sprite = getSprite(coords);
            return true;
        }
    }
    return false;
}

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_tileset->getTexture();
    target.draw(m_vertices, states);
}

sf::Vector2i Tilemap::convertPixelsToCoords(const sf::Vector2f& pos) const
{
    const sf::Vector2u& tileSize = {16, 16};
    return sf::Vector2i(
        std::floor(pos.x / tileSize.x),
        std::floor(pos.y / tileSize.y)
    );
}

bool Tilemap::containsPoint(const sf::Vector2f& point) const
{
    sf::Vector2i pos = convertPixelsToCoords(point);
    if (pos.x >= 0 && pos.x < m_width && pos.y >= 0 && pos.y < m_height)
    {
        int tileIndex = pos.x + pos.y * m_width;
        if (m_tiles[tileIndex] != 0)
        {
            std::cout << "point x:" << (int) point.x << ", y:" << (int) point.y << " is colliding\n";
            std::cout << "    index: ["<< pos.x << "][" << pos.y << "]\n";
            return true;
        }
    }
    return false;
}


sf::Sprite Tilemap::getSprite(const sf::Vector2i& coords) const
{
    sf::Sprite sprite;
    sprite.setTexture(m_tileset->getTexture());

    int tileNumber = m_tiles[coords.x + coords.y * m_width];

    const sf::Vector2u& tileSize = m_tileset->getTileSize();
    sprite.setPosition(sf::Vector2f(
        coords.x * tileSize.x, coords.y * tileSize.y
    ));

    int tu = tileNumber % (m_tileset->getTexture().getSize().x / tileSize.x);
    int tv = tileNumber / (m_tileset->getTexture().getSize().x / tileSize.x);



    sprite.setTextureRect(sf::IntRect(
        tu * tileSize.x, tv * tileSize.y, tileSize.x, tileSize.y
    ));
    return sprite;
}


