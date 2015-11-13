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
    };

    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC VARIABLES                                              ##
    ###                                                                  ###
    \*####################################################################*/

    class OutputClass : public IDrawable
    {
    public:
        OutputClass(SDL_Surface* given_surface);

        bool init(SDL_Surface* s);
    };

    extern OutputClass Output;


    /*####################################################################*\
    ###                                                                  ###
    ##      PUBLIC METHODS                                                ##
    ###                                                                  ###
    \*####################################################################*/

    bool init(SDL_Surface* target);

    bool render();
};