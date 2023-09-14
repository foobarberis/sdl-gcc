#include "pong.h"

static void game_init_variables(Game * game)
{
    game->l_score = 0;
    game->r_score = 0;
    game->quit = false;
    game->pause = false;
    game->serve = false;
}

static void game_init_viewport(Game * game)
{
    game->viewport.w = g_screen_width;
    game->viewport.h = g_screen_height;
    if ((float)g_screen_width / (float)g_screen_height > ASPECT_RATIO) {
        game->viewport.w = roundf(game->viewport.h * ASPECT_RATIO); /* window is wider, use height as base */
    } else {
        game->viewport.h = roundf(game->viewport.w / ASPECT_RATIO); /* window is taller, use width as base */
    }
    game->viewport.x = (g_screen_width - game->viewport.w) / 2;
    game->viewport.y = (g_screen_height - game->viewport.h) / 2;
}

static void game_init_colors(Game * game)
{
    game->grey.r = 200;
    game->grey.g = 200;
    game->grey.b = 200;
    game->grey.a = 0;
}

static int game_init_fonts(Game * game)
{
    if (!(game->fonts[SMALL] = TTF_OpenFont("./assets/gugi-regular.ttf", 18))) {
        return 1;
    }
    if (!(game->fonts[MEDIUM] = TTF_OpenFont("./assets/gugi-regular.ttf", 20))) {
        TTF_CloseFont(game->fonts[SMALL]);
        return 1;
    }
    if (!(game->fonts[BIG] = TTF_OpenFont("./assets/gugi-regular.ttf", 22))) {
        TTF_CloseFont(game->fonts[SMALL]);
        TTF_CloseFont(game->fonts[MEDIUM]);
        return 1;
    }
    TTF_SizeText(game->fonts[MEDIUM], "Paused, press Space to continue.", &game->pause_msg.w, &game->pause_msg.h);
    game->pause_msg.x = game->l_pad.r.x;
    game->pause_msg.y = g_screen_height - 2 * game->pause_msg.h;
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
        if (!(game->window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g_screen_width, g_screen_height, SDL_WINDOW_SHOWN))) {
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
    game_init_viewport(game);
    game_reset(game);
    game_init_variables(game);
    game_init_colors(game);
    if (game_init_sdl(game))
        return 1;
    if (game_init_fonts(game)) {
        fprintf(stderr, "An error occured while loading fonts! SDL_Error: %s\n", TTF_GetError());
        return 1;
    }
    if (game_init_textures(game)) {
        fprintf(stderr, "An error occured while loading textures! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
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
    game->serve = false;

    game->ball.r.w = game->ball.r.h = game->viewport.w / 50; /* ball's width and height */
    game->ball.r.x = game->viewport.x + (game->viewport.w - game->ball.r.w) / 2; /* ball's x coordinate */
    game->ball.r.y = game->viewport.y + (game->viewport.h - game->ball.r.h) / 2; /* ball's y coordinate */
    game->ball.v.x = 1.0;
    game->ball.v.y = 0.0; // (((float)rand() / RAND_MAX) * 2.0) - 1.0;
    game->ball.velocity = 0.10; // (game->viewport.w - game->ball.r.w) / 810.0;

    game->l_pad.r.h = game->r_pad.r.h = game->viewport.h / 6.25; /* left and right paddle height */
    game->l_pad.r.w = game->r_pad.r.w = game->viewport.w / 50; /* left and right paddle width */
    game->l_pad.r.x = game->viewport.x + game->l_pad.r.w * 2; /* left paddle x coordinate */
    game->r_pad.r.x = game->viewport.x + game->viewport.w - (game->l_pad.r.w * 2 + game->l_pad.r.w); /* right paddle x coordinate */
    game->l_pad.r.y = game->r_pad.r.y = game->viewport.y + ((game->viewport.h - game->l_pad.r.h) / 2); /* left and right paddles y coordinate */
    game->l_pad.v.x = game->r_pad.v.x = 0.0; /* left and right paddles movement vector in y */
    game->l_pad.v.y = game->r_pad.v.y = 1.0; /* left and right paddles movement vector in x */
    game->l_pad.velocity = game->r_pad.velocity = (game->viewport.h - game->l_pad.r.h) / (360.0);
}
