#include "pong.h"

void game_draw(Game * game)
{
    SDL_RenderClear(game->renderer);
    SDL_RenderCopyF(game->renderer, game->textures[BG], NULL, &game->viewport);
    SDL_RenderCopyF(game->renderer, game->textures[BALL], NULL, &game->ball.r);
    SDL_RenderCopyF(game->renderer, game->textures[PADDLE], NULL, &game->l_pad.r);
    SDL_RenderCopyF(game->renderer, game->textures[PADDLE], NULL, &game->r_pad.r);
    if (game->pause)
        SDL_RenderCopy(game->renderer, game->textures[PAUSE], NULL, &game->pause_msg);
    SDL_RenderPresent(game->renderer);
}
