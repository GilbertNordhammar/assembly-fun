#include <iostream>
#include "SDL3/SDL.h"
//#include <SDL3/SDL_main.h>

extern "C" int __cdecl GetSum(int a, int b, int c, int d, int e)
{
    return a + b + c + d + e;
}

extern "C" void __cdecl HelloWorldCStyle()
{
    std::cout << "Hello World from outside ASM with C-style!" << std::endl;
}

void __cdecl HelloWorldCPlusPlusStyle()
{
    std::cout << "Hello World from outside ASM C++ style!" << std::endl;
}

namespace SomeNamespace
{
    void Foo()
    {
        std::cout << "SomeNamespace::Foo()" << std::endl;
    }
}

extern "C" void __cdecl InitSDLCPlusPlus()
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

void main()
{
    InitSDL();

    //InitSDL();
}