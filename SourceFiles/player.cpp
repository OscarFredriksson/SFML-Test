#include "player.h"
#include <iostream>
#include <cmath>

Player::Player(World& world):
    world(world)
{
    texture.loadFromFile("Textures/Mario.png", sf::IntRect(0, 0, 48, 32));

    sprite.setTexture(texture);

    sprite.setPosition(sf::Vector2f(positionX, positionY));
}

void Player::jump()
{
    auto now = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_on_ground = now - landed_time;

    if(atGround && time_on_ground.count() > 0.1)
    {
        velocityY = jumpSpeed;
        atGround = false;
    }
}

void Player::endJump()
{
    if(velocityY < jumpSpeed/2) velocityY = jumpSpeed/2;
}

void Player::moveLeft()
{
    setDirection(Left);

    velocityX -= moveSpeed;
    if(velocityX < -maxSpeed) velocityX = -maxSpeed;
}

void Player::moveRight()
{
    setDirection(Right);

    velocityX += moveSpeed;
    if(velocityX > maxSpeed) velocityX = maxSpeed;
}

void Player::setDirection(Direction new_dir)
{
    if(!sprite.isStarted()) sprite.start();


    if(dir != new_dir)
    {
        dir = new_dir;
        sprite.flip();
    }
}

void Player::updatePosition()
{
    velocityY += gravity;
    positionY += velocityY;
    checkForGround();
    checkForRoof();

    velocityX *= stopSpeed;  //Sänk hastigheten för att tillslut stanna
    positionX += velocityX; 
    checkForWall();

    if(fabsf(velocityX) < 0.05f)  sprite.stop();

    sprite.update(fabsf(velocityX));
    setPosition(positionX, positionY);
}

void Player::checkForGround()
{
    bool foundGround = false;

    if( world.map[bottom()][left() ] != nullptr ||    //Kolla vänster hörn
        world.map[bottom()][right()] != nullptr)      //Kolla höger hörn
    {
        positionY = top();
        foundGround = true;
        velocityY = 0;

        if(!atGround)
        {
            atGround = true;
            landed_time = std::chrono::high_resolution_clock::now();
        }
    }
    if(!foundGround)    atGround = false;
}

void Player::checkForRoof()
{
    if( world.map[top()][left() ] != nullptr ||    //Kolla vänster hörn
        world.map[top()][right()] != nullptr)      //Kolla höger hörn
    {
        positionY = top() + 1;
        velocityY = -velocityY;
    }
}

void Player::checkForWall()
{
    //Kolla vänster
    if( world.map[top()   ][left()] != nullptr ||    
        world.map[top()+1 ][left()] != nullptr ||
        world.map[bottom()][left()] != nullptr )
    {
        positionX = left() + 1;
        velocityX = 0;
    }
    //Kollar höger
    if( world.map[top()   ][right()] != nullptr ||    
        world.map[top()+1 ][right()] != nullptr ||
        world.map[bottom()][right()] != nullptr )
    {
        positionX = right() - 1;
        velocityX = 0;
    }
}

void Player::setPosition(float x, float y)
{
    sprite.setPosition(convertCoords(positionX), convertCoords(positionY));
}
