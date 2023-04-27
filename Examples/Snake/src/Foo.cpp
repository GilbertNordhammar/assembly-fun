#include <iostream>
#include "SDL3/SDL.h"
#include <Windows.h>

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

extern "C" SDL_Texture * Foo(SDL_Renderer * renderer, Uint32 format, int access, int w, int h)
{
    return nullptr;
}

//void main()
//{
//    void* ptr = malloc(10);
//    free(ptr);
//
//    InitSDLCPlusPlus();
//    //InitSDL();
//}

int mainz(int argc, char** argv)
{
    bool leftMouseButtonDown = false;
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL2 Pixel Drawing", 640, 480, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 480);
    Uint32* pixels = new Uint32[640 * 480];

    memset(pixels, 255, 640 * 480 * sizeof(Uint32));

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    while (!quit)
    {
        SDL_UpdateTexture(texture, NULL, pixels, 640 * sizeof(Uint32));

        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            quit = true;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button == SDL_BUTTON_LEFT)
                leftMouseButtonDown = false;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
                leftMouseButtonDown = true;
        case SDL_EVENT_MOUSE_MOTION:
            if (leftMouseButtonDown)
            {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;
                pixels[mouseY * 640 + mouseX] = 0;
            }
            break;
        }

        SDL_RenderClear(renderer);
        //SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}