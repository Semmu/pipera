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

std::ostream& operator<<(std::ostream& os, Pipera::AABB aabb)
{
    os << "AABB( x: " << aabb.x << " - " << aabb.X << ", y: " << aabb.y << " - " << aabb.Y << " )";
    return os;
}

class Trippy : public Pipera::Window
{
public:
    Trippy(size_t w, size_t h) : Window(w, h) {};

    void onRender()
    {
        SDL_Rect arr;

        arr.x = 0;
        arr.y = 0;
        arr.w = getWidth();
        arr.h = getHeight();
        SDL_FillRect(surface, &arr, Pipera::RGB(rand(), rand(), rand()));

        arr.x = 10;
        arr.y = 10;
        arr.w = getWidth() - 20;
        arr.h = getHeight() - 20;
        SDL_FillRect(surface, &arr, Pipera::getColorKey());

    }
};

int main()
{
    srand(time(NULL));

    window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_ANYFORMAT);
    Pipera::init(window);

    SDL_Surface* textureTemp = IMG_Load("grass.png");
    texture = SDL_DisplayFormat(textureTemp);
    SDL_FreeSurface(textureTemp);

    Trippy t1{800, 100}, t2{150, 150}, t3{350, 500}, t4{400, 100};


    Pipera::Canvas::addWindow(&t1);
    Pipera::Canvas::alignWindow(&t1, Pipera::Pinpointer{0, 0, 0.5, 1}, NULL, Pipera::Pinpointer{0, -20, 0.5, 1});

    Pipera::Canvas::addWindow(&t2);
    Pipera::Canvas::alignWindow(&t2, Pipera::Pinpointer{0, 0, 0, 0}, NULL, Pipera::Pinpointer{20, 20, 0, 0});

    Pipera::Canvas::addWindow(&t3);
    Pipera::Canvas::alignWindow(&t3, Pipera::Pinpointer{0, 0, 1, 0}, NULL, Pipera::Pinpointer{-20, 20, 1, 0});

    Pipera::Canvas::addWindow(&t4);
    Pipera::Canvas::alignWindow(&t4, Pipera::Pinpointer{0, 0, 0, 0.5}, &t2, Pipera::Pinpointer{10, 0, 1, 0.5});

    Pipera::ImageWidget textureW(texture);
    Pipera::OneWidgetWindow oww(&textureW);
    Pipera::Canvas::addWindow(&oww);

    std::cout << Pipera::Canvas::getWindowAABB(&oww);

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
                            t1.toggle();
                            t2.markDirty();
                            t3.markDirty();
                            t4.markDirty();
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

        SDL_Flip(window);
        SDL_Delay(1);
    }

    return 0;
}