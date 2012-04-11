#include <SDL.h>
#include <stdio.h>
#include <assert.h>

#include <complex>

#include "font2.h"
#include "Lexer.h"
#include "Parser.h"
#include "ComplexParser.h"

using namespace std;

int width = 800;
int height = 600;


Uint8 getPixel24_R(SDL_Surface *surface, int x, int y)
{
	assert(surface->format->BytesPerPixel == 3);
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 3;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	return p[0];
#else
	return p[2];
#endif
}

Uint8 getPixel24_G(SDL_Surface *surface, int x, int y)
{
	assert(surface->format->BytesPerPixel == 3);
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 3;
	return p[1];
}

Uint8 getPixel24_B(SDL_Surface *surface, int x, int y)
{
	assert(surface->format->BytesPerPixel == 3);
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 3;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	return p[2];
#else
	return p[0];
#endif
}


void blendPixel24(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b, float a)
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

void addpixel24(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b, float a)
{
	assert(surface->format->BytesPerPixel == 3);
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 3;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	p[0] += (int)(r * a);
	p[1] += (int)(g * a);
	p[2] += (int)(b * a);
#else
	p[2] += (int)(r * a);
	p[1] += (int)(g * a);
	p[0] += (int)(b * a);
#endif
}

class FractalMatrix
{
private:
	int width, height;
	double *distance, *argument;
	bool* out;
	d_complex* zLast;
	int depthMax;
	double x0, y0, scale, bailOutRadius;
	SDL_Surface* surf;
	SDL_Surface* tex;
	SDL_Surface* tex_mask;

	ParserTree<d_complex>* parser;
public:
	FractalMatrix(int width, int height) : width(width), height(height)
	{
		distance = new double[width * height];
		argument = new double[width * height];
		out = new bool[width * height];
		zLast = new d_complex[width * height];
	}
	double getDistance(int x, int y) { return distance[width * y + x]; }
	double getArgument(int x, int y) { return argument[width * y + x]; }
	bool isOut(int x, int y) { return out[width * y + x]; }
	int getDepthMax() { return depthMax; }
	d_complex getZLast(int x, int y) { return zLast[width * y + x]; }

	void start(ParserTree<d_complex>& parser, double x0, double y0, double scale, double bailOutRadius, SDL_Surface* surf, SDL_Surface* tex, SDL_Surface* tex_mask)
	{
		this->surf = surf;
		this->tex = tex;
		this->tex_mask = tex_mask;
		this->x0 = x0;
		this->y0 = y0;
		this->scale = scale;
		this->bailOutRadius = bailOutRadius;
		this->parser = &parser;

		for (int i = 0; i < width * height; i++)
		{
			distance[i] = 1e+30; argument[i] = 0;
			out[i] = 0; zLast[i] = 0;
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

					z_var.setValue(zLast[width * j + i]);
					c_var.setValue(c);
					int k = 0;
					for (; k < steps; k++)
					{
						z_var.setValue(parser->execute());
						zLast[width * j + i] = z_var.getValue();

						// Minimizing the distance
						double dist = abs(zLast[width * j + i] - d_complex(0));
						double phi = arg(zLast[width * j + i] - d_complex(0));

						//if (dist < distance[width * j + i])
						{
							distance[width * j + i] = dist;

/*		            	Uint8 r = 255 * sin(phi)* sin(phi);
		            	Uint8 g = 255 * cos(phi)* cos(phi);
	            		Uint8 b = 0;*/

						dist = dist * (tex->w + tex->h) / 2;// * log(depthMax + 1);
						/*if (dist > 300) dist = 300;
						if (dist < 0) dist = 300;*/

		            	int x = 110 + dist * cos(phi);
		            	int y = 152 + dist * sin(phi);
		            	if (x < 0) x = 0;
		            	if (y < 0) y = 0;
		            	if (x > tex->w - 1) x = tex->w - 1;
		            	if (y > tex->h - 1) y = tex->h - 1;

		            	Uint8 r = getPixel24_R(tex, x, y);
		            	Uint8 g = getPixel24_G(tex, x, y);
		            	Uint8 b = getPixel24_B(tex, x, y);

		            	float a = (float)getPixel24_R(tex_mask, x, y) / 255;
		            	if (a < 0) a = 0;
		            	if (a > 1) a = 1;

		            	blendPixel24(surf, i, j, r, g, b, a);

						/*
						if (dist < distance[width * j + i])
						{
							distance[width * j + i] = dist;
							argument[width * j + i] = phi;
						}*/
						}


						if (abs(zLast[width * j + i]) > bailOutRadius)
						{
							out[width * j + i] = true;
							newBailOuts++;
						}
					}

	        	}
	        }
	    }
	    depthMax += steps;
	    return 1000;//newBailOuts;
	}

	~FractalMatrix()
	{
		delete [] distance;
		delete [] zLast;
		delete [] out;
	}
};

bool quit_pending = false;

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

void put_symbol(SDL_Surface *surface, wchar_t symbol, int x, int y, Uint8 font[], int symbol_w, int symbol_h, wchar_t encoding[], Uint8 r, Uint8 g, Uint8 b, int oversize)
{
	// Searching the symbol code in the current encoding
	int code = 0;
	for (unsigned int k = 0; k < wcslen(encoding); k++)
	{
		if (encoding[k] == symbol) { code = k; break; }
	}

	// Drawing it
	for (int j = 0; j < symbol_h; j++)
	for (int i = 0; i < symbol_w; i++)
	{
		if (font[j * symbol_w + i + (symbol_w * symbol_h) * code] == '#')
		{
			addpixel24(surface, (x + i) / oversize, (y + j) / oversize, r, g, b, 1.0 / oversize / oversize);
		}
	}
}

void put_string(SDL_Surface *surface, string str, int x, int y, Uint8 font[], int symbol_w, int symbol_h, int distance, wchar_t encoding[], Uint8 r, Uint8 g, Uint8 b, int oversize)
{
	int xc = x;
	for (int i = 0; i < str.length(); i++)
	{
		put_symbol(surface, str[i], xc, y, font, symbol_w, symbol_h, encoding, r, g, b, oversize);
		xc += symbol_w + distance;
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

    SDL_WM_SetCaption("Simplex", "Simplex");

    atexit(SDL_Quit);
    printf("Starting UI event loop.\n");

    SDL_Surface* screen;
    screen = SDL_SetVideoMode(width, height, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if(screen == NULL)
    {
    	printf("Could not set video mode: %d", SDL_GetError());
    	return 1;
    }

    string eq = "z^2+c";

    LexerTree lex(eq);
    lex.doLexing();

    ParserTree<d_complex> parser(lex, ComplexParser());

    FractalMatrix fracMat(width, height);

    SDL_Surface* tex = SDL_LoadBMP("eye.bmp");
    SDL_Surface* tex_mask = SDL_LoadBMP("eye_mask.bmp");
    SDL_LockSurface(tex);
    SDL_LockSurface(tex_mask);
    fracMat.start(parser, -0.5, 0, 0.3, 2, screen, tex, tex_mask);

    quit_pending = false;
    printf("Starting UI event loop.\n");
    bool success = false;
    int step = 0;

    while( !quit_pending )
    {
        process_events();

        if (!success)
        {
            SDL_Rect rct;
            rct.x = 0; rct.y = 0; rct.w = width; rct.h = height;
        	//SDL_FillRect(screen, &rct, 0xFFFFFF);
        	success = (fracMat.process(1) < 50);
        }

        char ttl[128];
        sprintf(ttl, "Mandelbrot - Simplex [Equation: %s, Steps: %d]", eq.c_str(), fracMat.getDepthMax());

        SDL_WM_SetCaption(ttl, ttl);
/*

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
            	double dist = fracMat.getDistance(i, j);// / fracMat.getDepthMax();
            	double phi = fracMat.getArgument(i, j);// / fracMat.getDepthMax();

            }
        }*/
        //put_string(screen, "Mandelbrot", 20, 20, font, symbol_w, symbol_h, 2, encoding, 255, 255, 255, 1);
        //put_string(screen, ttl, 40, 40 + 2 * symbol_h + 6, font, symbol_w, symbol_h, 2, encoding, 255, 255, 255, 2);
        //put_string(screen, "Mandelbrot set", 30, 30, font, symbol_w, symbol_h, 2, encoding, 255, 255, 255, 2);

        step ++;
        SDL_Delay(10);
        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }
}
