#include <iostream>
#include "SDL3/SDL.h"

extern "C" void InitSDLCPlusPlus()
{
    const int WIDTH = 640;
    const int HEIGHT = 480;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Hello SDL", WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

extern "C" void InitSDL();

//void main()
//{
//    InitSDLCPlusPlus();
//    //InitSDL();
//}