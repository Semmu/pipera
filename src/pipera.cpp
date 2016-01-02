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
        if (surface)
            return surface->w;

        return 0;
    }

    int IDrawable::getHeight() const
    {
        if (surface)
            return surface->h;

        return 0;
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

     AABB::AABB(int _x, int _y) : x(_x), X(_x), y(_y), Y(_y)
     {
        // nothing
     }

     AABB::AABB(int _x, int _X, int _y, int _Y) : x(_x), X(_X), y(_y), Y(_Y)
     {
        // nothing
     }

     bool AABB::doesOverlapWith(AABB aabb)
     {
        // its MUCH easier to check if they definitely do not overlap

        if (X < aabb.x)
            return false;

        if (aabb.X < x)
            return false;

        if (Y < aabb.y)
            return false;

        if (aabb.Y < y)
            return false;

        // if none of the previous conditions were true, they must overlap
        return true;
     }


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

    AABB Widget::getAABB() const
    {
        return {getGlobalX(), getGlobalX() + getWidth(),
                getGlobalY(), getGlobalY() + getHeight()};
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


    PaddingContainer::PaddingContainer(Widget* c, int t, int r, int b, int l) :
        Widget( c->getWidth() + ( l ? l : ( r ? r : t) ) + ( r ? r : t) ,
                c->getHeight() + t + ( b ? b : t) )
    {
        // nothing
    }

    bool PaddingContainer::render()
    {
        SDL_Rect r;
        r.w = surface->w;
        r.h = surface->h;
        r.x = 0;
        r.y = 0;

        SDL_FillRect(surface, &r, Pipera::RGBA(0, 0, 0, 255));

        r.w = child->getSurface()->w;
        r.h = child->getSurface()->h;
        r.x = left;
        r.y = top;

        SDL_BlitSurface(child->getSurface(), NULL, surface, &r);

        dirty = false;

        return true;
    }


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


    CursorClass::CursorClass() : Window(0, 0, 0, 0)
    {
        // nothing
    }

    void CursorClass::autosetLocation()
    {
        SDL_GetMouseState(&X, &Y);
    }

    bool CursorClass::render()
    {
        return true;
    }



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

    bool OutputClass::processClick(SDL_Event* e)
    {
        AABB cursor(Cursor.getX(), Cursor.getY());

        for(auto w : windows)
        {
            if (cursor.doesOverlapWith(w->getAABB()))
                w->markDirty();
        }

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
    ##      PRIVATE TYPES                                                 ##
    ###                                                                  ###
    \*####################################################################*/



    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC VARIABLES                                              ##
    ###                                                                  ###
    \*####################################################################*/

    OutputClass Output;
    CursorClass Cursor;

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

    bool processClick(SDL_Event* e)
    {
        return Output.processClick(e);
    }

    bool render()
    {
        Cursor.autosetLocation();
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