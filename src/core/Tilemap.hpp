#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Tileset;

class Tilemap: public sf::Drawable, public sf::Transformable
{
public:
    Tilemap();

    bool load(const Tileset& tileset, const uint32_t* tiles, size_t width, size_t height);

    bool collides(const sf::FloatRect& rect, sf::Sprite& sprite) const;

    bool containsPoint(const sf::Vector2f& point) const;

    sf::Vector2i convertPixelsToCoords(const sf::Vector2f& point) const;

    sf::Sprite getSprite(const sf::Vector2i& coords) const;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    size_t m_width;
    size_t m_height;
    sf::VertexArray m_vertices;
    const Tileset* m_tileset;
    std::vector<uint32_t> m_tiles;
};

#endif // TILEMAP_HPP
