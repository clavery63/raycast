#include "Game.hpp"
#include "ResourcePath.hpp"
#include <vector>
#include <math.h>

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

int level[20][10] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1},
    {1,0,1,0,0,0,1,0,0,1},
    {1,0,1,0,0,0,1,0,0,1},
    {1,0,1,0,0,0,1,0,0,1},
    {1,0,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,0,1,1,1,1,1,1,1},
    {1,1,0,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1},
    {1,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

Game::Game()
: mWindow()
, player(new Player())
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    mWindow.create(sf::VideoMode(1024, 768, desktop.bitsPerPixel), "SFML window", sf::Style::Close);
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    mWindow.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    if (!image.loadFromFile(resourcePath() + "wall_image.jpg")) {
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
    int frames = 0;
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
        float rate = 1.0 / elapsedTime.asSeconds();
        render(rate);
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

void Game::render(float frames)
{
    mWindow.clear(sf::Color(0, 0, 0, 255));
    drawWalls();
    drawFramesPerSec(frames);
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

void Game::drawFramesPerSec(float frames) {
    sf::Font font;
    font.loadFromFile(resourcePath() + "sansation.ttf");
    
    sf::Color color = sf::Color::White;

    if (frames < 30) { color = sf::Color::Red; }

    sf::Text text("STATS", font);
    text.setCharacterSize(30);
    text.setColor(color);
    mWindow.draw(text);
    
    text.setString("frames/sec: " + std::to_string(frames));
    text.setPosition(0, 40);
    mWindow.draw(text);
}

void Game::drawWalls() {
    sf::Vector2u windowSize = mWindow.getSize();
    int numRects = 600;
    double rectWidth = (double) windowSize.x / numRects;
    
    for (int i = 0; i < numRects; i++) {
        double currentX = player->getX();
        double currentY = player->getY();
        double deltaX = sin(player->getRotation() + ((float) i - numRects / 2) / 1200);
        double deltaY = cos(player->getRotation() + ((float) i - numRects / 2) / 1200);
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
        
        double rectHeight = mWindow.getSize().y * 350 / dist;
        
        int sliverIndex = 0;
        
        if ((int) (currentX + 1) % 100 < 2)
            sliverIndex = ((int) currentY % 100) * (224 / 100);
        else if ((int) (currentY + 1) % 100 < 2)
            sliverIndex = ((int) currentX % 100) * (224 / 100);
        else {}

        sf::RectangleShape rect(sf::Vector2f(rectWidth, rectHeight));
        rect.setPosition(i * rectWidth, (mWindow.getSize().y - rectHeight) / 2);
        rect.setTexture(&textures[sliverIndex]);
        mWindow.draw(rect);
    }
}

void Game::prepareImage() {
    sf::Vector2u size = image.getSize();
    int height = size.y;
    int width = 1;
    
    for (int i = 0; i < size.x / width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            for (int ii = 0; ii < width; ii++) {
                slivers[i][4 * (width * j + ii)] = image.getPixel(i * width, j).r;
                slivers[i][4 * (width * j + ii) + 1] = image.getPixel(i * width, j).g;
                slivers[i][4 * (width * j + ii) + 2] = image.getPixel(i * width, j).b;
                slivers[i][4 * (width * j + ii) + 3] = image.getPixel(i * width, j).a;
            }
        }
    }

    for (int i = 0; i < size.x / width; i++)
    {
        sf::Texture texture;
        texture.create(1, 224);
        texture.update(slivers[i]);
        textures.push_back(texture);
    }
}



























