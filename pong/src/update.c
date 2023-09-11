#include "pong.h"

// https://gamedev.stackexchange.com/questions/138837/drawing-at-floating-point-position
// TODO Could use SDL_FRect instead of SDL_Rect
void ball_move(Ball * ball)
{
    ball->pos_x += ball->dx;
    ball->pos_y += ball->dy;
}

void paddle_move(Paddle * paddle, int direction)
{
    if (direction == UP) {
        if (paddle->pos_y - paddle->dy >= 5.0)
            paddle->pos_y -= paddle->dy;
    } else if (direction == DOWN) {
        if (paddle->pos_y + (float)paddle->r.h + paddle->dy <= SCREEN_HEIGHT - 5)
            paddle->pos_y += paddle->dy;
    }
}

void game_update(Game * game)
{
    if (game->serve)
        ball_move(&game->ball);
    game_collision_handler(game);
    rect_set(&game->l_pad.r, game->l_pad.pos_x, game->l_pad.pos_y, 0, 0);
    rect_set(&game->r_pad.r, game->r_pad.pos_x, game->r_pad.pos_y, 0, 0);
    rect_set(&game->ball.r, game->ball.pos_x, game->ball.pos_y, 0, 0);
}

void game_input_handler(Game * game)
{
    SDL_PumpEvents();
    Uint8 const * keyboard_state_array = SDL_GetKeyboardState(NULL);
    if (keyboard_state_array[SDL_SCANCODE_A])
        paddle_move(&game->l_pad, UP);
    else if (keyboard_state_array[SDL_SCANCODE_S])
        paddle_move(&game->l_pad, DOWN);
    if (keyboard_state_array[SDL_SCANCODE_K])
        paddle_move(&game->r_pad, UP);
    else if (keyboard_state_array[SDL_SCANCODE_L])
        paddle_move(&game->r_pad, DOWN);

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
