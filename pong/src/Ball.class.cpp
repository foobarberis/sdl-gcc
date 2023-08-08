#include "Ball.class.hpp"
#include "Pong.class.hpp"

Ball::Ball()
{
    this->_r.w = 15;
    this->_r.h = this->_r.w;
    this->_posX = (SCREEN_WIDTH - this->_r.w) / 2;
    this->_posY = (SCREEN_HEIGHT - this->_r.h) / 2;
    this->_r.x = (SCREEN_WIDTH - this->_r.w) / 2;
    this->_r.y = (SCREEN_HEIGHT - this->_r.h) / 2;
    this->_dirX = 0.25;
    this->_dirY = 0.25;
}

Ball::~Ball() { }

// https://gamedev.stackexchange.com/questions/138837/drawing-at-floating-point-position
void Ball::moveBall()
{
    this->_posX += this->_dirX;
    this->_posY += this->_dirY;
    // this->_r.x = (int)this->_posX;
    // this->_r.y = (int)this->_posY;
}
