#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "globals.h"
#include "beat.h"
#include "level.h"
#include "track.h"

Uint64 last_ticks = 0;
Uint64 cur_ticks = 0;
Uint64 delta_time = 0;

unsigned int quit = FALSE;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Mix_Music* music = NULL;

Level* level = NULL;

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
				else if (e.key.keysym.sym == SDLK_SPACE) 
				{
					if (track_press(&level->tracks[0], &level->beats[level->cur_beat])) level->cur_beat++; 
					//printf("cur beat: %d\n\n", level->cur_beat);
					else printf("no beat hit.\n\n");
				}
			}

		else if (e.type == SDL_QUIT) quit = TRUE;
	}
}

int main(int argc, char* argv[])
{
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

	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf("mixer couldn't init. error: %s\n", Mix_GetError());
		exit(1);
	}
	music = Mix_LoadMUS("song.mp3");
	if (music == NULL)
	{
		printf("music couldn't be loaded");
		exit(1);
	}

	level = malloc(sizeof(Level));

	Beat* ptr = malloc(sizeof(Beat));
	if (ptr == NULL) exit(1);
	level->beats = ptr;
	
	Track* tptr = malloc((size_t)5 * sizeof(Track));
	if (tptr == NULL) exit(1);
	level->tracks = tptr;

	level_load(level, "level1.lvl");

	printf("BPM: %f\n", level->bpm);

	Mix_PlayMusic(music, 0);
	while (quit == FALSE)
	{
		update_delta_time();

		input();

		if (level->beats[level->cur_beat].x < 0)
		{
			level->cur_beat++;
			printf("cur beat: %d\n\n", level->cur_beat);
		}	

		for (int i = level->cur_beat; i < level->num_beats; i++)
		{
			beat_move(&level->beats[i], (SCREEN_WIDTH / 4000.0f) * (level->bpm / 60.0f), delta_time);
		}
		//printf("%f\n", level->beats[0].x);

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &level->tracks[0].sprite);
		SDL_RenderFillRect(renderer, &level->tracks[1].sprite);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		for (int i = level->cur_beat; i < level->num_beats; i++)
		{
			if (level->beats[i].x > SCREEN_WIDTH) continue;
			if (level->beats[i].x < 0) continue;

			SDL_RenderFillRect(renderer, &level->beats[i].sprite);
		}

		SDL_RenderPresent(renderer);
	}

	free(level);

	Mix_FreeMusic(music);
	music = NULL;

	SDL_DestroyWindow(window);

	Mix_Quit();
	SDL_Quit();

	return 0;
}
