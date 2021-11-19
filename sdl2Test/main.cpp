/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include "included.h"
//Screen dimension constants
const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 880;
float TIME_PER_TICK = 0.05;
void init();
SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;


void init() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	gWindow = SDL_CreateWindow("Balls", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}
void close()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
}
int main(int argc, char* args[]) {
	init();
	std::vector<Ball*> balls;
	std::srand(std::time(nullptr));
	Engine bs(gRenderer, TIME_PER_TICK);
	std::cout << 1;
	
	bool quit = 0;
	SDL_Event e;
	for (int i = 0; i < 40; ++i) {
		for (int j = 0; j < 40; ++j) {
			bs.add_ball( Vecf(300 + i * 20, 300 + j * 20), 10);
		}
	}
	while (!quit) {
		while (SDL_PollEvent(&e) != 0)
		{

			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				
				bs.handle_mouse_event(e);
			}

		}
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		bs.tick();
		SDL_RenderPresent(gRenderer);
		SDL_Delay(10);
	}
	
	close();
	return 0;
}