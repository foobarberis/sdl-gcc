#include "pong.h"

int game_init(Game * game)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    } else {
        game->window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!game->window) {
            fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return EXIT_FAILURE;
        } else {
            game->renderer = SDL_CreateRenderer(game->window, -1, 0);
        }
    }
    game->l_score = 0;
    game->r_score = 0;
    game->quit = false;
    game->pause = false;
    rect_set(&game->net, (SCREEN_WIDTH - game->net.w) / 2, 10, 15, 20);
    paddle_init(&game->l_pad, LEFT);
    paddle_init(&game->r_pad, RIGHT);
    return 0;
}

void game_reset(Game * game)
{
    ball_init(&game->ball);
    paddle_init(&game->l_pad, LEFT);
    paddle_init(&game->r_pad, RIGHT);
}

void game_draw(Game * game)
{
    bool draw = true;

    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(game->renderer);
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(game->renderer, &game->l_pad.r);
    SDL_RenderFillRect(game->renderer, &game->r_pad.r);
    SDL_RenderFillRect(game->renderer, &game->ball.r);
    for (game->net.y = 10; game->net.y < SCREEN_HEIGHT; game->net.y += 20) {
        if (draw) {
            SDL_RenderFillRect(game->renderer, &(game->net));
            draw = false;
        } else {
            draw = true;
        }
    }
    SDL_RenderPresent(game->renderer);
}

void game_update(Game * game)
{
    game_input_handler(game);
    game_collision_handler(game);
    ball_move(&game->ball);
    rect_set(&game->l_pad.r, game->l_pad.pos_x, game->l_pad.pos_y, 0, 0);
    rect_set(&game->r_pad.r, game->r_pad.pos_x, game->r_pad.pos_y, 0, 0);
    rect_set(&game->ball.r, game->ball.pos_x, game->ball.pos_y, 0, 0);
}

void game_input_handler(Game * game)
{
    Uint8 const * keyboard_state_array = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&game->event) != 0) {
        if (game->event.type == SDL_QUIT) {
            game->quit = true;
        } else {
            if (keyboard_state_array[SDL_SCANCODE_ESCAPE])
                game->quit = true;
            if (keyboard_state_array[SDL_SCANCODE_A])
                paddle_move(&game->l_pad, UP);
            else if (keyboard_state_array[SDL_SCANCODE_S])
                paddle_move(&game->l_pad, DOWN);
            if (keyboard_state_array[SDL_SCANCODE_K])
                paddle_move(&game->r_pad, UP);
            else if (keyboard_state_array[SDL_SCANCODE_L])
                paddle_move(&game->r_pad, DOWN);
        }
    }
}

/* check if a collision occured between the ball and the paddle */
static int check_collision(Paddle * paddle, Ball * ball)
{
    return ((paddle->pos_x < ball->pos_x + ball->r.w)
        && (ball->pos_x < paddle->pos_x + paddle->r.w)
        && (paddle->pos_y < ball->pos_y + ball->r.h)
        && (ball->pos_y < paddle->pos_y + paddle->r.h));
}

/* Handle the collision of the ball with the paddles and the walls */
void game_collision_handler(Game * game)
{
    if (game->ball.pos_x + game->ball.r.w >= SCREEN_WIDTH - 1) {
        game->l_score++;
        game_reset(game);
    } else if (game->ball.pos_x <= 0) {
        game->r_score++;
        game_reset(game);
    } else if ((game->ball.pos_y <= 1) || (game->ball.pos_y + game->ball.r.h >= SCREEN_HEIGHT - 1)) {
        game->ball.dir_y = -game->ball.dir_y;
    } else if (check_collision(&game->l_pad, &game->ball) || check_collision(&game->r_pad, &game->ball)) {
        game->ball.dir_x = -game->ball.dir_x;
    }
}

void rect_set(SDL_Rect * r, int x, int y, int w, int h)
{
    r->x = x;
    r->y = y;
    if (w && h) {
        r->w = w;
        r->h = h;
    }
}

void ball_init(Ball * ball)
{
    ball->r.w = 15;
    ball->r.h = ball->r.w;
    ball->pos_x = (SCREEN_WIDTH - ball->r.w) / 2;
    ball->pos_y = (SCREEN_HEIGHT - ball->r.h) / 2;
    ball->r.x = (SCREEN_WIDTH - ball->r.w) / 2;
    ball->r.y = (SCREEN_HEIGHT - ball->r.h) / 2;
    ball->dir_x = 0.25;
    ball->dir_y = 0.25;
}

// https://gamedev.stackexchange.com/questions/138837/drawing-at-floating-point-position
void ball_move(Ball * ball)
{
    ball->pos_x += ball->dir_x;
    ball->pos_y += ball->dir_y;
}

void paddle_move(Paddle * paddle, int direction)
{
    if (direction == UP) {
        if (paddle->pos_y - paddle->dir_y >= 5.0)
            paddle->pos_y -= paddle->dir_y;
    } else if (direction == DOWN) {
        if (paddle->pos_y + (float)paddle->r.h + paddle->dir_y <= SCREEN_HEIGHT - 5)
            paddle->pos_y += paddle->dir_y;
    }
}

void paddle_init(Paddle * paddle, int position)
{
    paddle->r.h = 100;
    paddle->r.w = 15;
    paddle->r.x = 0;
    paddle->r.y = (SCREEN_HEIGHT - paddle->r.h) / 2;
    paddle->dir_x = 0;
    paddle->dir_y = 10; // SCREEN_HEIGHT / 70;
    if (position == LEFT)
        paddle->pos_x = 10;
    else
        paddle->pos_x = SCREEN_WIDTH - (10 + paddle->r.w);
}
