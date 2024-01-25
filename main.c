#ifndef SDL_DEF
#define SDL_DEF

#include <SDL2/SDL.h>

#endif

#include "beat.h"
#include <stdio.h>

#define FALSE 0
#define TRUE 1

const int SCREEN_WIDTH = 1024; // 32 32-pix chunks
const int SCREEN_HEIGHT = 576; // 18 32-pix chunks

const char* BEAT_FORMAT = "b %d\n";

Uint64 last_ticks = 0;
Uint64 cur_ticks = 0;
Uint64 delta_time = 0;

unsigned int quit = FALSE;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void update_delta_time()
{
        last_ticks = cur_ticks;
        cur_ticks = SDL_GetTicks64();
        delta_time = cur_ticks - last_ticks;

        // if (delta_time > MAX_DT) delta_time = MAX_DT;

        if (delta_time < 6) SDL_Delay(6 - delta_time);
}

void input()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = TRUE;
			}

		else if (e.type == SDL_QUIT) quit = TRUE;
	}
}

int main(int argc, char* argv[])
{
	Beat beat = { 0, 0 };
	printf("%f %f\n", beat.x, beat.y);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize: %s\n", SDL_GetError());
		quit = TRUE;
	}

	window = SDL_CreateWindow("Rhythm Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		printf("Window could not be created: %s\n", SDL_GetError());
		quit = TRUE;
	}

	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RendererInfo r_info;
	SDL_GetRendererInfo(renderer, &r_info);

	if (renderer == NULL)
	{
		printf("Renderer could not be created: %s\n", SDL_GetError());
		quit = TRUE;
	}

	printf("Renderer driver: %s\n", r_info.name);

	Beat beat1;
	beat1.x = SCREEN_WIDTH;
	beat1.y = 300.0f;
	beat1.sprite.x = beat1.x;
	beat1.sprite.y = 300.0f;
	beat1.sprite.w = 64;
	beat1.sprite.h = 64;

	while (quit == FALSE)
	{
		update_delta_time();

		input();

		beat_move(&beat1, 0.1f, delta_time);	

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &beat1.sprite);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
