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

        std::map<Window*, Position> windows;
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

    int AABB::getWidth() const
    {
        return X - x;
    }

    int AABB::getHeight() const
    {
        return Y - y;
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

        SDL_SetColorKey(surface, SDL_SRCCOLORKEY, colorKey);
        SDL_FillRect(surface, NULL, colorKey);


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

        SDL_FillRect(surface, NULL, colorKey);

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

    FixedSizeContainer::FixedSizeContainer(size_t w, size_t h, Widget* c, double x, double y) : Widget{w, h}, xalign{x}, yalign{y}, child{c}
    {
        // nothing
    }

    void FixedSizeContainer::onRender()
    {
        if (child->isDirty())
            child->render();

        SDL_Rect srcr, destr;

        srcr.x = 0;
        srcr.y = 0;
        srcr.w = child->getWidth();
        srcr.h = child->getHeight();

        if (getWidth() < child->getWidth())
            srcr.x = (child->getWidth() - getWidth()) * xalign;

        if (getHeight() < child->getHeight())
            srcr.y = (child->getHeight() - getHeight()) * yalign;

        destr.x = (getWidth() - child->getWidth()) * xalign;
        destr.y = (getHeight() - child->getHeight()) * yalign;
        destr.w = child->getWidth();
        destr.h = child->getHeight();


        SDL_BlitSurface(child->getSurface(), &srcr, surface, &destr);
    }


    PaddingContainer::PaddingContainer(Widget* w, size_t t, size_t r, size_t b, size_t l) : Widget{0, 0}, top{t}, right{r}, bottom{b}, left{l}, child{w}
    {
        surface = Surface::create(left + w->getWidth() + right, top + w->getHeight() + bottom);
        // TODO set child's parent!!!
    }

    PaddingContainer::PaddingContainer(Widget* w, size_t t) : PaddingContainer{w, t, t, t, t}
    {
        // nothing
    }

    PaddingContainer::PaddingContainer(Widget* w, size_t t, size_t r) : PaddingContainer{w, t, r, t, r}
    {
        // nothing
    }

    PaddingContainer::PaddingContainer(Widget* w, size_t t, size_t r, size_t b) : PaddingContainer{w, t, r, b, r}
    {
        // nothing
    }

    void PaddingContainer::onRender()
    {
        if (child->isDirty())
            child->render();

        // FIXME what if the child changed its size?

        SDL_FillRect(surface, NULL, colorKey);

        SDL_Rect r;
        r.x = left;
        r.y = top;
        r.w = child->getWidth();
        r.h = child->getHeight();

        SDL_BlitSurface(child->getSurface(), NULL, surface, &r);
    }

    DecoratedContainer::DecoratedContainer(Widget* w, SDL_Surface* decor, size_t t, size_t r, size_t b, size_t l) : PaddingContainer{w, t, r, b, l}, decoration{decor}
    {
        // TODO set child's parent!!!

        if (size_t(decor->w) < l + 1 + r || size_t(decor->h) < t + 1 + b)
        {
            // TODO throw exception, if the decoration is smaller than the padding
        }
    }

    DecoratedContainer::DecoratedContainer(Widget* w, SDL_Surface* decor, size_t t) : DecoratedContainer{w, decor, t, t, t, t}
    {
        // nothing
    }

    DecoratedContainer::DecoratedContainer(Widget* w, SDL_Surface* decor, size_t t, size_t r) : DecoratedContainer{w, decor, t, r, t, r}
    {
        // nothing
    }

    DecoratedContainer::DecoratedContainer(Widget* w, SDL_Surface* decor, size_t t, size_t r, size_t b) : DecoratedContainer{w, decor, t, r, b, r}
    {
        // nothing
    }

    void DecoratedContainer::onRender()
    {
        // FIXME the whole decoration and pattern blitting process could be optimized likely a lot

        if (child->isDirty())
            child->render();

        // FIXME what if the child changed its size?

        SDL_FillRect(surface, NULL, colorKey);

        SDL_Rect decorr, destr;


        // top left corner
        decorr.x = 0;
        decorr.y = 0;
        decorr.w = left;
        decorr.h = top;
        destr.x = 0;
        destr.y = 0;
        destr.w = left;
        destr.h = top;
        SDL_BlitSurface(decoration, &decorr, surface, &destr);

        // top right corner
        decorr.x = decoration->w - right;
        decorr.y = 0;
        decorr.w = right;
        decorr.h = top;
        destr.x = surface->w - right;
        destr.y = 0;
        destr.w = right;
        destr.h = top;
        SDL_BlitSurface(decoration, &decorr, surface, &destr);


        // bottom left corner
        decorr.x = 0;
        decorr.y = decoration->h - bottom;
        decorr.w = left;
        decorr.h = bottom;
        destr.x = 0;
        destr.y = surface->h - bottom;
        destr.w = left;
        destr.h = bottom;
        SDL_BlitSurface(decoration, &decorr, surface, &destr);


        // bottom right corner
        decorr.x = decoration->w - right;
        decorr.y = decoration->h - bottom;
        decorr.w = right;
        decorr.h = bottom;
        destr.x = surface->w - right;
        destr.y = surface->h - bottom;
        destr.w = right;
        destr.h = bottom;
        SDL_BlitSurface(decoration, &decorr, surface, &destr);


        // center pattern

        int decorw = decoration->w - left - right;
        int destw = surface->w - left - right;

        int decorh = decoration->h - top - bottom;
        int desth = surface->h - top - bottom;


        int xcount = 0;
        int ycount = 0;

        while (ycount * decorh < desth)
        {
            xcount = 0;

            // left border
            decorr.x = 0;
            decorr.y = top;
            decorr.w = right;
            if (ycount * decorh <= desth - decorh)
                decorr.h = decorh;
            else
                decorr.h = desth % decorh;

            destr.x = 0;
            destr.y = top + ycount * decorh;
            destr.w = right;
            destr.h = decorh;

            SDL_BlitSurface(decoration, &decorr, surface, &destr);



            // right border
            decorr.x = decoration->w - right;
            decorr.y = top;
            decorr.w = right;
            if (ycount * decorh <= desth - decorh)
                decorr.h = decorh;
            else
                decorr.h = desth % decorh;

            destr.x = surface->w - right;
            destr.y = top + ycount * decorh;
            destr.w = right;
            destr.h = decorh;

            SDL_BlitSurface(decoration, &decorr, surface, &destr);



            while (xcount * decorw < destw)
            {
                // this is the first row, so we draw the top and bottom borders
                if (ycount == 0)
                {
                    // top border
                    decorr.x = left;
                    decorr.y = 0;
                    decorr.h = top;
                    if (xcount * decorw <= destw - decorw)
                        decorr.w = decorw;
                    else
                        decorr.w = destw % decorw;

                    destr.x = left + xcount * decorw;
                    destr.y = 0;
                    destr.w = decorw;
                    destr.h = top;

                    SDL_BlitSurface(decoration, &decorr, surface, &destr);

                    // bottom border
                    decorr.x = left;
                    decorr.y = top + decorh;
                    decorr.h = bottom;
                    if (xcount * decorw <= destw - decorw)
                        decorr.w = decorw;
                    else
                        decorr.w = destw % decorw;

                    destr.x = left + xcount * decorw;
                    destr.y = surface->h - bottom;
                    destr.w = decorw;
                    destr.h = bottom;

                    SDL_BlitSurface(decoration, &decorr, surface, &destr);
                }

                // center pattern
                destr.x = left + xcount * decorw;
                destr.y = top + ycount * decorh;
                destr.w = decorr.w;
                destr.h = decorr.h;

                decorr.x = left;
                decorr.y = top;
                if (ycount * decorh <= desth - decorh)
                    decorr.h = decorh;
                else
                    decorr.h = desth % decorh;

                if (xcount * decorw <= destw - decorw)
                    decorr.w = decorw;
                else
                    decorr.w = destw % decorw;

                SDL_BlitSurface(decoration, &decorr, surface, &destr);

                xcount++;
            }

            ycount++;
        }



        // the child widget
        destr.x = left;
        destr.y = top;
        destr.w = child->getWidth();
        destr.h = child->getHeight();
        SDL_BlitSurface(child->getSurface(), NULL, surface, &destr);
    }



    ImageWidget::ImageWidget(SDL_Surface* image) : Widget{0, 0}
    {
        surface = SDL_DisplayFormat(image);
        dirty = false; // FIXME this is not safe, because marking the widget dirty will erase the image
    }

    void ImageWidget::onRender()
    {
        //SDL_FillRect(surface, NULL, RGB(rand(), rand(), 0));
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



    OneWidgetWindow::OneWidgetWindow(Widget* ch) : Window{0, 0}
    {
        // TODO set child's parent!!!

        child = ch;
        surface = Surface::create(child->getWidth(), child->getHeight());
    }

    void OneWidgetWindow::onRender()
    {
        if (child->isDirty())
            child->render();

        SDL_BlitSurface(child->getSurface(), NULL, surface, NULL);
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

    void Canvas::addWindow(Window* w)
    {
        windows.insert(std::make_pair(w, Position{0, 0}));
    }

    AABB Canvas::getAABB()
    {
        return AABB{0, target->w, 0, target->h};
    }

    AABB Canvas::getWindowAABB(Window *w)
    {
        // window not added to canvas, TODO throw exception
        if (windows.find(w) == windows.end())
            return AABB{0, 0, 0, 0};

        Window* win = windows.find(w)->first;
        Position pos = windows.find(w)->second;

        return AABB{pos.X, int(win->getWidth()) + pos.X, pos.Y, int(win->getHeight()) + pos.Y};
    }

    void Canvas::alignWindow(Window* w, Pinpointer wp, Window* t, Pinpointer tp)
    {
        // TODO throw exception if *w is not in windows
        if (windows.find(w) == windows.end())
            return;

        AABB waabb = Canvas::getWindowAABB(w), taabb{0, 0};

        if (t == NULL)
            taabb = Canvas::getAABB();
        else
            taabb = Canvas::getWindowAABB(t);

        int wpx = waabb.x + wp.X + (waabb.getWidth() * wp.width);
        int wpy = waabb.y + wp.Y + (waabb.getHeight() * wp.height);

        int tpx = taabb.x + tp.X + (taabb.getWidth() * tp.width);
        int tpy = taabb.y + tp.Y + (taabb.getHeight() * tp.height);

        int dx = tpx - wpx;
        int dy = tpy - wpy;

        windows.find(w)->second.X += dx;
        windows.find(w)->second.Y += dy;
    }

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

        for (auto window : windows)
        {
            if (window.first->isHidden())
                continue;

            window.first->render();

            SDL_Rect r;
            r.w = window.first->getWidth();
            r.h = window.first->getHeight();
            r.x = window.second.X;
            r.y = window.second.Y;

            SDL_BlitSurface(window.first->getSurface(), NULL, canvas, &r);
        }

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