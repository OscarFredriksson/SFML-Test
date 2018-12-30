#include "game.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>

Game::Game(const int width, const int height, const std::string title):
    window(sf::VideoMode(width, height), title),
    player(world)
{
    window.setFramerateLimit(60);

    enemies.emplace_back(std::make_unique<Enemy>(world));

    world.loadMap("map.txt");
}

void Game::run()
{
    sf::View view;
    const double viewZoom = 0.5;
    view.setSize(window.getSize().x * viewZoom, window.getSize().y * viewZoom);
    window.setView(view);

    while (window.isOpen())
    {
        handleInputs(window);

        updateObjects(window, view);

        if(gameOver)    
        {
            displayGameOver(window);
            return;
        }

        drawObjects(window);
    }

}

void Game::displayGameOver(sf::RenderWindow& window)
{
    sf::Font font;
    if(!font.loadFromFile("Fonts/SuperMario.ttf"))
        std::cout << "Failed to open font" << std::endl;
    
    sf::Text text("GAME OVER", font);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);

    sf::View view;
    view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(text.getPosition().x + text.getGlobalBounds().width/2, text.getPosition().y + text.getGlobalBounds().height/2);

    window.setView(view);

    window.clear(sf::Color::Black);
    window.draw(text);
    window.display();

    sf::sleep(sf::seconds(2));
}

void Game::handleInputs(sf::RenderWindow& window)
{
    //-------Handle window events-------
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    //--------Handle keypress------
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    player.jump();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        player.endWalk();
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
        player.move(Player::Right);
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  
        player.move(Player::Left);

    //----------Handle keyrelease---------
    if( event.type == sf::Event::KeyReleased)
    {
        if(event.key.code == sf::Keyboard::Up)  
            player.endJump();
        if(event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
            player.endWalk();
    }
}

void Game::updateObjects(sf::RenderWindow& window, sf::View& view)
{
    player.updatePosition();

    if(!player.isAlive())
    {
        gameOver = true;
    }
        

    std::for_each(enemies.begin(), enemies.end(), [](std::unique_ptr<Enemy>& e)
    {
        e->updatePosition();
    });
    
    view.setCenter(sf::Vector2f(player.getSprite().getPosition().x, 175));
    
    if(view.getCenter().x < view.getSize().x/2)
        view.setCenter(sf::Vector2f(view.getSize().x/2, 175));
    
    window.setView(view);
}

void Game::drawObjects(sf::RenderWindow& window)
{
    window.clear(sf::Color::Cyan);

    world.draw(window);
    player.draw(window);

    std::for_each(enemies.begin(), enemies.end(), [&window](std::unique_ptr<Enemy>& e)
    {
        e->draw(window);
    });

    window.display();
}