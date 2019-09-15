#ifndef TILESET_HPP
#define TILESET_HPP

#include <SFML/Graphics/Texture.hpp>

class Tileset
{
public:
    bool loadTexture(const std::string& filename);

    const sf::Texture& getTexture() const;

    void setTileSize(const sf::Vector2u& size);
    const sf::Vector2u& getTileSize() const;

private:
    sf::Vector2u m_tileSize;
    sf::Texture m_texture;
};

#endif
