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

SDL_Surface* loadimg(const char* filename)
{
    SDL_Surface* temp = IMG_Load(filename);
    SDL_Surface* better = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    return better;
}

int main()
{
    srand(time(NULL));

    window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_ANYFORMAT);
    Pipera::init(window);

    SDL_Surface* texture = loadimg("grass.png");

    Trippy t1{800, 100}, t2{150, 150}, t3{350, 500}, t4{400, 100};


    Pipera::Canvas::addWindow(&t1);
    Pipera::Canvas::alignWindow(&t1, Pipera::Pinpointer{0, 0, 0.5, 1}, NULL, Pipera::Pinpointer{0, -20, 0.5, 1});

    Pipera::Canvas::addWindow(&t2);
    Pipera::Canvas::alignWindow(&t2, Pipera::Pinpointer{0, 0, 0, 0.5}, NULL, Pipera::Pinpointer{20, 0, 0, 0.5});

    Pipera::Canvas::addWindow(&t3);
    Pipera::Canvas::alignWindow(&t3, Pipera::Pinpointer{0, 0, 1, 0}, NULL, Pipera::Pinpointer{-20, 20, 1, 0});

    Pipera::Canvas::addWindow(&t4);
    Pipera::Canvas::alignWindow(&t4, Pipera::Pinpointer{0, 0, 0, 0.5}, &t2, Pipera::Pinpointer{10, 0, 1, 0.5});

    SDL_Surface* decor = loadimg("debug_border.bmp");
    SDL_Surface* bng_decor = loadimg("brown_and_gray_border.bmp");


    Pipera::ImageWidget textureW(bng_decor);
    Pipera::PaddingContainer pc(&textureW, 20);
    Pipera::DecoratedContainer dc(&pc, decor, 6, 12, 19, 7);
    Pipera::OneWidgetWindow oww(&dc);
    Pipera::Canvas::addWindow(&oww);
    Pipera::Canvas::alignWindow(&oww, Pipera::Pinpointer{0, 0, 0, 0}, NULL, Pipera::Pinpointer{50, 50, 0, 0});



    Pipera::ImageWidget textureW2(bng_decor);
    Pipera::PaddingContainer pc2(&textureW2, 0);
    Pipera::DecoratedContainer dc2(&pc2, bng_decor, 6);
    Pipera::OneWidgetWindow oww2(&dc2);
    Pipera::Canvas::addWindow(&oww2);
    Pipera::Canvas::alignWindow(&oww2, Pipera::Pinpointer{0, 0, 0.5, 0}, NULL, Pipera::Pinpointer{0, 20, 0.5, 0});



    SDL_Surface* red_button_decor = loadimg("red_button.bmp");

    SDL_Surface* newgamesurf = loadimg("text_new_game.bmp");
    Pipera::ImageWidget newgameimagewidget(newgamesurf);
    Pipera::PaddingContainer newgamepc(&newgameimagewidget, 0, 15);
    Pipera::DecoratedContainer newgamebtn(&newgamepc, red_button_decor, 15);

    Pipera::PaddingContainer menupadding(&newgamebtn, 20, 20, 50);
    Pipera::DecoratedContainer menudecor(&menupadding, bng_decor, 6);

    Pipera::OneWidgetWindow menuwin(&menudecor);
    Pipera::Canvas::addWindow(&menuwin);

    Pipera::Canvas::alignWindow(&menuwin, Pipera::Pinpointer{0, 0, 0.5, 0.5}, NULL, Pipera::Pinpointer{0, 0, 0.5, 0.5});


    SDL_Surface* loadgamesurf = loadimg("text_load_game.bmp");
    Pipera::ImageWidget loadgameimagewidget(loadgamesurf);
    Pipera::PaddingContainer loadgamepc(&loadgameimagewidget, 0, 15);
    Pipera::DecoratedContainer loadgamebtn(&loadgamepc, red_button_decor, 15);
    Pipera::OneWidgetWindow loadgamewin(&loadgamebtn);
    Pipera::Canvas::addWindow(&loadgamewin);
    Pipera::Canvas::alignWindow(&loadgamewin, Pipera::Pinpointer{0, 0, 0.5, 0}, &menuwin, Pipera::Pinpointer{0, 50, 0.5, 0});


    SDL_Surface* settingssurf = loadimg("text_settings.bmp");
    Pipera::ImageWidget settingsimagewidget(settingssurf);
    Pipera::PaddingContainer settingspc(&settingsimagewidget, 0, 15);
    Pipera::DecoratedContainer settingsbtn(&settingspc, red_button_decor, 15);
    Pipera::OneWidgetWindow settingswin(&settingsbtn);
    Pipera::Canvas::addWindow(&settingswin);
    Pipera::Canvas::alignWindow(&settingswin, Pipera::Pinpointer{0, 0, 0.5, 0}, &menuwin, Pipera::Pinpointer{0, 100, 0.5, 0});



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
        SDL_Delay(10);
    }

    return 0;
}