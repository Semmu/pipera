#include <iostream>
#include <utility>
#include "pipera.h"

namespace Pipera
{
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

        // FIXME the next part is probably wrong and unnecessary
        // but tbh i dont understand the SDL1 docs about alpha blending and
        // its working

        SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, w, h, 32, 0, 0, 0, 0);
        SDL_Surface* surface = SDL_DisplayFormatAlpha(temp);
        SDL_FreeSurface(temp);

        SDL_SetAlpha(surface, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);

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
        if (!dirty)
            return;

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
        return AABB{0, 0, getWidth(), getHeight()};
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



    CanvasClass::CanvasClass() : Widget{0, 0}, target{NULL}
    {
        // nothing
    }

    SDL_Surface* CanvasClass::getTargetSurface()
    {
        return target;
    }

    void CanvasClass::init(SDL_Surface* t)
    {
        target = t;

        if (surface)
            SDL_FreeSurface(surface);

        surface = Surface::create(target->w, target->h);
        markDirty();
    }

    void CanvasClass::addWindow(Window* w, Position pos)
    {
        windows.insert(std::make_pair(w, pos));
    }

    void CanvasClass::alignWindow(Window* w, Pinpointer wp, Window* t, Pinpointer tp)
    {
        // TODO
    }

    void CanvasClass::onRender()
    {
        SDL_FillRect(surface, NULL, RGBA(0, 0, 0, 255));

        SDL_Rect r;
        for (auto w : windows)
        {
            if (w.first->isDirty())
                w.first->render();

            r.x = w.second.X;
            r.y = w.second.Y;
            r.w = w.first->getWidth();
            r.h = w.first->getHeight();

            SDL_BlitSurface(w.first->getSurface(), NULL, surface, &r);
        }
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

    CanvasClass Canvas;

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
        Canvas.init(target);
        return true;
    }

    bool processClick(SDL_Event* e)
    {
        //return Canvas.processClick(e);
    }

    bool render()
    {
        //Cursor.autosetLocation();
        Canvas.render();

        return true;
    }

    int RGBA(int r, int g, int b, int a)
    {
        return SDL_MapRGBA(Canvas.getTargetSurface()->format, r, g, b, a);
    }

    /*####################################################################*\
    ###                                                                  ###
    ##      PRIVATE METHODS                                               ##
    ###                                                                  ###
    \*####################################################################*/
}