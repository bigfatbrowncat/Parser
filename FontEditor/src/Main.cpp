#include <SDL.h>
#include <stdio.h>
#include <assert.h>

#include <string>

using namespace std;

int width = 800;
int height = 600;

bool quit_pending = false;

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

void addPixel24(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b, float a)
{
	assert(surface->format->BytesPerPixel == 3);
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 3;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	int p0 = p[0] + (int)(r * a);
	int p1 = p[1] + (int)(g * a);
	int p2 = p[2] + (int)(b * a);
	p[0] = p0 < 255 ? p0 : 255;
	p[1] = p1 < 255 ? p1 : 255;
	p[2] = p2 < 255 ? p2 : 255;
#else
	int p2 = p[2] + (int)(r * a);
	int p1 = p[1] + (int)(g * a);
	int p0 = p[0] + (int)(b * a);
	p[2] = p2 < 255 ? p2 : 255;
	p[1] = p1 < 255 ? p1 : 255;
	p[0] = p0 < 255 ? p0 : 255;
#endif
}

void blendFrameFilled(SDL_Surface *surface, int x1, int x2, int y1, int y2, Uint8 r, Uint8 g, Uint8 b, float a)
{
	for (int x = x1; x <= x2; x++)
	for (int y = y1; y <= y2; y++)
	{
		blendPixel24(surface, x, y, r, g, b, a);
	}
}

void blendFrame(SDL_Surface *surface, int x1, int x2, int y1, int y2, Uint8 r, Uint8 g, Uint8 b, float a)
{
	for (int x = x1; x <= x2; x++)
	{
		blendPixel24(surface, x, y1, r, g, b, a);
		blendPixel24(surface, x, y2, r, g, b, a);
	}
	for (int y = y1; y <= y2; y++)
	{
		blendPixel24(surface, x1, y, r, g, b, a);
		blendPixel24(surface, x2, y, r, g, b, a);
	}
}



int drawing_box_x = 35;
int drawing_box_y = 70;

int char_view_length_half = 25;
int char_view_x = 15;
int char_view_y = 10;

int symbol_preview_x = 387;
int symbol_preview_y = 70;

int drawing_box_frame_width = 5;
int symbol_preview_frame_width = 5;

int symbol_w = 10;
int symbol_h = 16;

int cell_width = 10;
int oversize = 3;

wchar_t* encoding = L" "
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                     "abcdefghijklmnopqrstuvwxyz"
                     "0123456789.,:;!?@#$%^&*()[]{}_-+<>=~\"'`/\\|";
int encoding_size = 97;

bool** symbol;

bool painting = false;
bool base_mode = true;

enum color { black, white } painting_color;
int painting_cell_x = -1, painting_cell_y = -1;
int current_char = 0;

void draw(SDL_Surface* surface);
void process_events();

void allocSymbols()
{
    symbol = new bool*[encoding_size];

    for (int ch = 0; ch < encoding_size; ch++)
    {
    	symbol[ch] = new bool[symbol_w * oversize * symbol_h * oversize];
    	for (int i = 0; i < symbol_w * oversize * symbol_h * oversize; i++)
    		symbol[ch][i] = false;
    }
}

void freeSymbols()
{
    for (int ch = 0; ch < encoding_size; ch++)
    {
    	delete [] symbol[ch];
    }
    delete [] symbol;
}

bool scanSymbols(string filename)
{
	FILE* f = fopen(filename.c_str(), "r");
	if (f != NULL)
	{
		fscanf(f, "%d %d %d", &symbol_w, &symbol_h, &oversize);
		fgetc(f);
	}
	allocSymbols();

	if (f != NULL)
	{
		for (int ch = 0; ch < encoding_size; ch++)
		{
			for (int j = 0; j < symbol_h * oversize; j++)
			{
				for (int i = 0; i < symbol_w * oversize; i++)
				{
					char c = fgetc(f);
					symbol[ch][j * symbol_w * oversize + i] = (c == '#');
				}
				fgetc(f);
			}
			fgetc(f);
		}
		fclose(f);
		return true;
	}
	else
		return false;
}

void printSymbols(string filename)
{
	FILE* f = fopen(filename.c_str(), "w");
	fprintf(f, "%d %d %d\n", symbol_w, symbol_h, oversize);
	//fwprintf(f, L"%s\n", encoding);

    for (int ch = 0; ch < encoding_size; ch++)
    {
    	for (int j = 0; j < symbol_h * oversize; j++)
    	{
    		for (int i = 0; i < symbol_w * oversize; i++)
    		{
    			if (symbol[ch][j * symbol_w * oversize + i])
    				fprintf(f, "#");
    			else
    				fprintf(f, " ");
    		}
			fprintf(f, "\n");
    	}
		fprintf(f, "\n");
    }
    fclose(f);
}

void putSymbol(SDL_Surface *surface, char code, int x, int y, Uint8 r, Uint8 g, Uint8 b, int oversize)
{
	// Searching the symbol code in the current encoding
/*	int code = 0;
	for (unsigned int k = 0; k < wcslen(encoding); k++)
	{
		if (encoding[k] == symbol) { code = k; break; }
	}*/

	// Drawing it
	for (int j = 0; j < symbol_h * oversize; j++)
	for (int i = 0; i < symbol_w * oversize; i++)
	{
		if (symbol[code][j * symbol_w * oversize + i])
		{
			addPixel24(surface, x + i / oversize, y + j / oversize, r, g, b, 1.0 / oversize / oversize);
		}
	}
}
/*
void putString(SDL_Surface *surface, string str, int x, int y, Uint8 font[], int symbol_w, int symbol_h, int distance, wchar_t encoding[], Uint8 r, Uint8 g, Uint8 b, int oversize)
{
	int xc = x;
	for (int i = 0; i < str.length(); i++)
	{
		putSymbol(surface, str[i], xc, y, font, symbol_w, symbol_h, encoding, r, g, b, oversize);
		xc += symbol_w + distance;
	}
}*/


int main(int argc, char* argv[])
{
    printf("Starting UI event loop.\n");
    fflush(stdout);
    if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
    	printf("Could not initialize SDL: %d", SDL_GetError());
    	return 1;
    }

    SDL_WM_SetCaption("Font editor", "Font editor");

    atexit(SDL_Quit);
    printf("Starting UI event loop.\n");

    SDL_Surface* screen;
    screen = SDL_SetVideoMode(width, height, 24, SDL_HWSURFACE | /*SDL_FULLSCREEN |*/ SDL_DOUBLEBUF);

    if(screen == NULL)
    {
    	printf("Could not set video mode: %d", SDL_GetError());
    	return 1;
    }


    scanSymbols("font.txt");

    quit_pending = false;
    printf("Starting UI event loop.\n");
    bool success = false;
    while( !quit_pending )
    {
        SDL_Rect rct;
        rct.x = 0; rct.y = 0; rct.w = width; rct.h = height;
        SDL_FillRect(screen, &rct, 0x000000);

    	draw(screen);
        process_events();

        SDL_Delay(10);
        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }

    printSymbols("font.txt");

    freeSymbols();
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
			if (event.key.keysym.sym == SDLK_LCTRL)
			{
				base_mode = false;
			}
			if (event.key.keysym.sym == SDLK_KP_PLUS)
			{
				current_char = (current_char + 1) % encoding_size;
			}
			if (event.key.keysym.sym == SDLK_KP_MINUS)
			{
				current_char = (current_char + encoding_size - 1) % encoding_size;
			}
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LCTRL)
			{
				base_mode = true;
			}
			break;
		case SDL_QUIT:
			/* Handle quit requests (like Ctrl-c). */
			quit_pending = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.x > drawing_box_x + drawing_box_frame_width && event.button.x < drawing_box_x + drawing_box_frame_width + symbol_w * oversize * cell_width &&
			    event.button.y > drawing_box_y + drawing_box_frame_width && event.button.y < drawing_box_y + drawing_box_frame_width + symbol_h * oversize * cell_width)
			{
				if (!base_mode)
				{
					int cell_x = (event.button.x - drawing_box_x - drawing_box_frame_width) / cell_width;
					int cell_y = (event.button.y - drawing_box_y - drawing_box_frame_width) / cell_width;
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						symbol[current_char][cell_y * symbol_w * oversize + cell_x] = true;
						painting_color = white;
					}
					else if (event.button.button == SDL_BUTTON_RIGHT)
					{
						symbol[current_char][cell_y * symbol_w * oversize + cell_x] = false;
						painting_color = black;
					}

					painting = true;
					painting_cell_x = cell_x;
					painting_cell_y = cell_y;
				}
				else
				{
					int cell_x = (event.button.x - drawing_box_x - drawing_box_frame_width) / cell_width / oversize;
					int cell_y = (event.button.y - drawing_box_y - drawing_box_frame_width) / cell_width / oversize;
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						for (int p = 0; p < oversize; p++)
						for (int q = 0; q < oversize; q++)
						{
							symbol[current_char][(oversize * cell_y + q) * symbol_w * oversize + (oversize * cell_x + p)] = true;
						}
						painting_color = white;
					}
					else if (event.button.button == SDL_BUTTON_RIGHT)
					{
						for (int p = 0; p < oversize; p++)
						for (int q = 0; q < oversize; q++)
						{
							symbol[current_char][(oversize * cell_y + q) * symbol_w * oversize + (oversize * cell_x + p)] = false;
						}
						painting_color = black;
					}

					painting = true;
					painting_cell_x = cell_x;
					painting_cell_y = cell_y;
				}
			}
			break;
		case SDL_MOUSEMOTION:
			if (painting &&
				event.motion.x > drawing_box_x + drawing_box_frame_width && event.motion.x < drawing_box_x + drawing_box_frame_width + symbol_w * oversize * cell_width &&
			    event.motion.y > drawing_box_y + drawing_box_frame_width && event.motion.y < drawing_box_y + drawing_box_frame_width + symbol_h * oversize * cell_width)
			{
				if (!base_mode)
				{
					int cell_x = (event.motion.x - drawing_box_x - drawing_box_frame_width) / cell_width;
					int cell_y = (event.motion.y - drawing_box_y - drawing_box_frame_width) / cell_width;
					if (painting_cell_x != cell_x || painting_cell_y != cell_y)
					{

						if (painting_color == white)
						{
							symbol[current_char][cell_y * symbol_w * oversize + cell_x] = true;
						}
						else if (painting_color == black)
						{
							symbol[current_char][cell_y * symbol_w * oversize + cell_x] = false;
						}

						painting_cell_x = cell_x;
						painting_cell_y = cell_y;
					}
				}
				else
				{
					int cell_x = (event.button.x - drawing_box_x - drawing_box_frame_width) / cell_width / oversize;
					int cell_y = (event.button.y - drawing_box_y - drawing_box_frame_width) / cell_width / oversize;
					if (painting_color == white)
					{
						for (int p = 0; p < oversize; p++)
						for (int q = 0; q < oversize; q++)
						{
							symbol[current_char][(oversize * cell_y + q) * symbol_w * oversize + (oversize * cell_x + p)] = true;
						}
						painting_color = white;
					}
					else if (painting_color == black)
					{
						for (int p = 0; p < oversize; p++)
						for (int q = 0; q < oversize; q++)
						{
							symbol[current_char][(oversize * cell_y + q) * symbol_w * oversize + (oversize * cell_x + p)] = false;
						}
						painting_color = black;
					}

					painting = true;
					painting_cell_x = cell_x;
					painting_cell_y = cell_y;
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			painting = false;
			break;
		}
	}
}

void draw(SDL_Surface* surface)
{
	blendFrame(surface, drawing_box_x, drawing_box_x + 2 * drawing_box_frame_width + symbol_w * oversize * cell_width,
			             drawing_box_y, drawing_box_y + 2 * drawing_box_frame_width + symbol_h * oversize * cell_width, 255, 255, 255, 1);

	for (int i = 0; i < symbol_w * oversize; i++)
	for (int j = 0; j < symbol_h * oversize; j++)
	{
		if (symbol[current_char][j * symbol_w * oversize + i])
		{
			blendFrameFilled(surface, drawing_box_x + drawing_box_frame_width + cell_width * i,
			                           drawing_box_x + drawing_box_frame_width + cell_width * (i + 1),
			                           drawing_box_y + drawing_box_frame_width + cell_width * j,
			                           drawing_box_y + drawing_box_frame_width + cell_width * (j + 1), 255, 255, 255, 1);

		}
		blendFrame(surface, drawing_box_x + drawing_box_frame_width + cell_width * i,
							 drawing_box_x + drawing_box_frame_width + cell_width * (i + 1),
							 drawing_box_y + drawing_box_frame_width + cell_width * j,
							 drawing_box_y + drawing_box_frame_width + cell_width * (j + 1), 128, 128, 128, 0.3);
	}

	for (int i = 0; i < symbol_w; i++)
	for (int j = 0; j < symbol_h; j++)
	{
		blendFrame(surface, drawing_box_x + drawing_box_frame_width + cell_width * oversize * i,
							 drawing_box_x + drawing_box_frame_width + cell_width * oversize * (i + 1),
							 drawing_box_y + drawing_box_frame_width + cell_width * oversize * j,
							 drawing_box_y + drawing_box_frame_width + cell_width * oversize * (j + 1), 128, 128, 128, 0.8);
	}

	// Currently char preview

	blendFrame(surface, symbol_preview_x, symbol_preview_x + symbol_w + 2 * symbol_preview_frame_width,
	                    symbol_preview_y, symbol_preview_y + symbol_h + 2 * symbol_preview_frame_width, 255, 255, 255, 1);

	putSymbol(surface, current_char, symbol_preview_x + symbol_preview_frame_width,
	                                 symbol_preview_y + symbol_preview_frame_width, 255, 255, 255, 3);

	// Drawing characters palette


	for (int ch = current_char - char_view_length_half; ch <= current_char + char_view_length_half; ch++)
	{
		int ind = ch - (current_char - char_view_length_half);
		int ch_ind = (ch + encoding_size) % encoding_size;

		int x1 = char_view_x + (symbol_w + 5) * ind;
		int x2 = char_view_x + (symbol_w + 5) * (ind + 1) - 1;
		int y1 = char_view_y;
		int y2 = char_view_y + symbol_h + 4;

		blendFrame(surface, x1, x2, y1, y2, 128, 128, 128, 1);
		if (ch_ind == current_char)
		{
			blendFrame(surface, x1, x2, y1, y2, 255, 255, 255, 1);
		}

		putSymbol(surface, ch_ind, x1 + 3, y1 + 3, 255, 255, 255, 3);
	}
}
