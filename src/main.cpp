#include "pipera.h"
#include <iostream>

const int WIDTH = 1500;
const int HEIGHT = 800;

bool running = true;
SDL_Surface *window;
SDL_Event e;

int main()
{
	window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_ANYFORMAT);

	while (running)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
				case SDL_QUIT:
				{
					running = false;
				}
				break;

				case SDL_KEYDOWN:
				{
					switch (e.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = false;
						break;

						case SDLK_q:
							running = false;
						break;

						default:
						break;
					}
				}
			}
		}
	}

	return 0;
}