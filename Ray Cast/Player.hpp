//
//  Player.hpp
//  Ray Cast
//
//  Created by Lavery, Chris on 4/14/16.
//  Copyright © 2016 Chris Lavery. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <SFML/Graphics.hpp>
#include <memory>

class Player : private sf::NonCopyable
{
public:
    double getX() { return x; }
    double getY() { return y; }
    double getOldX() { return oldX; }
    double getOldY() { return oldY; }
    double getRotation() { return rotation; }
    
    void turnLeft();
    void turnRight();
    void moveForward();
    void moveBack();
    
    void setX(double x) { this->x = x; }
    void setY(double y) { this->y = y; }
    
    double min(double val) { return val - 30; }
    double max(double val) { return val + 30; }
    
private:
    double x = 500.0;
    double y = 500.0;
    double oldX = 500.0;
    double oldY = 500.0;
    double rotation;
    
    constexpr static const double TWO_PI = 2 * 3.14159;
    constexpr static const double PLAYER_SPEED = 14.0;
    constexpr static const double ROTATION_SPEED = 0.04;
    
    double collisionResult(double pos);
};

#endif /* Player_hpp */
