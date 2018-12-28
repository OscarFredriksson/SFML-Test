#ifndef SPRITE_H
#define SPRITE_H

#include <SFML/Graphics.hpp>


class Sprite: public sf::Sprite
{
public:
    Sprite() = default;

    int getTextureSize() const;

    void flip();

    void setPosition(float x, float y);

    void setTexture(sf::Texture _texture);

protected:
    sf::Texture texture;    //Behöver nås av grundklassen sf::Sprite

private:
    const int textureSize = 16;

    float convertCoords(const float c) const;
};

#endif