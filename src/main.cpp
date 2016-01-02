#include <iostream>
#include <SDL/SDL_image.h>
#include "pipera.h"

const int WIDTH = 1500;
const int HEIGHT = 800;

bool running = true;
SDL_Surface *window, *texture;
double X, Y;
double velX, velY, targetX, targetY;
SDL_Event e;

class FunnyWindow : public Pipera::Window
{
public:
    FunnyWindow(size_t w, size_t h) : Window(w, h)
    {
        // nothing
    }

    void onRender()
    {
        SDL_Rect r;
        r.w = surface->w;
        r.h = surface->h;
        r.x = 0;
        r.y = 0;

        SDL_FillRect(surface, &r, Pipera::RGBA(255, 255, 255, 100));

        r.w = surface->w - 40;
        r.h = surface->h - 40;
        r.x = 20;
        r.y = 20;

        SDL_FillRect(surface, &r, Pipera::RGBA(rand(), rand(), rand(), 255));
    }
};

int main()
{
    srand(time(NULL));

    window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_ANYFORMAT);
    SDL_ShowCursor(SDL_DISABLE);
    Pipera::init(window);

    FunnyWindow bottomCenter(600, 100);
    Pipera::Canvas.addWindow(&bottomCenter, {100, 100});


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

                        case SDLK_r:
                        break;

                        default:
                        break;
                    }
                }

                case SDL_MOUSEBUTTONDOWN:
                {
                    if (e.button.button == SDL_BUTTON_RIGHT)
                    {
                    }
                }

                default:
                break;
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

        X += velX / 10;
        Y += velY / 10;

        SDL_Rect r;
        r.w = texture->w;
        r.h = texture->h;
        for (int x = (int(X) % texture->w) - texture->w; x <= window->w; x+= texture->w)
        {
            for (int y = (int(Y) % texture->h) - texture->h; y <= window->h; y += texture->h)
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