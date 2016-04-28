#include "Game.hpp"
#include "ResourcePath.hpp"
#include <vector>
#include <math.h>

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

int level[10][10] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1},
    {1,0,1,0,0,0,1,0,0,1},
    {1,0,1,0,0,0,1,0,0,1},
    {1,0,1,0,0,0,1,0,0,1},
    {1,0,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

Game::Game()
: mWindow(sf::VideoMode(1024, 576, sf::VideoMode::getDesktopMode().bitsPerPixel), "Ray Cast")
, player(new Player())
{
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    mWindow.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    if (!image.loadFromFile(resourcePath() + "cute_image.jpg")) {
        return EXIT_FAILURE;
    }
    
    prepareImage();
    
    mWindow.clear(sf::Color(0, 0, 0, 255));
    mWindow.display();
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            
            processEvents();
            update();
        }
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                mWindow.close();
                break;
            default:
                break;
        }
    }
    handleKeyboard();
}

void Game::update()
{
    handleCollisions();
}

void Game::render()
{
    mWindow.clear(sf::Color(0, 0, 0, 255));
    drawWalls();
    drawPlayerStats();
//    drawImage();
    mWindow.display();
}

void Game::handleKeyboard()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        mWindow.close();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        player->turnLeft();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        player->turnRight();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        player->moveForward();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        player->moveBack();
}

void Game::handleCollisions() {
    int x = (int) player->getX() / 100;
    int y = (int) player->getY() / 100;
    int minX = (int) player->min(player->getX()) / 100;
    int minY = (int) player->min(player->getY()) / 100;
    int maxX = (int) player->max(player->getX()) / 100;
    int maxY = (int) player->max(player->getY()) / 100;
    
    if (level[minY][minX] == 1 || level[minY][maxX] == 1 || level[maxY][minX] == 1 || level[maxY][maxX] == 1) {
        int oldMinX = (int) player->min(player->getOldX()) / 100;
        int oldMaxX = (int) player->max(player->getOldX()) / 100;
        int oldMinY = (int) player->min(player->getOldY()) / 100;
        int oldMaxY = (int) player->max(player->getOldY()) / 100;
    
        if (level[minY][oldMinX] == 0 && level[maxY][oldMinX] == 0 && level[minY][oldMaxX] == 0 && level[maxY][oldMaxX] == 0) {
            player->setX(player->getOldX());
        } else if (level[oldMinY][minX] == 0 && level[oldMinY][maxX] == 0 && level[oldMaxY][minX] == 0 && level[oldMaxY][maxX] == 0) {
            player->setY(player->getOldY());
        } else {
            player->setX(player->getOldX());
            player->setY(player->getOldY());
        }
    }
}

void Game::drawPlayerStats() {
    sf::Font font;
    font.loadFromFile(resourcePath() + "sansation.ttf");
    
    sf::Text text("Player Stats:", font);
    text.setCharacterSize(30);
    text.setColor(sf::Color::White);
    mWindow.draw(text);
    
    text.setString("player x: " + std::to_string(player->getX()));
    text.setPosition(0, 40);
    mWindow.draw(text);
    
    text.setString("player y: " + std::to_string(player->getY()));
    text.setPosition(0, 80);
    mWindow.draw(text);
    
    text.setString("player rotation: " + std::to_string(player->getRotation()));
    text.setPosition(0, 120);
    mWindow.draw(text);
}

void Game::drawWalls() {
    sf::Vector2u windowSize = mWindow.getSize();
    int numRects = 600;
    double rectWidth = (double) windowSize.x / numRects;
    
    for (int i = 0; i < numRects; i++) {
        double currentX = player->getX();
        double currentY = player->getY();
        double deltaX = sin(player->getRotation() + ((float) i - numRects / 2) / 224);
        double deltaY = cos(player->getRotation() + ((float) i - numRects / 2) / 224);
        int arrayX = (int) currentX / 100;
        int arrayY = (int) currentY / 100;
        
        double dist = 0.0;
        int j = 0;
        
        while (level[arrayY][arrayX] == 0) {
            dist += 1;
            currentX += deltaX;
            currentY += deltaY;
            arrayX = (int) currentX / 100;
            arrayY = (int) currentY / 100;
        }
        
        double rectHeight = 400000 / dist;
        
        int sliverIndex = 0;
        
        if ((int) (currentX + 1) % 100 < 2)
            sliverIndex = ((int) currentY % 100) * (224 / 100);
        else if ((int) (currentY + 1) % 100 < 2)
            sliverIndex = ((int) currentX % 100) * (224 / 100);
        else {}
        
        
        sf::Texture texture;
        texture.create(1, 600);
        texture.update(slivers[sliverIndex]);
        
        sf::RectangleShape rect(sf::Vector2f(rectWidth, rectHeight));
        rect.setPosition(i * rectWidth, 400 - rectHeight / 2);
        rect.setTexture(&texture);
        mWindow.draw(rect);
    }
}

void Game::prepareImage() {
    sf::Vector2u size = image.getSize();
    std::cout << size.x;
    int height = size.y;
    
    for (int i = 0; i < size.x; i++)
    {
        for (int j = 0; j < height; j++)
        {
            slivers[i][4 * j] = image.getPixel(i, j).r;
            slivers[i][4 * j + 1] = image.getPixel(i, j).g;
            slivers[i][4 * j + 2] = image.getPixel(i, j).b;
            slivers[i][4 * j + 3] = image.getPixel(i, j).a;
        }
    }
}



























