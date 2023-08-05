#include "Paddle.class.hpp"
#include "Pong.class.hpp"

Paddle::Paddle()
{
    this->_r.h = 100;
    this->_r.w = 15;
    this->_r.x = 0;
    this->_r.y = (SCREEN_HEIGHT - this->_r.h) / 2;
    this->_dirX = 0;
    this->_dirY = SCREEN_HEIGHT / 70;
}

Paddle::~Paddle()
{
}

void Paddle::movePaddle(int dir)
{
    if (dir == UP) {
        if (this->_r.y - this->_dirY >= 5)
            this->_r.y -= this->_dirY;
    } else if (dir == DOWN) {
        if (this->_r.y + this->_r.h + this->_dirY <= SCREEN_HEIGHT - 5)
            this->_r.y += this->_dirY;
    }
}

SDL_Rect const * Paddle::getRect() const
{
    return &(this->_r);
}

int Paddle::getX() const
{
    return this->_r.x;
}

int Paddle::getY() const
{
    return this->_r.y;
}

int Paddle::getWidth() const
{
    return this->_r.w;
}

int Paddle::getHeight() const
{
    return this->_r.h;
}

int Paddle::getDirX() const
{
    return this->_dirX;
}

int Paddle::getDirY() const
{
    return this->_dirY;
}

void Paddle::setDirX(int f)
{
    this->_dirX = f;
}

void Paddle::setDirY(int f)
{
    this->_dirY = f;
}

void Paddle::setX(int x)
{
    this->_r.x = x;
}

void Paddle::setY(int y)
{
    this->_r.y = y;
}
