#include "pipera.h"

namespace Pipera
{
    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC TYPE DEFINITIONS                                       ##
    ###                                                                  ###
    \*####################################################################*/

    IDrawable::IDrawable(int w, int h)
    {
        if (w == 0 || h == 0)
            return;

        surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h,
                                       Output.getSurface()->format->BitsPerPixel,
                                       Output.getSurface()->format->Rmask,
                                       Output.getSurface()->format->Gmask,
                                       Output.getSurface()->format->Bmask,
                                       Output.getSurface()->format->Amask);
    }

    int IDrawable::getWidth() const
    {
        return surface->w;
    }

    int IDrawable::getHeight() const
    {
        return surface->w;
    }

    SDL_Surface* IDrawable::getSurface()
    {
        return surface;
    }

    Window::Window(int x, int y, int w, int h) : IDrawable(w, h), X(x), Y(y)
    {
        return;
    }

    int Window::getX()
    {
        return X;
    }

    int Window::getY()
    {
        return Y;
    }


    OutputClass::OutputClass(SDL_Surface* given_surface = NULL)
    {
        return; // lol
    }

    bool OutputClass::init(SDL_Surface* s)
    {
        if (surface != NULL)
            return false;

        surface = s;
        return true;
    }

    bool OutputClass::render()
    {
        SDL_Rect r;
        for (auto w : windows)
        {
            w->render();

            r.x = w->getX();
            r.y = w->getY();
            r.w = w->getWidth();
            r.h = w->getHeight();

            SDL_BlitSurface(w->getSurface(), NULL, surface, &r);
        }

        return true;
    }

    bool OutputClass::addWindow(Window* w)
    {
        windows.push_back(w);
        return true;
    }


    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC VARIABLES                                              ##
    ###                                                                  ###
    \*####################################################################*/

    OutputClass Output;

    /*####################################################################*\
    ###                                                                  ###
    ##      PRIVATE VARIABLES                                             ##
    ###                                                                  ###
    \*####################################################################*/



    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC METHODS                                                ##
    ###                                                                  ###
    \*####################################################################*/


    bool init(SDL_Surface* target)
    {
        return Output.init(target);
    }

    bool render()
    {
        return Output.render();
    }

    /*####################################################################*\
    ###                                                                  ###
    ##      PRIVATE METHODS                                               ##
    ###                                                                  ###
    \*####################################################################*/
}