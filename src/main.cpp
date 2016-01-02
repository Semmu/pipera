#include <iostream>
#include <iomanip>
#include <SDL/SDL_image.h>
#include "pipera.h"

const int WIDTH = 1500;
const int HEIGHT = 800;

bool running = true;
SDL_Surface *window, *texture;
double X, Y;
double velX, velY, targetX, targetY;
SDL_Event e;

int main()
{
    srand(time(NULL));

    window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_ANYFORMAT);
    Pipera::init(window);

    SDL_Surface* shit = Pipera::Surface::create(300, 180);

    SDL_Surface* textureTemp = IMG_Load("grass.png");
    texture = SDL_DisplayFormat(textureTemp);
    SDL_FreeSurface(textureTemp);

    std::cout << std::hex << Pipera::RGB(127, 127, 127) << std::endl <<
                 SDL_MapRGB(window->format, 127, 127, 127) << std::endl <<
                 Pipera::getColorKey();


    while (running)
    {
        SDL_Rect arr;
        arr.x = 10;
        arr.y = 10;
        arr.w = 280;
        arr.h = 160;
        SDL_FillRect(shit, &arr, Pipera::RGB(rand(), rand(), rand()));

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

        X += velX / 30;
        Y += velY / 30;

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

        r.x = 0;
        r.y = 0;
        r.w = 300;
        r.h = 180;
        SDL_BlitSurface(shit, NULL, window, &r);

        SDL_Flip(window);
        SDL_Delay(1);
    }

    return 0;
}