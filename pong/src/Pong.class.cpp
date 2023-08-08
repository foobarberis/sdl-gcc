#include "Pong.class.hpp"
#include <SDL.h>

Pong::Pong()
{
    this->_lPaddle.setX(10);
    this->_rPaddle.setX(SCREEN_WIDTH - (10 + this->_rPaddle.getWidth()));
    this->_lScore = 0;
    this->_rScore = 0;
    this->_quit = false;
    this->_pause = false;
    this->_net.w = 15;
    this->_net.h = 20;
    this->_net.x = (SCREEN_WIDTH - this->_net.w) / 2;
    this->_net.y = 10;
}
Pong::~Pong() { }

void Pong::drawGame(SDL_Renderer * Renderer)
{
    bool draw = true;

    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Renderer);
    SDL_SetRenderDrawColor(Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(Renderer, this->_lPaddle.getRect());
    SDL_RenderFillRect(Renderer, this->_rPaddle.getRect());
    SDL_RenderFillRect(Renderer, this->_ball.getRect());
    for (this->_net.y = 10; this->_net.y < SCREEN_HEIGHT; this->_net.y += 20) {
        if (draw) {
            SDL_RenderFillRect(Renderer, &(this->_net));
            draw = false;
        } else {
            draw = true;
        }
    }
    SDL_RenderPresent(Renderer);
}

bool Pong::getQuit()
{
    return this->_quit;
}

void Pong::processEvent(SDL_Event * Event)
{
    Uint8 const * keyboard_state_array = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(Event) != 0) {
        if (Event->type == SDL_QUIT) {
            this->_quit = true;
        } else {
            if (keyboard_state_array[SDL_SCANCODE_ESCAPE])
                this->_quit = true;
            if (keyboard_state_array[SDL_SCANCODE_A])
                this->_lPaddle.movePaddle(UP);
            else if (keyboard_state_array[SDL_SCANCODE_S])
                this->_lPaddle.movePaddle(DOWN);
            if (keyboard_state_array[SDL_SCANCODE_K])
                this->_rPaddle.movePaddle(UP);
            else if (keyboard_state_array[SDL_SCANCODE_L])
                this->_rPaddle.movePaddle(DOWN);
        }
    }
}

bool Pong::checkHitbox(Paddle const & p, Ball const & b)
{
    return ((p.getX() < b.getX() + b.getWidth())
        && (b.getX() < p.getX() + p.getWidth())
        && (p.getY() < b.getY() + b.getHeight())
        && (b.getY() < p.getY() + p.getHeight()));
}

void Pong::setPaddleBall()
{
    this->_ball.setX((SCREEN_WIDTH - this->_ball.getWidth()) / 2);
    this->_ball.setY((SCREEN_HEIGHT - this->_ball.getHeight()) / 2);
    this->_lPaddle.setY((SCREEN_HEIGHT - this->_lPaddle.getHeight()) / 2);
    this->_rPaddle.setY((SCREEN_HEIGHT - this->_rPaddle.getHeight()) / 2);
}

void Pong::handleCollisions()
{
    if (this->_ball.getX() + this->_ball.getWidth() >= SCREEN_WIDTH - 1) {
        this->_lScore++;
        setPaddleBall();
    } else if (this->_ball.getX() <= 0) {
        this->_rScore++;
        setPaddleBall();
    } else if ((this->_ball.getY() <= 1) || (this->_ball.getY() + this->_ball.getHeight() >= SCREEN_HEIGHT - 1)) {
        this->_ball.setDirY(-this->_ball.getDirY());
    } else if (checkHitbox(this->_lPaddle, this->_ball) || checkHitbox(this->_rPaddle, this->_ball)) {
        this->_ball.setDirX(-this->_ball.getDirX());
    }
}

void Pong::updateGame(SDL_Event * Event)
{
    processEvent(Event);
    handleCollisions();
    this->_ball.moveBall();
    // TODO Set r.x and r.y to posX and posY
    this->_lPaddle.updateRect();
    this->_rPaddle.updateRect();
    this->_ball.updateRect();
}
