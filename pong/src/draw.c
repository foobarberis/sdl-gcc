#include "pong.h"

void game_draw(Game * game)
{
    SDL_RenderClear(game->renderer);
    SDL_RenderCopy(game->renderer, game->textures[BG], NULL, NULL);
    SDL_RenderCopy(game->renderer, game->textures[BALL], NULL, &game->ball.r);
    SDL_RenderCopy(game->renderer, game->textures[PADDLE], NULL, &game->l_pad.r);
    SDL_RenderCopy(game->renderer, game->textures[PADDLE], NULL, &game->r_pad.r);
    if (game->pause)
        SDL_RenderCopy(game->renderer, game->textures[PAUSE], NULL, &game->pause_msg);
    SDL_RenderPresent(game->renderer);
}
