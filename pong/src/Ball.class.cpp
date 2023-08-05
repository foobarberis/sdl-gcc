#include "Ball.class.hpp"
#include "Pong.class.hpp"

Ball::Ball()
{
    this->_r.w = 15;
    this->_r.h = this->_r.w;
    this->_r.x = (SCREEN_WIDTH - this->_r.w) / 2;
    this->_r.y = (SCREEN_HEIGHT - this->_r.h) / 2;
    this->_dirX = 1;
    this->_dirY = 1;
}

Ball::~Ball() { }

void Ball::moveBall()
{
    this->_r.x += this->_dirX;
    this->_r.y += this->_dirY;
}


