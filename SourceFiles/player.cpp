#include "player.h"
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

Player::Player(World& world):
    Character(world, width, height),
    jumpSound(jumpSound_path)
{
    setMaxSpeed(maxSpeed);
    setMoveSpeed(moveSpeed);
    setJumpSpeed(jumpSpeed);

    sf::Texture texture;
    texture.loadFromFile("Textures/Mario.png", sf::IntRect(0, 0, 80, 32));

    sprite.setTexture(texture);
}

void Player::jump()
{
    if(!jumpKeyReleased)    return;
    jumpKeyReleased = false;

    if(onGround())
    {
        jumpSound.play();
        Character::jump();
    }
}

void Player::endJump()
{    
    if(getVerticalVelocity() < jumpSpeed/2) setVerticalVelocity(jumpSpeed/2);
    jumpKeyReleased = true;
}

void Player::move(Direction dir)
{
    setDirection(dir);
    Character::move(dir);
}

void Player::endWalk()
{
    horisontalButtonHeld = false;
}

void Player::updatePosition()
{
    if(!onGround())                   sprite.set(4);
    else if(horisontalButtonHeld)   sprite.update(0, 2);
    else                            sprite.update(0, 0);
    
    Character::updatePosition();
    setPosition(getPositionX(), getPositionY());

}

void Player::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::Sprite Player::getSprite() const
{
    return sprite;
}

void Player::setDirection(Direction new_dir)
{
    horisontalButtonHeld = true;
    if(getDirection() != new_dir)
    {
        sprite.flip();
        Character::setDirection(new_dir);
    }
}