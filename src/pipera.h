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
        bool transparent;

    public:
        IDrawable(int w = 0, int h = 0, bool t = false);
        virtual ~IDrawable();

        int getWidth() const;
        int getHeight() const;
        SDL_Surface* getSurface();
        bool getTransparent();

        virtual bool render() = 0;
    };



    class Widget : public IDrawable
    {
    protected:
        int X, Y;
        Widget* parent;
        bool dirty;

    public:
        Widget(int w = 0, int h = 0, int x = 0, int y = 0);

        int getX() const;
        int getY() const;

        void setX(int x = 0);
        void setY(int y = 0);

        int getGlobalX() const;
        int getGlobalY() const;

        bool isDirty() const;
        void markDirty();
    };



    class IContainer
    {
    public:
        virtual ~IContainer();

        virtual bool attachWidget(Widget* w) = 0;
        virtual bool detachWidget(Widget* w) = 0;
    };



    class Window : public Widget
    {
    protected:
        enum class Type
        {
            NORMAL,

        } type;

    public:
        Window(int w = 100, int h = 100, int x = 0, int y = 0);
    };



    class OutputClass : public Window
    {
    private:
        std::vector<Window*> windows;
        SDL_Surface* targetSurface;
        int transparentColor;

    public:
        OutputClass();
        bool init(SDL_Surface* target);

        virtual bool addWindow(Window* w);

        bool render();

        int getTransparentColor();
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

    int RGBA(int r, int g, int b, int a = SDL_ALPHA_OPAQUE);
};