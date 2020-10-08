#include "util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _WIN32
  #include <windows.h>
#endif

#include <iostream>

namespace util
{

SDL_Window* sdl_window;
SDL_Surface* sdl_screen;
bool window_closed;
double delta_time;

void log(std::string message)
{
	std::cout << "[util]" << message << std::endl;
}

void cleanup()
{
	log("Cleaning up SDL2");
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();
}

void error(std::string message)
{
	std::cout << "Error: " << message << std::endl;
#ifdef _WIN32
	MessageBox(0, message.c_str(), "SDL Program Error", 0);
#endif
	exit(1);
}

void sdl_initialize(std::string title, int width, int height)
{
	log("Initializing SDL2");

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		error("Failed to initialize SDL");
	}

	sdl_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

	if (!sdl_window)
	{
		error("Failed to open window");
	}

	sdl_screen = SDL_GetWindowSurface(sdl_window);
	atexit(cleanup);
}

void sdl_poll()
{
	static Uint32 now = 0;
	static Uint32 last = 0;

	now = SDL_GetTicks();
	delta_time = (now - last) / 1000.0f;
	last = now;

	SDL_Event e = { 0 };

	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			window_closed = true;
		}
	}
}

void sdl_clearscreen(int r, int g, int b)
{
	SDL_FillRect(sdl_screen, &sdl_screen->clip_rect, SDL_MapRGB(sdl_screen->format, r, g, b));
}

void sdl_blit(SDL_Surface* src, SDL_Surface* dest, int x, int y)
{
	SDL_Rect r = { 0 };
	r.x = x;
	r.y = y;
	SDL_BlitSurface(src, NULL, dest, &r);
}

bool sdl_keydown(SDL_Keycode key)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[(Uint8)key]) return true;
	return false;
}

Spritesheet* SpritesheetLoad(std::string path, int columns, int rows)
{
	Spritesheet* rtn = new Spritesheet();
	rtn->surface = load_image(path.c_str());

	rtn->rows = rows;
	rtn->columns = columns;

	return rtn;
}

void SpritesheetFree(Spritesheet* ctx)
{
	SDL_FreeSurface(ctx->surface);
	delete ctx;
}

void SpritesheetBlit(Spritesheet* ctx, int column, int row, SDL_Surface* dest, int x, int y)
{
	SDL_Rect sr = {0};

	column = column % ctx->columns;
	row = row % ctx->rows;

	sr.w = ctx->surface->w / ctx->columns;
	sr.h = ctx->surface->h / ctx->rows;
	sr.x = sr.w * column;
	sr.y = sr.h * row;

	SDL_Rect dr = {0};

	dr.x = x;
	dr.y = y;

	SDL_BlitSurface(ctx->surface, &sr, dest, &dr);
}

SDL_Surface* load_image(const std::string& path)
{
	int width = 0;
	int height = 0;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, NULL, 4);

	if (!data) error("Failed to load image");

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	Uint32 rmask = 0xff000000;
	Uint32 gmask = 0x00ff0000;
	Uint32 bmask = 0x0000ff00;
	Uint32 amask = 0x000000ff;
#else
	Uint32 rmask = 0x000000ff;
	Uint32 gmask = 0x0000ff00;
	Uint32 bmask = 0x00ff0000;
	Uint32 amask = 0xff000000;
#endif

	SDL_Surface* rtn = SDL_CreateRGBSurface(0, width, height, 32,
		rmask, gmask, bmask, amask);

    if (!rtn) error("Failed to allocate image");

	SDL_LockSurface(rtn);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			size_t di = width * 4 * y + x * 4;
			Uint8 *tp = (Uint8*)rtn->pixels + rtn->pitch * y + x * 4;
			*(Uint32*)tp = SDL_MapRGBA(rtn->format, data[di], data[di+1], data[di+2], data[di+3]);
		}
	}

	free(data);
	SDL_UnlockSurface(rtn);

	return rtn;
}

}
