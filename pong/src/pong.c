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
    // FIXME: Add check for NULL
    game->textures[PADDLE] = SDL_CreateTextureFromSurface(game->renderer, SDL_LoadBMP("./assets/paddle.bmp"));
    game->textures[BALL] = SDL_CreateTextureFromSurface(game->renderer, SDL_LoadBMP("./assets/ball.bmp"));
    game->textures[NET] = SDL_CreateTextureFromSurface(game->renderer, SDL_LoadBMP("./assets/net.bmp"));
    game->l_score = 0;
    game->r_score = 0;
    game->quit = false;
    game->pause = false;
    game->serve = false;
    rect_set(&game->net, (SCREEN_WIDTH - game->net.w) / 2, 0, 16, SCREEN_HEIGHT);
    paddle_init(&game->l_pad, LEFT);
    paddle_init(&game->r_pad, RIGHT);
    return 0;
}

void game_reset(Game * game)
{
    game->serve = false;
    ball_init(&game->ball);
    paddle_init(&game->l_pad, LEFT);
    paddle_init(&game->r_pad, RIGHT);
}

void game_draw(Game * game)
{
    SDL_RenderClear(game->renderer);
    SDL_RenderCopy(game->renderer, game->textures[PADDLE], NULL, &game->l_pad.r);
    SDL_RenderCopy(game->renderer, game->textures[PADDLE], NULL, &game->r_pad.r);
    SDL_RenderCopy(game->renderer, game->textures[BALL], NULL, &game->ball.r);
    SDL_RenderCopy(game->renderer, game->textures[NET], NULL, &game->net);
    SDL_RenderPresent(game->renderer);
}

void game_update(Game * game)
{
    game_collision_handler(game);
    if (game->serve)
        ball_move(&game->ball);
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
        game->ball.dir_x = -game->ball.dir_x; /* change the direction of the 'serve' */
    } else if (game->ball.pos_x <= 0) {
        game->r_score++;
        game_reset(game);
    } else if ((game->ball.pos_y <= 1) || (game->ball.pos_y + game->ball.r.h >= SCREEN_HEIGHT - 1)) {
        game->ball.dir_y = -game->ball.dir_y;
    } else {
        if (check_collision(&game->l_pad, &game->ball)) {
            game->ball.dir_x -= 0.01;
            game->ball.dir_x = -game->ball.dir_x;
            game->ball.pos_x = game->l_pad.pos_x + game->l_pad.r.w;
        } else if (check_collision(&game->r_pad, &game->ball)) {
            game->ball.dir_x += 0.01;
            game->ball.dir_x = -game->ball.dir_x;
            game->ball.pos_x = game->r_pad.pos_x - game->r_pad.r.w;
        }
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
    ball->pos_x = (SCREEN_WIDTH - ball->r.w) / 2;
    ball->pos_y = (SCREEN_HEIGHT - ball->r.h) / 2;
    ball->dir_x = 0.25;
    ball->dir_y = 0.25;
    rect_set(&ball->r, ball->pos_x, ball->pos_y, 15, 15);
}

// https://gamedev.stackexchange.com/questions/138837/drawing-at-floating-point-position
// TODO Could use SDL_FRect instead of SDL_Rect
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
    if (position == LEFT) {
        paddle->pos_x = paddle->r.x = 10;
    } else {
        paddle->pos_x = paddle->r.x = SCREEN_WIDTH - (10 + paddle->r.w);
    }
    paddle->pos_y = (SCREEN_HEIGHT - paddle->r.h) / 2;
    paddle->dir_x = 0;
    paddle->dir_y = 0.50;
    rect_set(&paddle->r, paddle->pos_x, paddle->pos_y, 15, 100);
}
