#include "pong.h"

/* check if a collision occured between the ball and the paddle */
// FIXME: Change this to accept SDL_Rect struct as argument
static int check_collision(SDL_Rect * r1, SDL_Rect * r2)
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
    int hit_position = (paddle->pos_y + paddle->r.h) - ball->pos_y;
    float dy = 0.5;
    float ddy = (2 * dy) / precision;
    for (int i = 0; i < precision; i++) {
        if ((hit_position >= i * slice) && (hit_position < (i + 1) * slice))
            ball->dy = dy * ball->dx;
        dy = dy - ddy;
    }
}

/* Handle the collision of the ball with the paddles and the walls */
void game_collision_handler(Game * game)
{
    if (game->ball.pos_x > SCREEN_WIDTH + game->ball.r.w) { /* goes out right */
        game->l_score++;
        game_reset(game);
        game->ball.dx = -game->ball.dx; /* change the direction of the 'serve' */
    } else if (game->ball.pos_x < 10 * -game->ball.r.w) { /* goes out left */
        game->r_score++;
        game_reset(game);
    } else if ((game->ball.pos_y <= 9) || (game->ball.pos_y + game->ball.r.h >= SCREEN_HEIGHT - 9)) {
        game->ball.dy = -game->ball.dy;
    } else {
        if (check_collision(&game->l_pad.r, &game->ball.r)) {
            game->ball.dx -= 0.05;
            game->ball.dx = -game->ball.dx;
            game->ball.pos_x = game->l_pad.pos_x + game->l_pad.r.w;
            ball_update_after_collision(&game->l_pad, &game->ball);
        } else if (check_collision(&game->r_pad.r, &game->ball.r)) {
            game->ball.dx += 0.05;
            game->ball.dx = -game->ball.dx;
            game->ball.pos_x = game->r_pad.pos_x - game->ball.r.w;
            ball_update_after_collision(&game->l_pad, &game->ball);
        }
    }
}
