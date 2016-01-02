#include <iostream>
#include <utility>
#include "pipera.h"

namespace Pipera
{
    namespace
    {
        Uint32 colorKey;
        SDL_Surface* target;
        SDL_Surface* canvas;
    }


    Position::Position(int x, int y) : X{x}, Y{y}
    {
        // nothing
    }

    Pinpointer::Pinpointer(int x, int y, double w, double h) : Position{x, y}, width{w}, height{h}
    {
        // nothing
    }

    AABB::AABB(int _x, int _y) : Position{_x, _y}, x{_y}, y{_y}
    {
        // nothing
    }

    AABB::AABB(int _x, int _X, int _y, int _Y) : Position{_X, _Y}, x(_x), y(_y)
    {
        if (x > X)
            std::swap(x, X);

        if (y > Y)
            std::swap(y, Y);
    }

    bool AABB::collides(AABB other) const
    {
        // its MUCH easier to check if they definitely do not overlap

        if (X < other.x)
            return false;

        if (other.X < x)
            return false;

        if (Y < other.y)
            return false;

        if (other.Y < y)
            return false;

        // if none of the previous conditions were true, they must overlap
        return true;
    }


    Surface::Surface(SDL_Surface* given) : surface{given}
    {
        // nothing
    }

    Surface::Surface(size_t w, size_t h) : surface{Surface::create(w, h)}
    {
        // nothing
    }

    size_t Surface::getWidth() const
    {
        if (surface)
            return surface->w;

        return 0;
    }

    size_t Surface::getHeight() const
    {
        if (surface)
            return surface->h;

        return 0;
    }

    SDL_Surface* Surface::getSurface()
    {
        return surface;
    }

    SDL_Surface* Surface::create(size_t w, size_t h)
    {
        if (w == 0 || h == 0)
            return NULL;

        SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
        SDL_Surface* surface = SDL_DisplayFormat(temp);
        SDL_FreeSurface(temp);

        SDL_FillRect(surface, NULL, colorKey);

        SDL_SetColorKey(surface, SDL_SRCCOLORKEY, colorKey);

        return surface;
    }

    Surface::~Surface()
    {
        if (surface)
            SDL_FreeSurface(surface);
    }



    Widget::Widget(size_t w, size_t h) : Surface{w, h}, dirty{true}, parent{NULL}
    {
        // nothing
    }


    void Widget::setSize(size_t w, size_t h)
    {
        if (surface)
            SDL_FreeSurface(surface);

        surface = Surface::create(w, h);
        markDirty();
    }

    void Widget::onResize()
    {
        // nothing? FIXME do we even need this?
    }

    void Widget::render()
    {
        /*if (!dirty)
            return;*/

        SDL_FillRect(surface, NULL, RGB(rand(), rand(), rand()));

        onRender();
        dirty = false;
    }

    void Widget::markDirty()
    {
        dirty = true;

        if (parent)
            parent->markDirty();
    }

    bool Widget::isDirty() const
    {
        return dirty;
    }

    AABB Widget::getAABB() const
    {
        // TODO compute global AABB
        return AABB{0, 0, int(getWidth()), int(getHeight())};
    }







    Window::Window(size_t w, size_t h, bool hid) : Widget{w, h}, hidden{hid}
    {
        // nothing
    }

    bool Window::isHidden() const
    {
        return hidden;
    }

    void Window::hide()
    {
        hidden = true;
    }

    void Window::show()
    {
        hidden = false;
    }

    void Window::toggle()
    {
        hidden = !hidden;
    }




    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC TYPE DEFINITIONS                                       ##
    ###                                                                  ###
    \*####################################################################*/



    /*   *\
    \#\ /#/     IDRAWABLE
     \###/
      \*/

      /*\
     /###\
    /#/ \#\     IDRAWABLE
    \*   */






    /*   *\
    \#\ /#/     WIDGET
     \###/
      \*/

      /*\
     /###\
    /#/ \#\     WIDGET
    \*   */


    /*   *\
    \#\ /#/     WINDOW
     \###/
      \*/

      /*\
     /###\
    /#/ \#\     WINDOW
    \*   */


    /*   *\
    \#\ /#/     OUTPUTCLASS
     \###/
      \*/



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

    void init(SDL_Surface* t)
    {
        if (canvas)
            SDL_FreeSurface(canvas);

        target = t;

        colorKey = RGB(255, 0, 255);

        canvas = Surface::create(t->w, t->h);
    }

    Uint32 getColorKey()
    {
        return colorKey;
    }

    void render()
    {
        SDL_FillRect(canvas, NULL, colorKey);

        SDL_Rect r;
        r.w = 100 + rand() % 100;
        r.h = 100 + rand() % 100;

        r.x = rand() % (target->w - r.w);
        r.y = rand() % (target->h - r.h);

        SDL_FillRect(canvas, &r, RGB(255, 255, 255));

        SDL_BlitSurface(canvas, NULL, target, NULL);
    }

    Uint32 RGB(int r, int g, int b)
    {
        return SDL_MapRGB(target->format, r, g, b);
    }

    /*####################################################################*\
    ###                                                                  ###
    ##      PRIVATE METHODS                                               ##
    ###                                                                  ###
    \*####################################################################*/
}