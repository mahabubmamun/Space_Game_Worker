#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

// class Init
// {
// public:
//     SDL_Window *window = SDL_Window *o = SDL_CreateWindow("Space_Game",
//                                                           SDL_WINDOWPOS_UNDEFINED,
//                                                           SDL_WINDOWPOS_UNDEFINED,
//                                                           Width,
//                                                           Height,
//                                                           SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

//     SDL_Renderer *rend = SDL_CreateRenderer(o, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
// };

void CreateRect(SDL_Rect &a, int l1, int w1, int x1, int y1)
{
    a.x = x1;
    a.y = y1;
    a.h = l1;
    a.w = w1;
}