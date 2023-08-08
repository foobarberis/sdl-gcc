#include "Paddle.class.hpp"
#include "Pong.class.hpp"

Paddle::Paddle()
{
    this->_r.h = 100;
    this->_r.w = 15;
    this->_r.x = 0;
    this->_r.y = (SCREEN_HEIGHT - this->_r.h) / 2;
    this->_dirX = 0;
    this->_dirY = 10; // SCREEN_HEIGHT / 70;
}

Paddle::~Paddle()
{
}

void Paddle::movePaddle(int dir)
{
    if (dir == UP) {
        if (this->_posY - this->_dirY >= 5.0)
            this->_posY -= this->_dirY;
    } else if (dir == DOWN) {
        if (this->_posY + (float)this->_r.h + this->_dirY <= SCREEN_HEIGHT - 5)
            this->_posY += this->_dirY;
    }
    // this->_r.x = (int)this->_posX;
    // this->_r.y = (int)this->_posY;
}

void Paddle::updateRect()
{
    this->_r.x = this->_posX;
    this->_r.y = this->_posY;
}

/* void Paddle::movePaddle(int dir)
{
    if (dir == UP) {
        if (this->_r.y - this->_dirY >= 5)
            this->_r.y -= this->_dirY;
    } else if (dir == DOWN) {
        if (this->_r.y + this->_r.h + this->_dirY <= SCREEN_HEIGHT - 5)
            this->_r.y += this->_dirY;
    }
} */

SDL_Rect const * Paddle::getRect() const
{
    return &(this->_r);
}

int Paddle::getWidth() const
{
    return this->_r.w;
}

int Paddle::getHeight() const
{
    return this->_r.h;
}

float Paddle::getX() const
{
    return this->_posX;
}

float Paddle::getY() const
{
    return this->_posY;
}

float Paddle::getDirX() const
{
    return this->_dirX;
}

float Paddle::getDirY() const
{
    return this->_dirY;
}

void Paddle::setDirX(float f)
{
    this->_dirX = f;
}

void Paddle::setDirY(float f)
{
    this->_dirY = f;
}

void Paddle::setX(float x)
{
    this->_posX = x;
}

void Paddle::setY(float y)
{
    this->_posY = y;
}
