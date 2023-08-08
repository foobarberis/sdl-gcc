#ifndef PADDLE_CLASS_HPP
#define PADDLE_CLASS_HPP

#include <SDL.h>

class Paddle {
public:
    Paddle();
    virtual ~Paddle();

    SDL_Rect const * getRect() const;
    int getWidth() const;
    int getHeight() const;
    float getX() const;
    float getY() const;
    float getDirX() const;
    float getDirY() const;

    void setX(float x);
    void setY(float y);
    void setDirX(float f);
    void setDirY(float f);

    void movePaddle(int dir);
	void updateRect();

protected:
    SDL_Rect _r;
	float _posX;
	float _posY;
    float _dirX;
    float _dirY;
};

#endif
