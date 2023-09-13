#include "pong.h"

static int check_collision(SDL_FRect * r1, SDL_FRect * r2)
{
    return ((r1->x < r2->x + r2->w)
        && (r2->x < r1->x + r1->w)
        && (r1->y < r2->y + r2->h)
        && (r2->y < r1->y + r1->h));
}

static void ball_update_after_collision(Paddle * paddle, Ball * ball)
{
    int precision = 8;
    int slice = paddle->r.h / precision;
    int hit_position = (paddle->r.y + paddle->r.h) - ball->r.y;
    float dy = 0.5;
    float ddy = (2 * dy) / precision;
    for (int i = 0; i < precision; i++) {
        if ((hit_position >= i * slice) && (hit_position < (i + 1) * slice))
            ball->v.y = dy * ball->v.x;
        dy = dy - ddy;
    }
}

/* Handle the collision of the ball with the paddles and the walls */
static void game_collision_handler(Game * game)
{
    if (game->ball.r.x > g_screen_width + game->ball.r.w) { /* goes out right */
        game->l_score++;
        game_reset(game);
        game->ball.v.x = -game->ball.v.x; /* change the direction of the 'serve' */
    } else if (game->ball.r.x < 10 * -game->ball.r.w) { /* goes out left */
        game->r_score++;
        game_reset(game);
    } else if ((game->ball.r.y <= BORDER_SIZE) || (game->ball.r.y + game->ball.r.h >= g_screen_height - BORDER_SIZE)) {
        game->ball.v.y = -game->ball.v.y;
    } else {
        if (check_collision(&game->l_pad.r, &game->ball.r)) {
            game->ball.v.x -= 0.05;
            game->ball.v.x = -game->ball.v.x;
            game->ball.r.x = game->l_pad.r.x + game->l_pad.r.w;
            ball_update_after_collision(&game->l_pad, &game->ball);
        } else if (check_collision(&game->r_pad.r, &game->ball.r)) {
            game->ball.v.x += 0.05;
            game->ball.v.x = -game->ball.v.x;
            game->ball.r.x = game->r_pad.r.x - game->ball.r.w;
            ball_update_after_collision(&game->r_pad, &game->ball);
        }
    }
}

void game_update(Game * game)
{
    if (game->serve) {
        game->ball.r.x += game->ball.v.x * game->dt;
        game->ball.r.y += game->ball.v.y * game->dt;
    }
    game_collision_handler(game);
}
