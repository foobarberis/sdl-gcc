#ifndef BALL_CLASS_HPP
#define BALL_CLASS_HPP

#include "Paddle.class.hpp"
#include <SDL.h>

class Ball : public Paddle {
public:
    Ball();
    virtual ~Ball();

    void moveBall();
};

#endif
