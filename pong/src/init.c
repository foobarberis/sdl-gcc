#include "pong.h"

static void game_init_vars(Game * game)
{
    game->l_score = 0;
    game->r_score = 0;
    game->quit = false;
    game->pause = false;
    game->serve = false;

    game->grey.r = 200;
    game->grey.g = 200;
    game->grey.b = 200;
    game->grey.a = 0;
}

static int game_init_fonts(Game * game)
{
    if (!(game->fonts[SMALL] = TTF_OpenFont("./assets/gugi-regular.ttf", 16))) {
        return 1;
    }
    if (!(game->fonts[MEDIUM] = TTF_OpenFont("./assets/gugi-regular.ttf", 22))) {
        TTF_CloseFont(game->fonts[SMALL]);
        return 1;
    }
    if (!(game->fonts[BIG] = TTF_OpenFont("./assets/gugi-regular.ttf", 28))) {
        TTF_CloseFont(game->fonts[SMALL]);
        TTF_CloseFont(game->fonts[MEDIUM]);
        return 1;
    }
    TTF_SizeText(game->fonts[MEDIUM], "Paused, press Space to continue.", &game->pause_msg.w, &game->pause_msg.h);
    game->pause_msg.x = 10;
    game->pause_msg.y = SCREEN_HEIGHT - 2 * game->pause_msg.h;
    return 0;
}

static int game_init_textures(Game * game)
{
    SDL_Surface * surf[4];

    /* Start by creating SDL_Surface's */
    if (!(surf[PADDLE] = SDL_LoadBMP("./assets/paddle.bmp"))) {
        return 1;
    }
    if (!(surf[BALL] = SDL_LoadBMP("./assets/ball.bmp"))) {
        SDL_FreeSurface(surf[PADDLE]);
        return 1;
    }
    if (!(surf[BG] = SDL_LoadBMP("./assets/bg.bmp"))) {
        SDL_FreeSurface(surf[PADDLE]);
        SDL_FreeSurface(surf[BALL]);
        return 1;
    }
    if (!(surf[PAUSE] = TTF_RenderText_Solid(game->fonts[MEDIUM], "Paused, press Space to continue.", game->grey))) {
        SDL_FreeSurface(surf[PADDLE]);
        SDL_FreeSurface(surf[BALL]);
        SDL_FreeSurface(surf[BG]);
        return 1;
    }

    /* Turn SDL_Surface's into SDL_Texture's */
    if (!(game->textures[PADDLE] = SDL_CreateTextureFromSurface(game->renderer, surf[PADDLE]))) {
        SDL_FreeSurface(surf[PADDLE]);
        SDL_FreeSurface(surf[BALL]);
        SDL_FreeSurface(surf[BG]);
        SDL_FreeSurface(surf[PAUSE]);
        return 1;
    }
    if (!(game->textures[BALL] = SDL_CreateTextureFromSurface(game->renderer, surf[BALL]))) {
        SDL_FreeSurface(surf[PADDLE]);
        SDL_FreeSurface(surf[BALL]);
        SDL_FreeSurface(surf[BG]);
        SDL_FreeSurface(surf[PAUSE]);
        SDL_DestroyTexture(game->textures[PADDLE]);
        return 1;
    }
    if (!(game->textures[BG] = SDL_CreateTextureFromSurface(game->renderer, surf[BG]))) {
        SDL_FreeSurface(surf[PADDLE]);
        SDL_FreeSurface(surf[BALL]);
        SDL_FreeSurface(surf[BG]);
        SDL_FreeSurface(surf[PAUSE]);
        SDL_DestroyTexture(game->textures[PADDLE]);
        SDL_DestroyTexture(game->textures[BALL]);
        return 1;
    }
    if (!(game->textures[PAUSE] = SDL_CreateTextureFromSurface(game->renderer, surf[PAUSE]))) {
        SDL_FreeSurface(surf[PADDLE]);
        SDL_FreeSurface(surf[BALL]);
        SDL_FreeSurface(surf[BG]);
        SDL_FreeSurface(surf[PAUSE]);
        SDL_DestroyTexture(game->textures[PADDLE]);
        SDL_DestroyTexture(game->textures[BALL]);
        SDL_DestroyTexture(game->textures[BG]);
        return 1;
    }
    SDL_FreeSurface(surf[PADDLE]);
    SDL_FreeSurface(surf[BALL]);
    SDL_FreeSurface(surf[BG]);
    SDL_FreeSurface(surf[PAUSE]);
    return 0;
}

static int game_init_sdl(Game * game)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    } else {
        if (TTF_Init() < 0) {
            fprintf(stderr, "SDL_ttf could not initialize! SDL_Error: %s\n", TTF_GetError());
            return 1;
        }
        if (!(game->window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN))) {
            fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return 1;
        } else {
            if (!(game->renderer = SDL_CreateRenderer(game->window, -1, 0))) {
                fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                return 1;
            }
        }
    }
    return 0;
}

int game_init(Game * game)
{
    game_init_vars(game);
    if (game_init_sdl(game))
        return 1;
    if (game_init_fonts(game)) {
        fprintf(stderr, "SDL_ttf encountered an error while opening a font! SDL_Error: %s\n", TTF_GetError());
        return 1;
    }
    if (game_init_textures(game)) {
        fprintf(stderr, "An error occured while loading textures! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    game_reset(game);
    return 0;
}

void game_quit(Game * game)
{
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_DestroyTexture(game->textures[PADDLE]);
    SDL_DestroyTexture(game->textures[BALL]);
    SDL_DestroyTexture(game->textures[BG]);
    SDL_DestroyTexture(game->textures[PAUSE]);
    TTF_CloseFont(game->fonts[SMALL]);
    TTF_CloseFont(game->fonts[MEDIUM]);
    TTF_CloseFont(game->fonts[BIG]);
    TTF_Quit();
    SDL_Quit();
}

void game_reset(Game * game)
{
    SDL_Delay(500);
    game->serve = false;
    ball_init(&game->ball);
    paddle_init(&game->l_pad, LEFT);
    paddle_init(&game->r_pad, RIGHT);
}

void ball_init(Ball * ball)
{
    ball->pos_x = (SCREEN_WIDTH - ball->r.w) / 2;
    ball->pos_y = (SCREEN_HEIGHT - ball->r.h) / 2;
    ball->dx = 0.25;
    ball->dy = 0.25;
    rect_set(&ball->r, ball->pos_x, ball->pos_y, 16, 16);
}

void paddle_init(Paddle * paddle, int position)
{
    if (position == LEFT) {
        paddle->pos_x = paddle->r.x = 10;
    } else {
        paddle->pos_x = paddle->r.x = SCREEN_WIDTH - (10 + paddle->r.w);
    }
    paddle->pos_y = (SCREEN_HEIGHT - paddle->r.h) / 2;
    paddle->dx = 0;
    paddle->dy = 0.50;
    rect_set(&paddle->r, paddle->pos_x, paddle->pos_y, 16, 128);
}