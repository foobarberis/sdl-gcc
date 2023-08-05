#include "Ball.class.hpp"
#include "Paddle.class.hpp"
#include "Pong.class.hpp"
#include <SDL.h>
#include <iostream>

static int initContext(SDL_Window ** Window, SDL_Surface ** Surface)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return -1;
    } else {
        *Window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (*Window == NULL) {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            return -1;
        } else {
            *Surface = SDL_GetWindowSurface(*Window);
        }
    }
    return 0;
}

int main(int argc, char ** argv)
{
    (void)argc, (void)argv;

    SDL_Event e;
    SDL_Window * Window = NULL;
    SDL_Surface * Surface = NULL;
    if (initContext(&Window, &Surface) < 0) {
        std::cout << "Failed to initialize!\n";
        return 1;
    }

    Pong p(Surface);

    while (p.getQuit() == false) {
        p.updateGame(&e);
        p.drawGame(Window, Surface);
    }
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}
