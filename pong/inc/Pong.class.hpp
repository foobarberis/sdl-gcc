#ifndef PONG_CLASS_HPP
#define PONG_CLASS_HPP

#include "Ball.class.hpp"
#include "Paddle.class.hpp"
#include <SDL.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

enum eDirections {
    UP,
    DOWN
};

class Pong {
public:
    Pong();
    Pong(SDL_Surface * Surface);
    ~Pong();

    void initGame();
    void drawGame(SDL_Window * Window, SDL_Surface * Surface);
    void processEvent(SDL_Event * Event);
    void updateGame(SDL_Event * Event);
	bool checkHitbox(Paddle const & p, Ball const & b);
	void handleCollisions();


    bool getQuit();

private:
    Ball _ball;
    Paddle _lPaddle;
    Paddle _rPaddle;
    int _lScore;
    int _rScore;
    bool _quit;
    bool _pause;
    Uint32 _white;
    Uint32 _black;
    SDL_Rect _net;
};

#endif
