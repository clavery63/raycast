#ifndef __Raycast__Game__
#define __Raycast__Game__

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Player.hpp"

class Game
{
public:
    Game();
    void run();
    
private:
    void processEvents();
    void update();
    void render(float frames);
    void handleKeyboard();
    void handleCollisions();
    void handleInput(sf::Keyboard::Key key);
    void prepareImage();
    void drawImage();
    void drawFramesPerSec(float frames);
    void drawWalls();
    
    uint8_t slivers[224][2688];
    std::vector<sf::Texture> textures;
    
private:
    static const sf::Time TimePerFrame;
    static const int windowHeight;
    static const int windowWidth;
    
    sf::RenderWindow mWindow;
    sf::Image image;
    
    std::unique_ptr<Player> player;
};

#endif /* defined(__Raycast__Game__) */
