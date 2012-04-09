#include <SDL.h>
#include <stdio.h>
#include <assert.h>

#include <complex>

#include "Lexer.h"
#include "Parser.h"
#include "ComplexParser.h"

using namespace std;

int width = 800;
int height = 600;

class FractalMatrix
{
private:
	int width, height;
	int* data;
	bool* out;
	d_complex* zLast;
	int depthMax;
	double x0, y0, scale, bailOutRadius;

	ParserTree<d_complex>* parser;
public:
	FractalMatrix(int width, int height) : width(width), height(height)
	{
		data = new int[width * height];
		out = new bool[width * height];
		zLast = new d_complex[width * height];
	}
	int getData(int x, int y) { return data[width * y + x]; }
	int getDepthMax() { return depthMax; }
	d_complex getZLast(int x, int y) { return zLast[width * y + x]; }

	void start(ParserTree<d_complex>& parser, double x0, double y0, double scale, double bailOutRadius)
	{
		this->x0 = x0;
		this->y0 = y0;
		this->scale = scale;
		this->bailOutRadius = bailOutRadius;
		this->parser = &parser;

		for (int i = 0; i < width * height; i++)
		{
			data[i] = 0; out[i] = 0; zLast[i] = 0;
		}
		depthMax = 0;
	}

	// Returns the new bail-outs. Can be used to determine how "successful" it is
	int process(int steps)
	{
		int newBailOuts = 0;
	    ParserVariable<d_complex>& z_var = parser->getVariable("z");
	    ParserVariable<d_complex>& c_var = parser->getVariable("c");
		int avg_size = (width + height) / 2;

		for (int i = 0; i < width; i++)
	    {
	        for (int j = 0; j < height; j++)
	        {
	        	if (!out[width * j + i])
	        	{
					d_complex c((i - width  / 2) / scale / avg_size + x0,
					            (j - height / 2) / scale / avg_size + y0);

					if (abs(c) > bailOutRadius)
					{
						out[width * j + i] = true;
						newBailOuts++;
					}
					else
					{
						z_var.setValue(zLast[width * j + i]);
						c_var.setValue(c);
						int k = 0;
						for (; k < steps; k++)
						{
							z_var.setValue(parser->execute());
							if (abs(z_var.getValue()) > bailOutRadius)
							{
								out[width * j + i] = true;
								newBailOuts++;
								break;
							}
						}
						zLast[width * j + i] = z_var.getValue();
						data[width * j + i] += k;
					}
	        	}
	        }
	    }
	    depthMax += steps;
	    return newBailOuts;
	}

	~FractalMatrix()
	{
		delete [] data;
		delete [] zLast;
		delete [] out;
	}
};

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
    if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
    	printf("Could not initialize SDL: %d", SDL_GetError());
    	return 1;
    }

    SDL_WM_SetCaption("Mandelbrot", "Mandelbrot");

    atexit(SDL_Quit);
    printf("Starting UI event loop.\n");

    SDL_Surface* screen;
    screen = SDL_SetVideoMode(width, height, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if(screen == NULL)
    {
    	printf("Could not set video mode: %d", SDL_GetError());
    	return 1;
    }

    LexerTree lex("z^2+c");
    lex.doLexing();

    ParserTree<d_complex> parser(lex, ComplexParser());

    FractalMatrix fracMat(width * 2, height * 2);
    fracMat.start(parser, -0.5, 0, 0.3, 2);

    quit_pending = false;
    printf("Starting UI event loop.\n");
    bool success = false;
    while( !quit_pending )
    {
        process_events();

        if (!success)
        {
        	success = (fracMat.process(1) < 50);
        }

        char ttl[128];
        sprintf(ttl, "Mandelbrot (steps: %d)", fracMat.getDepthMax());

        SDL_WM_SetCaption(ttl, "Mandelbrot");

        for (int i = 0; i < width * 2; i++)
        {
            for (int j = 0; j < height * 2; j++)
            {
            	int depth = fracMat.getData(i, j);
            	int depthMax = fracMat.getDepthMax();
            	int v = 255 * (1.0 - 1.0 / pow(1.05, depth));
            	if (depth == depthMax) v = 255;
      			addpixel24(screen, i / 2, j / 2, v, v, v, 0.25);
            }
        }
        SDL_Delay(10);
        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }
}
