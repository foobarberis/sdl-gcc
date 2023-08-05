#ifndef PADDLE_CLASS_HPP
#define PADDLE_CLASS_HPP

#include <SDL.h>

class Paddle {
public:
    Paddle();
    virtual ~Paddle();

    SDL_Rect const * getRect() const;
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    int getDirX() const;
    int getDirY() const;

    void setX(int x);
    void setY(int y);
    void setDirX(int f);
    void setDirY(int f);

    void movePaddle(int dir);

protected:
    SDL_Rect _r;
    int _dirX;
    int _dirY;
};

#endif
