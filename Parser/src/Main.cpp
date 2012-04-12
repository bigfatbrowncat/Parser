#include <SDL.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>

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
	double *level;
	bool* out;
	d_complex* zLast;
	int depthMax;
	double x0, y0, scale, bailOutRadius;

	ParserTree<d_complex>* parser;

protected:
	FractalMatrix& operator = (const FractalMatrix& other)
	{
		// Never use it!
	}
public:

	FractalMatrix(int width, int height) : width(width), height(height)
	{
		level = new double[width * height];
		out = new bool[width * height];
		zLast = new d_complex[width * height];
	}
	double getLevel(int x, int y) { return level[width * y + x]; }
	bool isOut(int x, int y) { return out[width * y + x]; }
	int getDepthMax() { return depthMax; }
	d_complex getZLast(int x, int y) { return zLast[width * y + x]; }

	void scaleUp(int k)
	{
		int nw = width * k, nh = height * k;

		double* nLevel = new double[nw * nh];
		bool* nOut = new bool[nw * nh];
		d_complex* nZLast = new d_complex[nw * nh];

		for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			for (int p = 0; p < k; p++)
			for (int q = 0; q < k; q++)
			{
				int ii = k * i + p;
				int jj = k * j + q;

				nLevel[nw * jj + ii] = level[j * width + i];
				nOut[nw * jj + ii] = false;//out[j * width + i];
				nZLast[nw * jj + ii] = zLast[j * width + i];
			}
		}

		delete [] level;
		delete [] out;
		delete [] zLast;

		width = nw; height = nh;
		level = nLevel;
		out = nOut;
		zLast = nZLast;
	}

	void start(ParserTree<d_complex>& parser, double x0, double y0, double scale, double bailOutRadius)
	{
		this->x0 = x0;
		this->y0 = y0;
		this->scale = scale;
		this->bailOutRadius = bailOutRadius;
		this->parser = &parser;

		for (int i = 0; i < width * height; i++)
		{
			level[i] = 0;
			out[i] = 0;
			zLast[i] = 0;
		}
		depthMax = 0;
	}

	double i_to_cx(int i)
	{
		int avg_size = (width + height) / 2;
		return (i - width  / 2) / scale / avg_size + x0;
	}
	double j_to_cy(int j)
	{
		int avg_size = (width + height) / 2;
		return (j - height / 2) / scale / avg_size + y0;
	}
	int cx_to_i(double cx)
	{
		int avg_size = (width + height) / 2;
		return (cx - x0) * avg_size * scale + width / 2;
	}
	int cy_to_j(double cy)
	{
		int avg_size = (width + height) / 2;
		return (cy - y0) * avg_size * scale + height / 2;
	}

	// Returns the new bail-outs. Can be used to determine how "successful" it is
	int process(int steps)
	{
		int newBailOuts = 0;
	    ParserVariable<d_complex>& z_var = parser->getVariable("z");
	    ParserVariable<d_complex>& c_var = parser->getVariable("c");
		int avg_size = (width + height) / 2;

		omp_set_num_threads(8);
		#pragma omp for
        for (int j = 0; j < height; j++)
	    {
    		for (int i = 0; i < width; i++)
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

						if (abs(z_var.getValue()) > bailOutRadius)
						{
							out[width * j + i] = true;
							#pragma omp critical
							newBailOuts++;
						}

						level[width * j + i] = abs(zLast[width * j + i]);

					}

	        	}
	        }
	    }
		#pragma omp barrier
	    depthMax += steps;
	    return newBailOuts;
	}

	~FractalMatrix()
	{
		delete [] level;
		delete [] zLast;
		delete [] out;
	}
};

bool quit_pending = false;
double xc = 0, yc = 0, zoom = 0.3;
bool success = false;
bool run_started = false;
int scale_up = 1;
int scale_down = 8;
string eq = "z^3+c^(3/2)";
LexerTree lex(eq);
ParserTree<d_complex> parser(lex, ComplexParser());
FractalMatrix* fracMat;

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
		case SDL_MOUSEBUTTONDOWN:
			zoom *= 2;
			xc = fracMat->i_to_cx(event.button.x);
			yc = fracMat->j_to_cy(event.button.y);
			success = false;
			run_started = false;
		    fracMat->start(parser, xc, yc, zoom, 3);
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
    if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
    	printf("Could not initialize SDL: %d", SDL_GetError());
    	return 1;
    }

    SDL_WM_SetCaption("Simplex", "Simplex");

    atexit(SDL_Quit);

    SDL_Surface* screen;
    screen = SDL_SetVideoMode(width, height, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if(screen == NULL)
    {
    	printf("Could not set video mode: %d", SDL_GetError());
    	return 1;
    }

    int runaways_to_success = 200.0 / (640 * 480) * width * height;

    fracMat = new FractalMatrix(scale_up * width / scale_down, scale_up * height / scale_down);
    fracMat->start(parser, xc, yc, zoom, 3);

    quit_pending = false;
    printf("Starting UI event loop.\n");

    while( !quit_pending )
    {
        process_events();

        if (!success || !run_started)
        {
        	int runaways = fracMat->process(1);
        	if (runaways > runaways_to_success) run_started = true;

        	if (run_started) success = (runaways < 100);
        }
        else
        {
        	// Painting the success
    	    SDL_Rect rct;
    	    rct.x = 0; rct.y = 0; rct.w = width; rct.h = height;
    		SDL_FillRect(screen, &rct, 0x0000AA);

    		for (int i = 0; i < width / scale_down; i++)
    		{
    			for (int j = 0; j < height / scale_down; j++)
    			{
    				double a = 1;
    				double r = 0, g = 0, b = 0;

    				for (int p = 0; p < scale_up; p++)
    				{
    					for (int q = 0; q < scale_up; q++)
    					{
    						//if (!fracMat->isOut(scale_up * i + p, scale_up * j + q))
//    							a += 1.0 / scale_up / scale_up;
  //  						else
    						{
    							double w = 1 - exp(-fracMat->getLevel(scale_up * i + p, scale_up * j + q));
    							r += w * 255 / scale_up / scale_up;
    							g += w * 255 / scale_up / scale_up;
    							b += w * 255 / scale_up / scale_up;
    						}

    					}
    				}

    				for (int p = 0; p < scale_down; p++)
    				{
    					for (int q = 0; q < scale_down; q++)
    					{
    						blendPixel24(screen, i * scale_down + p, j * scale_down + q, (int)r, (int)g, (int)b, 1);
    					}
    				}
    			}
    		}

        	if (scale_down > 1)
        	{
        		scale_down /= 2;
        		delete fracMat;
        		fracMat = new FractalMatrix(scale_up * width / scale_down, scale_up * height / scale_down);
        	    fracMat->start(parser, xc, yc, zoom, 3);
        		success = false;
        		run_started = false;
        	}
        	else
        	{
        		if (scale_up < 2)
        		{
        			scale_up *= 2;
            		delete fracMat;
            		fracMat = new FractalMatrix(scale_up * width / scale_down, scale_up * height / scale_down);
            	    fracMat->start(parser, xc, yc, zoom, 3);
            		success = false;
            		run_started = false;
        		}
        	}
        }


        char ttl[128];
        sprintf(ttl, "Mandelbrot - Simplex [Equation: %s, Steps: %d]", eq.c_str(), fracMat->getDepthMax());

        SDL_WM_SetCaption(ttl, ttl);

        SDL_Delay(10);
        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }
}
