#include "pong.h"

const float g_screen_width = 800.0;
const float g_screen_height = 600.0;

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

    free(game);
    return EXIT_SUCCESS;
}
