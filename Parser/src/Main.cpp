#include <SDL.h>
#include <stdio.h>
#include <assert.h>

#include <complex>

using namespace std;

int width = 640;
int height = 480;

bool quit_pending = false;

void addpixel24(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b, float a)
{
	assert(surface->format->BytesPerPixel == 3);
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 3;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	p[0] = (int)(p[0] * (1 - a) + r * a);
	p[1] = (int)(p[1] * (1 - a) + g * a);
	p[2] = (int)(p[2] * (1 - a) + b * a);
#else
	p[2] = (int)(p[2] * (1 - a) + r * a);
	p[1] = (int)(p[1] * (1 - a) + g * a);
	p[0] = (int)(p[0] * (1 - a) + b * a);
#endif
}

void process_events()
{
	/* Our SDL event placeholder. */
	SDL_Event event;
	bool down = false;

	/* Grab all the events off the queue. */
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit_pending = true;
			}
			break;
		case SDL_KEYUP:
			break;
		case SDL_QUIT:
			/* Handle quit requests (like Ctrl-c). */
			quit_pending = true;
			break;
		}
	}
}

int main(int argc, char* argv[])
{
    printf("Starting UI event loop.\n");
    fflush(stdout);
    if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
    {
    	printf("Could not initialize SDL: %d", SDL_GetError());
    	return 1;
    }

    atexit(SDL_Quit);
    printf("Starting UI event loop.\n");

    SDL_Surface* screen;
    screen = SDL_SetVideoMode(width, height, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if(screen == NULL)
    {
    	printf("Could not set video mode: %d", SDL_GetError());
    	return 1;
    }

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
        	complex<double> c((double)(i - 320) / 150, (double)(j - 240) / 150);
        	complex<double> z(0, 0);
        	int k = 0;
        	for (; k < 30; k++)
        	{
        		z = z * z + c;
        		if (abs(z) > 10) break;
        	}
        	addpixel24(screen, i, j, 255 * k / 30, 255 * k / 30, 255 * k / 30, 1);
        }
    }

    SDL_UpdateRect(screen, 0, 0, 0, 0);

    quit_pending = false;
    printf("Starting UI event loop.\n");
    while( !quit_pending ) {
        /* Process incoming events. */
        process_events( );
    }
}
