#include "pong.h"

bool RayVsRect(Vec2 * ray_origin, Vec2 * ray_dir, SDL_FRect * target, Vec2 * contact_point, Vec2 * contact_normal, float * t_hit_near)
{
    contact_normal->x = contact_normal->y = 0;
    contact_point->x = contact_point->y = 0;

    // Cache division
    Vec2 invdir;
    invdir.x = 1.0f / ray_dir->x;
    invdir.y = 1.0f / ray_dir->y;

    // Calculate intersections with rectangle bounding axes
    Vec2 t_near, t_far;
    t_near.x = (target->x - ray_origin->x) * invdir.x;
    t_near.y = (target->y - ray_origin->y) * invdir.y;
    t_far.x = (target->x + target->w - ray_origin->x) * invdir.x;
    t_far.y = (target->y + target->h - ray_origin->y) * invdir.y;

    if (isnan(t_far.y) || isnan(t_far.x))
        return false;
    if (isnan(t_near.y) || isnan(t_near.x))
        return false;

    // Sort distances
    float tmp;
    if (t_near.x > t_far.x) {
        tmp = t_near.x;
        t_near.x = t_far.x;
        t_far.x = tmp;
    }
    if (t_near.y > t_far.y) {
        tmp = t_near.y;
        t_near.y = t_far.y;
        t_far.y = tmp;
    }

    // Early rejection
    if (t_near.x > t_far.y || t_near.y > t_far.x)
        return false;

    // Closest 'time' will be the first contact
    *t_hit_near = fmaxf(t_near.x, t_near.y);

    // Furthest 'time' is contact on opposite side of target
    float t_hit_far = fminf(t_far.x, t_far.y);

    // Reject if ray direction is pointing away from object
    if (t_hit_far < 0)
        return false;

    // Contact point of collision from parametric line equation
    contact_point->x = ray_origin->x + *t_hit_near * ray_dir->x;
    contact_point->y = ray_origin->y + *t_hit_near * ray_dir->y;

    if (t_near.x > t_near.y) {
        if (invdir.x < 0) {
            contact_normal->x = 1;
            contact_normal->y = 0;
        } else {
            contact_normal->x = -1;
            contact_normal->y = 0;
        }
    } else if (t_near.x < t_near.y) {
        if (invdir.y < 0) {
            contact_normal->x = 0;
            contact_normal->y = 1;
        } else {
            contact_normal->x = 0;
            contact_normal->y = -1;
        }
    }

    // Note if t_near == t_far, collision is principly in a diagonal
    // so pointless to resolve. By returning a CN={0,0} even though its
    // considered a hit, the resolver wont change anything.
    return true;
}

bool DynamicRectVsRect(Ball * r_dynamic, const float fTimeStep, Paddle * r_static, Vec2 * contact_point, Vec2 * contact_normal, float * contact_time)
{
    // Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
    // if (r_dynamic->vel.x == 0 && r_dynamic->vel.y == 0)
    //     return false;

    if (r_dynamic->velocity == 0)
        return false;

    // Expand target rectangle by source dimensions
    SDL_FRect expanded_target;
    expanded_target.x = r_static->r.x - r_dynamic->r.w / 2;
    expanded_target.y = r_static->r.y - r_dynamic->r.h / 2;
    expanded_target.w = r_static->r.w + r_dynamic->r.w;
    expanded_target.h = r_static->r.h + r_dynamic->r.h;

    // bool RayVsRect(Vec2 * ray_origin, Vec2 * ray_dir, SDL_Rect * target, Vec2 * contact_point, Vec2 * contact_normal, float * t_hit_near)
    Vec2 ray_origin;
    ray_origin.x = r_dynamic->r.x + r_dynamic->r.w / 2;
    ray_origin.y = r_dynamic->r.y + r_dynamic->r.h / 2;
    Vec2 ray_dir;
    ray_dir.x = r_dynamic->v.x * fTimeStep;
    ray_dir.y = r_dynamic->v.y * fTimeStep;
    if (RayVsRect(&ray_origin, &ray_dir, &expanded_target, contact_point, contact_normal, contact_time))
        return (*contact_time >= 0.0f && *contact_time < 1.0f);
    else
        return false;
}

static int check_collision(SDL_FRect * r1, SDL_FRect * r2)
{
    return ((r1->x < r2->x + r2->w)
        && (r2->x < r1->x + r1->w)
        && (r1->y < r2->y + r2->h)
        && (r2->y < r1->y + r1->h));
}

/* static int check_collision(SDL_FRect * r1, SDL_FRect * r2)
{
    bool lat = (r1->x < r2->x + r2->w) && (r2->x < r1->x + r1->w);
        bool top = (r1->y < r2->y + r2->h) && (r2->y < r1->y + r1->h));
} */

static void ball_update_after_collision(Paddle * paddle, Ball * ball)
{
    float dx = -1;
    float dy = -1;
    float ball_center_x = ball->r.x + ball->r.w / 2;
    float ball_center_y = ball->r.y + ball->r.h / 2;
    float max_width = paddle->r.w + (2 * ball->r.w);
    float max_height = paddle->r.h + (2 * ball->r.h);
    float step_x = fabsf((dy * 2) / max_width);
    float step_y = fabsf((dy * 2) / max_height);
    float hit_x = fabsf((paddle->r.x - ball->r.w) - ball_center_x);
    float hit_y = fabsf((paddle->r.y - ball->r.h) - ball_center_y);
    // ball->v.x = dx + (hit_x * step_x);
    ball->v.y = dy + (hit_y * step_y);
    // printf("hit_y: %f, hit_y * step_y: %f\n", hit_y, hit_y * step_y);
}

/* Handle the collision of the ball with the paddles and the walls */
static void game_collision_handler(Game * game)
{
    Vec2 cp, cn;
    float t = 0;
    if (game->ball.r.x > game->viewport.x + game->viewport.w + game->ball.r.w) { /* goes out right */
        game->l_score++;
        SDL_Delay(500);
        game_reset(game);
        game->ball.v.x = -game->ball.v.x;
    } else if (game->ball.r.x < game->viewport.x - (2 * game->ball.r.w)) { /* goes out left */
        game->r_score++;
        SDL_Delay(500);
        game_reset(game);
    } else if ((game->ball.r.y <= game->viewport.y + BORDER_SIZE)) { /* collision top */
        game->ball.v.y = -game->ball.v.y;
        game->ball.r.y = game->viewport.y + BORDER_SIZE;
    } else if (game->ball.r.y + game->ball.r.h >= game->viewport.y + game->viewport.h - BORDER_SIZE) { /* collision bottom */
        game->ball.v.y = -game->ball.v.y;
        game->ball.r.y = game->viewport.y + game->viewport.h - BORDER_SIZE - game->ball.r.h;
    } else if (DynamicRectVsRect(&game->ball, game->dt, &game->l_pad, &cp, &cn, &t) || DynamicRectVsRect(&game->l_pad, game->dt, &game->ball, &cp, &cn, &t)) { /* collision left paddle */
        printf("cn.x: %f, cn.y: %f\n", cn.x, cn.y);
        game->ball.r.x += cn.x + fabsf(game->ball.v.x) * (1 - t);
        game->ball.r.y += cn.y + fabsf(game->ball.v.y) * (1 - t);
        ball_update_after_collision(&game->l_pad, &game->ball);
        game->ball.v.x = -game->ball.v.x;
    } else if (DynamicRectVsRect(&game->ball, game->dt, &game->r_pad, &cp, &cn, &t) || DynamicRectVsRect(&game->r_pad, game->dt, &game->ball, &cp, &cn, &t)) { /* collision right paddle */
        printf("cn.x: %f, cn.y: %f\n", cn.x, cn.y);
        game->ball.r.x += cn.x + fabsf(game->ball.v.x) * (1 - t);
        game->ball.r.y += cn.y + fabsf(game->ball.v.y) * (1 - t);
        ball_update_after_collision(&game->r_pad, &game->ball);
        game->ball.v.x = -game->ball.v.x;
    }
}

void game_update(Game * game)
{
    if (game->serve) {
        game->ball.r.x += game->ball.v.x * game->ball.velocity * game->dt;
        game->ball.r.y += game->ball.v.y * game->ball.velocity * game->dt;
    }
    game_collision_handler(game);
}
