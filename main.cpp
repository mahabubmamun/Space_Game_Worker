#include "Functions/motion.hpp"
#include "Functions/values.hpp"
#include "Functions/Initialize.hpp"

int main(int argv, char **args)
{
    // Initializations
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 5240);

    SDL_Event e;

    SDL_Window *o = SDL_CreateWindow("Space_Game",
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     Width,
                                     Height,
                                     SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    SDL_Renderer *rend = SDL_CreateRenderer(o, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int i = 1;

    Mix_Music *music = Mix_LoadMUS("images/music1.mp3");

    if (music == NULL)
    {
        cout << "no music";
    }

    Mix_PlayMusic(music, -1);

    // Corner Point text

    SDL_Rect ScoreBoardRect;
    CreateRect(ScoreBoardRect, 50, 50, 0, 0);

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

    TTF_Font *font = TTF_OpenFont("roboto.ttf", 24);
    SDL_Color white = {255, 255, 255, 255};

    spaceship_class spaceship_game;
    spaceship_game.declare();

    bullet shot[shot_limit];
    asteroid obstacle[asteroid_limit];

    for (int i = 0; i < shot_limit; i++)
    {
        shot[i].declare();
    }

    for (int i = 0; i < asteroid_limit; i++)
    {
        obstacle[i].declare();
    }

    SDL_Rect SpaceshipRect;
    SpaceshipRect.h = spaceship_game.dimension;
    SpaceshipRect.w = spaceship_game.dimension;

    while (i)
    {
        SDL_Delay(15);

        SDL_RenderCopy(rend, bg_texture, NULL, NULL);

        spaceship_game.border_limits();

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
                i = 0;

            else if (e.type == SDL_MOUSEMOTION)
            {
                spaceship_game.x = e.motion.x;
                spaceship_game.y = e.motion.y;

                SpaceshipRect.x = spaceship_game.x;
                SpaceshipRect.y = spaceship_game.y;
            }

            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {

                case SDLK_SPACE:
                {
                    for (int i = 0; i < shot_limit; i++)
                    {
                        if (shot[i].on == false and shot[0].reload == false)
                        {
                            shot[i].turn_on();
                            shot[i].x = spaceship_game.x;
                            shot[i].y = spaceship_game.y;
                            break;
                        }

                        else if (i == shot_limit - 1 and shot[0].reload == false)
                        {
                            shot[0].reload = true;
                            shot[0].relaod_start = SDL_GetTicks();
                        }
                    }

                    break;
                };

                case SDLK_ESCAPE:
                {
                    i = 0;
                    break;
                };
                }
            }
        }

        // Bullet
        for (int i = 0; i < shot_limit; i++)
        {
            if (shot[0].reload == true)
            {
                if (SDL_GetTicks() - shot[0].relaod_start >= shot[0].reload_time)
                {
                    shot[0].reload = false;

                    for (int i = 0; i < shot_limit; i++)
                    {
                        shot[i].on = false;
                    }
                }
            }

            if (shot[i].on == true)
            {
                SDL_Rect a;

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
            obstacle[i].update();
        }

        char points[10];
        char lives[10];

        if (spaceship_game.on == true)
        {
            // Printing Score
            SDL_Rect ScoreBoardRect;
            CreateRect(ScoreBoardRect, 50, 120, 0, 0);
            surface = TTF_RenderText_Solid(font, spaceship_game.PointString(points), white);
            SDL_Texture *PointTexture = SDL_CreateTextureFromSurface(rend, surface);
            SDL_RenderCopy(rend, PointTexture, NULL, &ScoreBoardRect);
            SDL_FreeSurface(surface);

            // Printing Lives
            SDL_Rect LivesDisplayRect;
            CreateRect(LivesDisplayRect, 50, 100, 400, 0);
            surface = TTF_RenderText_Solid(font, spaceship_game.LivesDispayString(lives), white);
            SDL_Texture *LivesTexture = SDL_CreateTextureFromSurface(rend, surface);
            SDL_RenderCopy(rend, LivesTexture, NULL, &LivesDisplayRect);
            SDL_FreeSurface(surface);

            // Reloading
            if (shot[0].reload == true)
            {
                SDL_Rect ReloadingReqt;
                CreateRect(ReloadingReqt, 50, 100, 800, 0);
                surface = TTF_RenderText_Solid(font, "RELOADING...", white);
                SDL_Texture *ReloadingTexture = SDL_CreateTextureFromSurface(rend, surface);
                SDL_RenderCopy(rend, ReloadingTexture, NULL, &ReloadingReqt);
                SDL_FreeSurface(surface);
            }
        }

        else
            spaceship_game.respawn();

        if (spaceship_game.CheckLives())
        {
            SDL_Delay(3000);
            SDL_RenderClear(rend);
            SDL_RenderCopy(rend, GameOver_texture, NULL, NULL);
            SDL_RenderPresent(rend);
            SDL_Delay(3000);

            spaceship_game.on = false;
            spaceship_game.declare();

            for (int i = 0; i < shot_limit; i++)
            {
                shot[i].declare();
            }

            for (int i = 0; i < asteroid_limit; i++)
            {
                obstacle[i].declare();
            }
        }

        else
        {
            SDL_RenderCopy(rend, spaceship, NULL, &SpaceshipRect);
            SDL_RenderPresent(rend);
        }

        SDL_RenderClear(rend);
    }

    SDL_DestroyWindow(o);
    SDL_DestroyRenderer(rend);
    TTF_Quit();
    SDL_Quit();

    return 0;
}