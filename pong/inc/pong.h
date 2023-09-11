#ifndef PADDLE_H_
#define PADDLE_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct s_game Game;
typedef struct s_object Ball;
typedef struct s_object Paddle;

typedef enum e_directions Directions;
typedef enum e_textures Textures;
typedef enum e_font_size Font_size;

struct s_object {
    SDL_Rect r;
    float pos_x;
    float pos_y;
    float dx;
    float dy;
};

struct s_game {
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * textures[4];
    TTF_Font * fonts[3];
    SDL_Color grey;
    SDL_Event event;
    int r_score;
    int l_score;
    bool quit;
    bool pause;
    bool serve;
    Ball ball;
    Paddle r_pad;
    Paddle l_pad;
    SDL_Rect pause_msg;
};

enum e_directions {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum e_textures {
    PADDLE,
    BALL,
    BG,
    PAUSE
};

enum e_font_size {
    SMALL,
    MEDIUM,
    BIG,
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
