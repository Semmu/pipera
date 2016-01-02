#include <vector>
#include <map>

#if USING_SDL1
    #include <SDL/SDL.h>
#else
    #include <SDL2/SDL.h>
#endif

namespace Pipera
{
    struct Position
    {
        int X, Y;

        Position(int x, int y);
    };

    struct Pinpointer : Position
    {
        double width, height;

        Pinpointer(int x = 0, int y = 0, double width = 0.5, double height = 0.5);
    };

    struct AABB : Position
    {
        int x, y;

        AABB(int x, int y);
        AABB(int x, int X, int y, int Y);

        int getWidth() const;
        int getHeight() const;

        bool collides(AABB other) const;
    };



    class Surface
    {
    protected:
        SDL_Surface* surface;

    public:
        Surface(SDL_Surface* given_surface);
        Surface(size_t width, size_t height);

        size_t getWidth() const;
        size_t getHeight() const;

        SDL_Surface* getSurface();

        static SDL_Surface* create(size_t width, size_t height);

        virtual ~Surface();
    };



    class Widget : public Surface
    {
    protected:
        bool dirty;

        Widget* parent;

        // TODO add widget reactsTo and cursor

    public:
        Widget(size_t width, size_t h);

        void setSize(size_t width, size_t height);
        virtual void onResize();

        void render();
        virtual void onRender() = 0;

        void markDirty();
        bool isDirty() const;

        AABB getAABB() const;
    };



    class Window : public Widget
    {
    protected:
        bool hidden;

        // TODO different window types

    public:
        Window(size_t width, size_t height, bool hidden = false);

        bool isHidden() const;
        void hide();
        void show();
        void toggle();
    };



    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC TYPE DEFINITIONS                                       ##
    ###                                                                  ###
    \*####################################################################*/




    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC VARIABLES                                              ##
    ###                                                                  ###
    \*####################################################################*/

    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC METHODS                                                ##
    ###                                                                  ###
    \*####################################################################*/

    namespace Canvas
    {
        void addWindow(Window* window);

        AABB getAABB();
        AABB getWindowAABB(Window* w);

        void alignWindow(Window* window, Pinpointer window_pixel, Window* target, Pinpointer target_pixel);
    }


    void init(SDL_Surface* target);

    Uint32 RGB(int r, int g, int b);
    Uint32 getColorKey();

    void render();
};