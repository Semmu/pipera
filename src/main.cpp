#include <iostream>
#include <SDL/SDL_image.h>
#include "pipera.h"

const int WIDTH = 1500;
const int HEIGHT = 800;

bool running = true;
SDL_Surface *window, *texture;
int X, Y;
double velX, velY, targetX, targetY;
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

        SDL_FillRect(surface, &r, Pipera::RGBA(255, 0, 0, rand()));

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

    SDL_Surface* textureTemp = IMG_Load("grass.png");
    texture = SDL_DisplayFormat(textureTemp);
    SDL_FreeSurface(textureTemp);

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

        if (abs(velX - targetX) < 0.3 && abs(velY - targetY) < 0.3)
        {
            targetX = rand() % 200 - 100;
            targetY = rand() % 200 - 100;
        }
        else
        {
            velX += (targetX - velX) / 100;
            velY += (targetY - velY) / 100;
        }

        X += velX / 40;
        Y += velY / 40;

        SDL_Rect r;
        r.w = texture->w;
        r.h = texture->h;
        for (int x = (X % texture->w) - texture->w; x <= window->w; x+= texture->w)
        {
            for (int y = (Y % texture->h) - texture->h; y <= window->h; y += texture->h)
            {
                r.x = x;
                r.y = y;
                SDL_BlitSurface(texture, NULL, window, &r);
            }
        }


        // the magic
        Pipera::render();

        SDL_Flip(window);
        SDL_Delay(1);
    }

    return 0;
}