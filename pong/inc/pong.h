#ifndef PADDLE_H_
#define PADDLE_H_

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct s_game Game;
typedef struct s_object Ball;
typedef struct s_object Paddle;

typedef enum e_directions Directions;

struct s_object {
    SDL_Rect r;
    float posX; // rename to camel_case
    float posY; // rename to camel_case
    float dirX; // rename to camel_case
    float dirY; // rename to camel_case
};

struct s_game {
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Event event;
    int r_score;
    int l_score;
    bool quit;
    bool pause;
    Ball ball;
    Paddle r_pad;
    Paddle l_pad;
    SDL_Rect net;
};

enum e_directions {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

int game_init(Game * game);
void game_reset(Game * game);
void game_draw(Game * game);
void game_update(Game * game);
void game_input_handler(Game * game);
void game_collision_handler(Game * game);

void ball_init(Ball * ball);
void ball_move(Ball * ball);
void paddle_init(Paddle * paddle, int position);
void paddle_move(Paddle * paddle, int direction);

void rect_set(SDL_Rect * r, int x, int y, int w, int h);

#endif
