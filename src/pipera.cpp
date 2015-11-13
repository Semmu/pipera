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

        Uint32 rmask, gmask, bmask, amask;
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            rmask = 0xff000000;
            gmask = 0x00ff0000;
            bmask = 0x0000ff00;
            amask = 0x000000ff;
        #else
            rmask = 0x000000ff;
            gmask = 0x0000ff00;
            bmask = 0x00ff0000;
            amask = 0xff000000;
        #endif

        surface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
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
        SDL_Rect r;
        r.w = 200;
        r.h = 200;
        r.x = 200;
        r.y = 200;
        SDL_FillRect(Output.getSurface(), &r, 0);

        return true;
    }

    /*####################################################################*\
    ###                                                                  ###
    ##      PRIVATE METHODS                                               ##
    ###                                                                  ###
    \*####################################################################*/
}