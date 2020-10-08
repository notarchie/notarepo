#include "util.h"

#include <SDL2/SDL.h>

#include <iostream>

SDL_Surface* image;
SDL_Surface* icon;

void on_load()
{
	image = util::load_image("resources/player.png");
	icon = util::load_image("resources/icon.png");
}

void on_update()
{
	if (util::sdl_keydown(SDL_SCANCODE_RIGHT))
	{
		std::cout << "I should move right" << std::endl;
	}
}

void on_draw()
{
	util::sdl_clearscreen(254, 222, 0);

	util::sdl_blit(image, util::sdl_screen, 200, 200);

	SDL_UpdateWindowSurface(util::sdl_window);
}

void on_cleanup()
{
	SDL_FreeSurface(image);
	SDL_FreeSurface(icon);
}

int main(int argc, char* args[])
{
	util::sdl_initialize("Introduction Game (C++/SDL)", 800, 600);

	on_load();
	SDL_SetWindowIcon(util::sdl_window, icon);

	while (!util::window_closed)
	{
		util::sdl_poll();
		on_update();
		on_draw();
		SDL_Delay(5);
	}

	on_cleanup();

	return 0;
}
