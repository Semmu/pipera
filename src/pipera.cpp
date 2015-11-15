#include <iostream>
#include "pipera.h"

namespace Pipera
{
    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC TYPE DEFINITIONS                                       ##
    ###                                                                  ###
    \*####################################################################*/



    /*   *\
    \#\ /#/     IDRAWABLE
     \###/
      \*/

    IDrawable::IDrawable(int w, int h, bool t) : surface(NULL), transparent(t)
    {
        if (w == 0 || h == 0)
            return;

        if (Output.getSurface() == NULL)
            exit(2);

        SDL_Surface* surfaceTemp = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
        surface = SDL_DisplayFormatAlpha(surfaceTemp);
        SDL_FreeSurface(surfaceTemp);

        SDL_SetAlpha(surface, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
    }

    IDrawable::~IDrawable()
    {
        if (surface)
            SDL_FreeSurface(surface);
    }

    int IDrawable::getWidth() const
    {
        return surface->w;
    }

    int IDrawable::getHeight() const
    {
        return surface->h;
    }

    SDL_Surface* IDrawable::getSurface()
    {
        return surface;
    }

      /*\
     /###\
    /#/ \#\     IDRAWABLE
    \*   */






    /*   *\
    \#\ /#/     WIDGET
     \###/
      \*/

    Widget::Widget(int w, int h, int x, int y) : IDrawable(w, h), X(x), Y(y), parent(NULL), dirty(true)
    {
        // nothing
    }

    int Widget::getX() const
    {
        return X;
    }

    int Widget::getY() const
    {
        return Y;
    }

    int Widget::getGlobalX() const
    {
        if (parent)
            return parent->getX() + X;

        return X;
    }

    int Widget::getGlobalY() const
    {
        if (parent)
            return parent->getY() + Y;

        return Y;
    }

    bool Widget::isDirty() const
    {
        return dirty;
    }

    void Widget::markDirty()
    {
        if (parent)
            parent->markDirty();

        dirty = true;
    }

      /*\
     /###\
    /#/ \#\     WIDGET
    \*   */


    PixelPicker::PixelPicker(float w, float h, int x, int y) : W(w), H(h), X(x), Y(y)
    {
        // nothing
    }



    /*   *\
    \#\ /#/     WINDOW
     \###/
      \*/

    Window::Window(int w, int h, int x, int y) : Widget(w, h, x, y)
    {
        return;
    }

    void Window::alignTo(Window* target, PixelPicker subjectPixel, PixelPicker targetPixel)
    {
        int subjextPixelGlobalX = getGlobalX() + ( getWidth() * subjectPixel.W ) + subjectPixel.X;
        int subjextPixelGlobalY = getGlobalY() + ( getHeight() * subjectPixel.H ) + subjectPixel.Y;

        int targetPixelGlobalX = target->getGlobalX() + ( target->getWidth() * targetPixel.W ) + targetPixel.X;
        int targetPixelGlobalY = target->getGlobalY() + ( target->getHeight() * targetPixel.H ) + targetPixel.Y;

        X += (targetPixelGlobalX - subjextPixelGlobalX);
        Y += (targetPixelGlobalY - subjextPixelGlobalY);
    }

      /*\
     /###\
    /#/ \#\     WINDOW
    \*   */






    /*   *\
    \#\ /#/     OUTPUTCLASS
     \###/
      \*/

    OutputClass::OutputClass() : Window(0, 0, 0, 0), targetSurface(NULL), transparentColor(0)
    {
        return; // lol
    }

    bool OutputClass::init(SDL_Surface* target)
    {
        if (targetSurface != NULL)
            return false;

        SDL_Surface* surfaceTemp = SDL_CreateRGBSurface(SDL_SWSURFACE, target->w, target->h, 32, 0, 0, 0, 0);
        surface = SDL_DisplayFormatAlpha(surfaceTemp);
        SDL_FreeSurface(surfaceTemp);

        targetSurface = target;

        transparentColor = SDL_MapRGBA(target->format, 0, 0, 0, SDL_ALPHA_TRANSPARENT);

        return true;
    }

    bool OutputClass::addWindow(Window* w)
    {
        windows.push_back(w);
        return true;
    }

    bool OutputClass::render()
    {
        SDL_Rect r;
        for (auto w : windows)
        {
            if (w->isDirty())
                w->render();

            r.x = w->getX();
            r.y = w->getY();
            r.w = w->getWidth();
            r.h = w->getHeight();

            SDL_BlitSurface(w->getSurface(), NULL, targetSurface, &r);
        }

        return true;
    }

    int OutputClass::getTransparentColor()
    {
        return transparentColor;
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

    int RGBA(int r, int g, int b, int a)
    {
        return SDL_MapRGBA(Output.getSurface()->format, r, g, b, a);
    }

    /*####################################################################*\
    ###                                                                  ###
    ##      PRIVATE METHODS                                               ##
    ###                                                                  ###
    \*####################################################################*/
}