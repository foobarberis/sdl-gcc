#include "pong.h"

const unsigned int g_screen_width = 800;
const unsigned int g_screen_height = 600;

// const unsigned int g_screen_width = 1920;
// const unsigned int g_screen_height = 1080;

// const unsigned int g_screen_width = 640;
// const unsigned int g_screen_height = 512;

int main(int argc, char ** argv)
{
    (void)argc, (void)argv;
    srand(time(NULL));
    Game * game = (Game *)malloc(sizeof(Game));
    if (game_init(game)) {
        free(game);
        return EXIT_FAILURE;
    }
    Uint64 time_now = SDL_GetPerformanceCounter();
    Uint64 time_last = 0;
    while (game->quit == false) {
        time_last = time_now;
        time_now = SDL_GetPerformanceCounter();
        game->dt = (double)((time_now - time_last) * 1000 / (double)SDL_GetPerformanceFrequency());
        // printf("dt: %f\n", game->dt);
        game_input_handler(game);
        if (!game->pause) {
            game_update(game);
        }
        game_draw(game);
    }
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    free(game);
    return EXIT_SUCCESS;
}
