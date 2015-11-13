#include "pipera.h"
#include <iostream>

const int WIDTH = 1500;
const int HEIGHT = 800;

bool running = true;
SDL_Surface *window;
SDL_Event e;

int main()
{
    srand(time(NULL));

    window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_ANYFORMAT);
    Pipera::init(window);

    while (running)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
                case SDL_QUIT:
                {
                    running = false;
                }
                break;

                case SDL_KEYDOWN:
                {
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            running = false;
                        break;

                        case SDLK_q:
                            running = false;
                        break;

                        default:
                        break;
                    }
                }
            }
        }

        // to simulate a running app
        SDL_FillRect(window, NULL, rand());

        // the magic
        Pipera::render();

        SDL_Flip(window);
        SDL_Delay(400);
    }

    return 0;
}