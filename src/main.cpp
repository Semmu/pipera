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
        SDL_Rect r;
        r.w = surface->w;
        r.h = surface->h;
        r.x = 0;
        r.y = 0;

        SDL_FillRect(surface, &r, Pipera::RGBA(255, 0, 0, 100));

        r.w = surface->w - 40;
        r.h = surface->h - 40;
        r.x = 20;
        r.y = 20;

        SDL_FillRect(surface, &r, Pipera::RGBA(255, 0, 0, 255));

        return true;
    }
};

int main()
{
    srand(time(NULL));

    window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_ANYFORMAT);
    Pipera::init(window);
    Pipera::Output.addWindow(new FunnyWindow(100, 50, 100, 50));
    Pipera::Output.addWindow(new FunnyWindow(500, 500, 200, 200));

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

        SDL_FillRect(window, NULL, Pipera::RGBA(0, 50, rand() % 20 + 175));

        // the magic
        Pipera::render();

        SDL_Flip(window);
        SDL_Delay(100);
    }

    return 0;
}