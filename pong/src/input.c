#include "pong.h"

static void paddle_move(Paddle * paddle, int direction, double dt)
{
    if (direction == UP) {
        if (paddle->r.y - paddle->v.y >= BORDER_SIZE)
            paddle->r.y -= paddle->v.y * dt;
    } else if (direction == DOWN) {
        if (paddle->r.y + paddle->r.h + paddle->v.y <= g_screen_height - BORDER_SIZE)
            paddle->r.y += paddle->v.y * dt;
    }
}

void game_input_handler(Game * game)
{
    SDL_PumpEvents();
    Uint8 const * keyboard_state_array = SDL_GetKeyboardState(NULL);
    if (keyboard_state_array[SDL_SCANCODE_A])
        paddle_move(&game->l_pad, UP, game->dt);
    else if (keyboard_state_array[SDL_SCANCODE_S])
        paddle_move(&game->l_pad, DOWN, game->dt);
    if (keyboard_state_array[SDL_SCANCODE_K])
        paddle_move(&game->r_pad, UP, game->dt);
    else if (keyboard_state_array[SDL_SCANCODE_L])
        paddle_move(&game->r_pad, DOWN, game->dt);

    while (SDL_PollEvent(&game->event)) {
        switch (game->event.type) {
        case SDL_QUIT:
            game->quit = true;
            break;
        case SDL_KEYDOWN:
            if (game->event.key.keysym.sym == SDLK_ESCAPE)
                game->quit = true;
            else if (game->event.key.keysym.sym == SDLK_p)
                game->serve = true;
            else if (game->event.key.keysym.sym == SDLK_SPACE) {
                if (game->pause == false)
                    game->pause = true;
                else
                    game->pause = false;
            }
            break;
        }
    }
}
