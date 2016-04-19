//
//  Player.cpp
//  Ray Cast
//
//  Created by Lavery, Chris on 4/14/16.
//  Copyright © 2016 Chris Lavery. All rights reserved.
//

#include <math.h>
#include "Player.hpp"

void Player::turnLeft(){
    rotation -= ROTATION_SPEED;
    if (rotation < 0)
        rotation += TWO_PI;
    if (rotation > TWO_PI)
        rotation -= TWO_PI;
};

void Player::turnRight() {
    rotation += ROTATION_SPEED;
    if (rotation < 0)
        rotation += TWO_PI;
    if (rotation > TWO_PI)
        rotation -= TWO_PI;
};

void Player::moveForward() {
    oldX = x;
    oldY = y;
    x += PLAYER_SPEED * sin(rotation);
    y += PLAYER_SPEED * cos(rotation);
};

void Player::moveBack() {
    oldX = x;
    oldY = y;
    x -= PLAYER_SPEED * sin(rotation);
    y -= PLAYER_SPEED * cos(rotation);
};
