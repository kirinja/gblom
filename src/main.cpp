#include <iostream>
#include "SDL2/SDL.h"
#include "cpu.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main()
{
    CPU cpu;

    // cpu.run();

    // std::cout << "Hello World!" << std::endl;
    SDL_Window *window = nullptr;
    SDL_Surface *surface = nullptr;
    int res = SDL_Init(SDL_INIT_VIDEO);
    if (res < 0)
    {
        printf("Error initializing SDL. SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr)
        {
            printf("Error creating window. SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            surface = SDL_GetWindowSurface(window);
            SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 0x0B, 0xFF, 0xFF));
            SDL_UpdateWindowSurface(window);

            SDL_Event e;
            bool quit = false;
            while (!quit)
            {
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
                        quit = true;
                }
                cpu.step();
            }
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
    }
    return EXIT_SUCCESS;
}