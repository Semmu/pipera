#include <vector>
#if USING_SDL1
    #include <SDL/SDL.h>
#else
    #include <SDL2/SDL.h>
#endif

namespace Pipera
{
    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC TYPE DEFINITIONS                                       ##
    ###                                                                  ###
    \*####################################################################*/

    class IDrawable
    {
    protected:
        SDL_Surface* surface;

    public:
        IDrawable(int w = 0, int h = 0);

        virtual int getWidth() const;
        virtual int getHeight() const;
        virtual SDL_Surface* getSurface();
        virtual bool render() = 0;
    };

    // at least for now
    typedef IDrawable Widget;

    class IContainer : public IDrawable
    {
    public:
        virtual bool attachWidget(Widget* w) = 0;
        virtual bool detachWidget(Widget* w) = 0;
    };

    class Window : public IDrawable
    {
    private:
        int X, Y;

    public:
        Window(int x = 0, int y = 0, int w = 100, int h = 100);

        virtual int getX();
        virtual int getY();
    };

    class OutputClass : public IDrawable
    {
    private:
        std::vector<Window*> windows;

    public:
        OutputClass(SDL_Surface* given_surface);

        virtual bool addWindow(Window* w);

        bool init(SDL_Surface* s);
        bool render();
    };

    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC VARIABLES                                              ##
    ###                                                                  ###
    \*####################################################################*/

    extern OutputClass Output;


    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC METHODS                                                ##
    ###                                                                  ###
    \*####################################################################*/

    bool init(SDL_Surface* target);

    bool render();
};