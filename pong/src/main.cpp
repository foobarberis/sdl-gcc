#include "Ball.class.hpp"
#include "Paddle.class.hpp"
#include "Pong.class.hpp"
#include <SDL.h>
#include <iostream>

int main(int argc, char ** argv)
{
    (void)argc, (void)argv;
    SDL_Window * Window = NULL;
    SDL_Renderer * Renderer = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return -1;
    } else {
        Window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!Window) {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            return -1;
        } else {
            Renderer = SDL_CreateRenderer(Window, -1, 0);
        }
    }

    Pong p;
    SDL_Event e;
    while (p.getQuit() == false) {
        p.updateGame(&e);
        p.drawGame(Renderer);
    }
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}
