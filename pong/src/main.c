#include "pong.h"

int main(int argc, char ** argv)
{
    (void)argc, (void)argv;
    Game * game = (Game *)malloc(sizeof(Game));
    if (game_init(game)) {
        free(game);
        return EXIT_FAILURE;
    }
    while (game->quit == false) {
        game_input_handler(game);
        if (game->pause == false) {
            game_update(game);
            game_draw(game);
        }
    }
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
    free(game);
    return EXIT_SUCCESS;
}
