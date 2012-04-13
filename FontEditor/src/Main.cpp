#include <SDL.h>
#include <stdio.h>
#include <assert.h>
#include <vector>

#include <string>

using namespace std;

class Font
{
private:
	vector<bool*> letters;
	int letterWidth, letterHeight, overSize;
protected:
	void initFromOther(const Font& other)
	{
		letterWidth = other.letterWidth;
		letterHeight = other.letterHeight;
		for (vector<bool*>::const_iterator iter = other.getLettersBegin(); iter != other.getLettersEnd(); iter++)
		{
			bool* newChar = insert(letters.end());
			memcpy(newChar, *iter, letterWidth * letterHeight * sizeof(bool));
		}
	}
public:
	// Getters

	int getOverSize() const { return overSize; }
	int getLetterWidth() const { return letterWidth; }
	int getLetterHeight() const { return letterHeight; }
	const vector<bool*>::const_iterator getLettersBegin() const { return letters.begin(); }
	const vector<bool*>::const_iterator getLettersEnd() const { return letters.end(); }
	const vector<bool*>::iterator getLettersBegin() { return letters.begin(); }
	const vector<bool*>::iterator getLettersEnd() { return letters.end(); }

	// Constructors / destructor

	Font(const Font& other)
	{
		initFromOther(other);
	}

	Font(int letterWidth, int letterHeight) : letterWidth(letterWidth),
	                                      letterHeight(letterHeight)
	{

	}

	Font(const string& fileName)
	{
		if (!loadFromFile(fileName))
		{
			throw exception();
		}
	}

	virtual ~Font()
	{
		clear();
	}

	// Operators

	Font& operator = (const Font& other)
	{
		clear();
		initFromOther(other);
		return *this;
	}

	// Methods

	bool loadFromFile(const string& fileName)
	{
		clear();
		FILE* f = fopen(fileName.c_str(), "r");
		if (f != NULL)
		{
			fscanf(f, "%d %d %d", &letterWidth, &letterHeight, &overSize);
			fgetc(f);	// skip '\n'
		}

		if (f != NULL)
		{
			while (!feof(f))
			{
				bool* newChar = insert(letters.end());
				for (int j = 0; j < letterHeight * overSize; j++)
				{
					for (int i = 0; i < letterWidth * overSize; i++)
					{
						char c = fgetc(f);
						newChar[j * letterWidth * overSize + i] = (c == '#');
					}
					fgetc(f);	// skip '\n'
				}
				fgetc(f);	// skip '\n'
			}
			fclose(f);
			return true;
		}
		else
			return false;
	}

	bool saveToFile(const string& filename)
	{
		FILE* f = fopen(filename.c_str(), "w");
		fprintf(f, "%d %d %d\n", letterWidth, letterHeight, overSize);

		if (f != NULL)
		{
			for (vector<bool*>::iterator iter = letters.begin(); iter != letters.end(); iter++)
			{
				for (int j = 0; j < letterHeight * overSize; j++)
				{
					for (int i = 0; i < letterWidth * overSize; i++)
					{
						if ((*iter)[j * letterWidth * overSize + i])
							fprintf(f, "#");
						else
							fprintf(f, " ");
					}
					fprintf(f, "\n");
				}
				if (iter != letters.end() - 1)
					fprintf(f, "\n");
			}
			fclose(f);
			return true;
		}
		else
			return false;
	}

	bool* insert(vector<bool*>::iterator iter)
	{
		bool* newChar = new bool[letterWidth * letterHeight * overSize * overSize];
		letters.insert(iter, newChar);
		return newChar;
	}

	void remove(vector<bool*>::iterator iter)
	{
		delete [] (*iter);
		letters.erase(iter);
	}

	void clear()
	{
		while (letters.size() > 0) remove(letters.begin());
	}
};


int width = 800;
int height = 600;

bool quit_pending = false;

void blendPixel24(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b, float a)
{
//	if(x >= 0 && y >= 0 && x < surface->w && y < surface->h)
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

int char_view_number_half = 25;
int char_view_y = 20;
int char_view_margin = 20;

//int symbol_preview_x = 387;
int symbol_preview_y = 70;

int drawing_box_frame_width = 5;
int symbol_preview_frame_width = 10;

/*int symbol_w = 10;
int symbol_h = 16;*/

int cell_size = 10;

/*
wchar_t* encoding = L" "
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                     "abcdefghijklmnopqrstuvwxyz"
                     "0123456789.,:;!?@#$%^&*()[]{}_-+<>=~\"'`/\\|";
int encoding_size = 97;*/

bool painting = false;
bool base_mode = true;

enum color { black, white } painting_color;
int painting_cell_x = -1, painting_cell_y = -1;

void draw(SDL_Surface* surface, const Font& edited);
void process_events(Font& edited);

void putSymbol(SDL_Surface *surface, const Font& font, vector<bool*>::const_iterator iter, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	// Searching the symbol code in the current encoding
/*	int code = 0;
	for (unsigned int k = 0; k < wcslen(encoding); k++)
	{
		if (encoding[k] == symbol) { code = k; break; }
	}*/

	// Drawing it
	for (int j = 0; j < font.getLetterHeight() * font.getOverSize(); j++)
	for (int i = 0; i < font.getLetterWidth() * font.getOverSize(); i++)
	{
		if ((*iter)[j * font.getLetterWidth() * font.getOverSize() + i])
		{
			/*addPixel24(surface, x + i / oversize - 1, y + j / oversize, 1, 1, b, 1.0 / oversize / oversize / 2);
			addPixel24(surface, x + i / oversize, y + j / oversize, 1, 1, b, 1.0 / oversize / oversize / 2);
			addPixel24(surface, x + i / oversize, y + j / oversize, 1, g, 1, 1.0 / oversize / oversize);
			addPixel24(surface, x + i / oversize, y + j / oversize, r, 1, 1, 1.0 / oversize / oversize / 2);
			addPixel24(surface, x + i / oversize + 1, y + j / oversize, r, 1, 1, 1.0 / oversize / oversize / 2);*/
			addPixel24(surface, x + i / font.getOverSize(), y + j / font.getOverSize(), r, g, b, 1.0 / font.getOverSize() / font.getOverSize());
		}
	}
}

vector<bool*>::iterator currentLetter;

int main(int argc, char* argv[])
{
	Font& edited = *new Font("font.txt");
	currentLetter = edited.getLettersBegin();

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

    //scanSymbols("font.txt");

    quit_pending = false;
    printf("Starting UI event loop.\n");
    bool success = false;
    while( !quit_pending )
    {
        SDL_Rect rct;
        rct.x = 0; rct.y = 0; rct.w = width; rct.h = height;
        SDL_FillRect(screen, &rct, 0x000000);

    	draw(screen, edited);
        process_events(edited);

        SDL_Delay(10);
        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }

    edited.saveToFile("font.txt");
}

void process_events(Font& edited)
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
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				currentLetter++;
				if (currentLetter == edited.getLettersEnd()) currentLetter = edited.getLettersBegin();
			}
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				if (currentLetter == edited.getLettersBegin())
				{
					currentLetter = edited.getLettersEnd() - 1;
				}
				else
				{
					currentLetter--;
				}
			}
			if (event.key.keysym.sym == SDLK_INSERT)
			{
				edited.insert(currentLetter);
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
			if (event.button.x > drawing_box_x + drawing_box_frame_width && event.button.x < drawing_box_x + drawing_box_frame_width + edited.getLetterWidth() * edited.getOverSize() * cell_size &&
			    event.button.y > drawing_box_y + drawing_box_frame_width && event.button.y < drawing_box_y + drawing_box_frame_width + edited.getLetterHeight() * edited.getOverSize() * cell_size)
			{
				if (!base_mode)
				{
					int cell_x = (event.button.x - drawing_box_x - drawing_box_frame_width) / cell_size;
					int cell_y = (event.button.y - drawing_box_y - drawing_box_frame_width) / cell_size;
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						(*currentLetter)[cell_y * edited.getLetterWidth() * edited.getOverSize() + cell_x] = true;
						painting_color = white;
					}
					else if (event.button.button == SDL_BUTTON_RIGHT)
					{
						(*currentLetter)[cell_y * edited.getLetterWidth() * edited.getOverSize() + cell_x] = false;
						painting_color = black;
					}

					painting = true;
					painting_cell_x = cell_x;
					painting_cell_y = cell_y;
				}
				else
				{
					int cell_x = (event.button.x - drawing_box_x - drawing_box_frame_width) / cell_size / edited.getOverSize();
					int cell_y = (event.button.y - drawing_box_y - drawing_box_frame_width) / cell_size / edited.getOverSize();
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						for (int p = 0; p < edited.getOverSize(); p++)
						for (int q = 0; q < edited.getOverSize(); q++)
						{
							(*currentLetter)[(edited.getOverSize() * cell_y + q) * edited.getLetterWidth() * edited.getOverSize() + (edited.getOverSize() * cell_x + p)] = true;
						}
						painting_color = white;
					}
					else if (event.button.button == SDL_BUTTON_RIGHT)
					{
						for (int p = 0; p < edited.getOverSize(); p++)
						for (int q = 0; q < edited.getOverSize(); q++)
						{
							(*currentLetter)[(edited.getOverSize() * cell_y + q) * edited.getLetterWidth() * edited.getOverSize() + (edited.getOverSize() * cell_x + p)] = false;
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
				event.motion.x > drawing_box_x + drawing_box_frame_width &&
				event.motion.x < drawing_box_x + drawing_box_frame_width + edited.getLetterWidth() * edited.getOverSize() * cell_size &&
			    event.motion.y > drawing_box_y + drawing_box_frame_width &&
			    event.motion.y < drawing_box_y + drawing_box_frame_width + edited.getLetterHeight() * edited.getOverSize() * cell_size)
			{
				if (!base_mode)
				{
					int cell_x = (event.motion.x - drawing_box_x - drawing_box_frame_width) / cell_size;
					int cell_y = (event.motion.y - drawing_box_y - drawing_box_frame_width) / cell_size;
					if (painting_cell_x != cell_x || painting_cell_y != cell_y)
					{

						if (painting_color == white)
						{
							(*currentLetter)[cell_y * edited.getLetterWidth() * edited.getOverSize() + cell_x] = true;
						}
						else if (painting_color == black)
						{
							(*currentLetter)[cell_y * edited.getLetterWidth() * edited.getOverSize() + cell_x] = false;
						}

						painting_cell_x = cell_x;
						painting_cell_y = cell_y;
					}
				}
				else
				{
					int cell_x = (event.button.x - drawing_box_x - drawing_box_frame_width) / cell_size / edited.getOverSize();
					int cell_y = (event.button.y - drawing_box_y - drawing_box_frame_width) / cell_size / edited.getOverSize();
					if (painting_color == white)
					{
						for (int p = 0; p < edited.getOverSize(); p++)
						for (int q = 0; q < edited.getOverSize(); q++)
						{
							(*currentLetter)[(edited.getOverSize() * cell_y + q) * edited.getLetterWidth() * edited.getOverSize() + (edited.getOverSize() * cell_x + p)] = true;
						}
						painting_color = white;
					}
					else if (painting_color == black)
					{
						for (int p = 0; p < edited.getOverSize(); p++)
						for (int q = 0; q < edited.getOverSize(); q++)
						{
							(*currentLetter)[(edited.getOverSize() * cell_y + q) * edited.getLetterWidth() * edited.getOverSize() + (edited.getOverSize() * cell_x + p)] = false;
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

void draw(SDL_Surface* surface, const Font& edited)
{
	blendFrame(surface, drawing_box_x, drawing_box_x + 2 * drawing_box_frame_width + edited.getLetterWidth() * edited.getOverSize() * cell_size,
			             drawing_box_y, drawing_box_y + 2 * drawing_box_frame_width + edited.getLetterHeight() * edited.getOverSize() * cell_size, 255, 255, 255, 1);

	for (int i = 0; i < edited.getLetterWidth() * edited.getOverSize(); i++)
	for (int j = 0; j < edited.getLetterHeight() * edited.getOverSize(); j++)
	{
		if ((*currentLetter)[j * edited.getLetterWidth() * edited.getOverSize() + i])
		{
			blendFrameFilled(surface, drawing_box_x + drawing_box_frame_width + cell_size * i,
			                           drawing_box_x + drawing_box_frame_width + cell_size * (i + 1),
			                           drawing_box_y + drawing_box_frame_width + cell_size * j,
			                           drawing_box_y + drawing_box_frame_width + cell_size * (j + 1), 255, 255, 255, 1);

		}
		blendFrame(surface, drawing_box_x + drawing_box_frame_width + cell_size * i,
							 drawing_box_x + drawing_box_frame_width + cell_size * (i + 1),
							 drawing_box_y + drawing_box_frame_width + cell_size * j,
							 drawing_box_y + drawing_box_frame_width + cell_size * (j + 1), 128, 128, 128, 0.3);
	}

	// Drawing the big (pixel) cells

	for (int i = 0; i < edited.getLetterWidth(); i++)
	for (int j = 0; j < edited.getLetterHeight(); j++)
	{
		blendFrame(surface, drawing_box_x + drawing_box_frame_width + cell_size * edited.getOverSize() * i,
							 drawing_box_x + drawing_box_frame_width + cell_size * edited.getOverSize() * (i + 1),
							 drawing_box_y + drawing_box_frame_width + cell_size * edited.getOverSize() * j,
							 drawing_box_y + drawing_box_frame_width + cell_size * edited.getOverSize() * (j + 1), 128, 128, 128, 0.8);
	}

	// Currently char preview

	int symbol_preview_x = width / 2 - edited.getLetterWidth() / 2 - symbol_preview_frame_width;

	blendFrame(surface, symbol_preview_x, symbol_preview_x + edited.getLetterWidth() + 2 * symbol_preview_frame_width,
	                    symbol_preview_y, symbol_preview_y + edited.getLetterHeight() + 2 * symbol_preview_frame_width, 255, 255, 255, 1);

	putSymbol(surface, edited, currentLetter, symbol_preview_x + symbol_preview_frame_width,
	                                 symbol_preview_y + symbol_preview_frame_width, 255, 255, 255);

	// Drawing characters palette

	char_view_number_half = (width - 2 * char_view_margin) / 2 / (edited.getLetterWidth() + 5);

	int char_view_left = (int)(width / 2 - (double)(edited.getLetterWidth() + 5) * (char_view_number_half + 0.5));

	vector<bool*>::const_iterator iter = currentLetter;
	for (int i = 0; i < char_view_number_half; i++)
	{
		if (iter == edited.getLettersBegin())
			iter = edited.getLettersEnd() - 1;
		else
			iter --;
	}

	for (int ind = 0; ind <= 2 * char_view_number_half; ind++)
	{
		iter++;
		if (iter == edited.getLettersEnd()) iter = edited.getLettersBegin();

		int x1 = char_view_left + (edited.getLetterWidth() + 5) * ind;
		int x2 = char_view_left + (edited.getLetterWidth() + 5) * (ind + 1) - 1;
		int y1 = char_view_y;
		int y2 = char_view_y + edited.getLetterHeight() + 4;

		blendFrame(surface, x1, x2, y1, y2, 128, 128, 128, 1);
		if (iter == currentLetter)
		{
			blendFrame(surface, x1, x2, y1, y2, 255, 255, 255, 1);
		}

		putSymbol(surface, edited, iter, x1 + 3, y1 + 3, 255, 255, 255);
	}
}
