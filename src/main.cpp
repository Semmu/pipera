#include "pipera.h"
#include <iostream>

const int WIDTH = 1500;
const int HEIGHT = 800;

bool running = true;
SDL_Surface *window;
SDL_Event e;

class FunnyWindow : public Pipera::Window
{
public:
    FunnyWindow(int x, int y, int w, int h) : Window(x, y, w, h)
    {
        return;
    }

    bool render()
    {
        SDL_FillRect(surface, NULL, rand());

        return true;
    }
};

int main()
{
    srand(time(NULL));

    window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_ANYFORMAT);
    Pipera::init(window);
    Pipera::Output.addWindow(new FunnyWindow(100, 50, 100, 50));

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

        SDL_FillRect(window, NULL, 0);

        // the magic
        Pipera::render();

        SDL_Flip(window);
        SDL_Delay(10);
    }

    return 0;
}