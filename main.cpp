#include "Functions/motion.hpp"
#include "Functions/values.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

enum
{
    RIGHT,
    LEFT,
    UP,
    DOWN,
    SPACE
};
bool state[] = {false, false, false, false, false};

int main(int argv, char **args)
{
    SDL_Window *o;
    SDL_Event e;
    SDL_Rect spaceship_rect;
    int i = 1;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();

    o = SDL_CreateWindow("Space_Game",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         Width,
                         Height,
                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    // declaring renderer
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *rend = SDL_CreateRenderer(o, -1, render_flags);

    // Corner Point text

    SDL_Rect ScoreBoardRect;
    ScoreBoardRect.x = 50;
    ScoreBoardRect.y = 50;
    ScoreBoardRect.h = 50;
    ScoreBoardRect.w = 100;

    SDL_Surface *surface = IMG_Load("images/background.png");
    SDL_Texture *bg_texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    // render Game Over texture
    surface = IMG_Load("images/Game_Over.png");
    SDL_Texture *GameOver_texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    // render bullet texture
    surface = IMG_Load("images/bullet.png");
    SDL_Texture *bullet_texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    // render Explosion
    surface = IMG_Load("images/explosion.png");
    SDL_Texture *explosion_texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    // render asteroid texture
    surface = IMG_Load("images/asteroid.png");
    SDL_Texture *asteroid_texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    // render spaceship texture
    surface = IMG_Load("images/spaceship.png");
    SDL_Texture *spaceship = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    TTF_Font *sans = TTF_OpenFont("Sans.ttf", 50);
    SDL_Color white = {255, 255, 0};

    char name[100] = "hello";
    SDL_Surface *PointSurface = TTF_RenderText_Solid(sans, name, white);
    if (PointSurface == NULL)
        cout << "point surface is null\n";
    SDL_Texture *PointTexture = SDL_CreateTextureFromSurface(rend, PointSurface);
    SDL_FreeSurface(PointSurface);

    SDL_Color color = {0, 0, 0};
    SDL_Surface *text_surface;
    if (!(text_surface = TTF_RenderText_Solid(sans, "Hello World!", color)))
    {
        // handle error here, perhaps print TTF_GetError at least
    }

    spaceship_class spaceship_game;
    spaceship_game.declare();

    bullet shot[shot_limit];
    asteroid obstacle[asteroid_limit];

    for (int i = 0; i < shot_limit; i++)
    {
        shot[i].dec();
    }

    for (int i = 0; i < asteroid_limit; i++)
    {
        obstacle[i].declare();
    }

    // // The music that will be played
    // Mix_Music *gMusic = NULL;

    // // The sound effects that will be used
    // Mix_Chunk *gScratch = NULL;
    // Mix_Chunk *gHigh = NULL;
    // Mix_Chunk *gMedium = NULL;
    // Mix_Chunk *gLow = NULL;

    while (i)
    {
        SDL_Delay(10);

        SDL_RenderCopy(rend, bg_texture, NULL, NULL);

        spaceship_game.border_limits();

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
                i = 0;
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                case SDLK_q:
                    i = 0;
                    break;
                case SDLK_UP:
                    state[UP] = true;
                    break;
                case SDLK_DOWN:
                    state[DOWN] = true;
                    break;
                case SDLK_RIGHT:
                    state[RIGHT] = true;
                    break;
                case SDLK_LEFT:
                    state[LEFT] = true;
                    break;
                case SDLK_SPACE:
                {
                    state[SPACE] = true;
                };
                }

                {
                    if (state[UP])
                    {
                        spaceship_game.motion_up();
                    }
                    if (state[DOWN])
                    {
                        spaceship_game.motion_down();
                    }
                    if (state[LEFT])
                    {
                        spaceship_game.motion_left();
                    }
                    if (state[RIGHT])
                    {
                        spaceship_game.motion_right();
                    }

                    if (state[SPACE])
                    {
                        for (int i = 0; i < shot_limit; i++)
                        {
                            if (shot[i].turn_on() == true)
                            {
                                shot[i].x = spaceship_game.x;
                                shot[i].y = spaceship_game.y;
                                break;
                            }
                        }
                    }
                }
            }
            else if (e.type == SDL_KEYUP)
            {
                for (int i = 0; i < 5; ++i)
                {
                    state[i] = false;
                }
            }
        }

        // Bullet
        for (int i = 0; i < shot_limit; i++)
        {
            if (shot[i].on == true)
            {
                SDL_Rect a;

                if (shot[i].x >= Width)
                {
                    shot[i].on = false;
                    continue;
                }

                a.x = shot[i].x;
                a.y = shot[i].y + 15;

                a.h = shot[i].height;
                a.w = shot[i].width;

                SDL_RenderCopy(rend, bullet_texture, NULL, &a);
                shot[i].update();
            }
        }

        // Asteroid
        for (int i = 0; i < asteroid_limit; i++)
        {
            SDL_Rect a;

            if (obstacle[i].on == false && obstacle[i].probability_asteroid())
            {
                obstacle[i].on = true;
                obstacle[i].create_asteroid();
            }

            if (obstacle[i].on == true || obstacle[i].explode == true)
            {
                if (obstacle[i].x <= 0)
                {
                    obstacle[i].on = false;
                    continue;
                }

                a.x = obstacle[i].x;
                a.y = obstacle[i].y;

                a.h = obstacle[i].length;
                a.w = obstacle[i].width;

                if (obstacle[i].on)
                    SDL_RenderCopy(rend, asteroid_texture, NULL, &a);

                else if (obstacle[i].explode)
                    SDL_RenderCopy(rend, explosion_texture, NULL, &a);

                obstacle[i].update();
            }

            // check if bullet hits asteroid
            for (int j = 0; j < shot_limit; j++)
            {
                if (shot[j].on == false)
                    continue;
                if (obstacle[i].collide(shot[j]))
                {
                    spaceship_game.IncreasePoint();
                };
            }

            // check if asteroid hit spaceship
            obstacle[i].collide_spaceship(spaceship_game);
        }

        // SDL_RenderCopy(rend, PointTexture, NULL, &ScoreBoardRect);

        if (spaceship_game.on == true)
        {
            SDL_Rect a;
            a.x = spaceship_game.x;
            a.y = spaceship_game.y;

            a.h = spaceship_game.dimension;
            a.w = spaceship_game.dimension;

            SDL_RenderCopy(rend, spaceship, NULL, &a);
        }

        else
            spaceship_game.respawn();

        if (spaceship_game.CheckLives())
        {
            SDL_RenderClear(rend);
            SDL_RenderCopy(rend, GameOver_texture, NULL, NULL);
            SDL_RenderPresent(rend);
            SDL_Delay(3000);

            spaceship_game.on = false;
            spaceship_game.declare();
        }

        else
        {
            SDL_RenderCopy(rend, PointTexture, NULL, &ScoreBoardRect);
            SDL_RenderPresent(rend);
        }

        SDL_RenderClear(rend);
    }

    SDL_DestroyWindow(o);
    SDL_DestroyRenderer(rend);
    SDL_Quit();

    return 0;
}