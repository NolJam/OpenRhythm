#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "globals.h"
#include "beat.h"
#include "track.h"
#include "level.h"
#include "menu.h"

Uint64 last_ticks = 0;
Uint64 cur_ticks = 0;
Uint64 delta_time = 0;

double last_music_pos = 0.0;
double cur_music_pos = 0.0;
double music_delta = 0.0;

unsigned int quit = FALSE;

float vel = 0.0f;

int cur_screen_w = 1024;
int cur_screen_h = 576;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Mix_Music* music = NULL;

SDL_Texture* up_texture = NULL;
SDL_Texture* down_texture = NULL;
SDL_Texture* beat_textures[2];

SDL_Texture* t_tx1 = NULL;

Level* level = NULL;

GameState state = MAIN_MENU;

SDL_Texture* texture_load(SDL_Renderer* r, const char* path)
{
	SDL_Texture* new_texture = SDL_CreateTexture(r, 376840196, 0, 16, 16);
	if (new_texture == NULL)
	{
		printf("New Texture could not be created: %s\n", SDL_GetError());
		exit(1);
	}

	new_texture = IMG_LoadTexture(r, path);

	Uint32 format;
	int access;
	SDL_QueryTexture(new_texture, &format, &access, NULL, NULL);
	printf("%d\n%d\n\n", format, access);

	if (new_texture == NULL)
	{
		printf("Texture couldn't be loaded. Error: %s\n\n", IMG_GetError());
		exit(1);
	}

	return new_texture;
}

void update_delta_time()
{
        last_ticks = cur_ticks;
        cur_ticks = SDL_GetTicks64();
        delta_time = cur_ticks - last_ticks;

        // if (delta_time > MAX_DT) delta_time = MAX_DT;

        if (delta_time < 8) SDL_Delay(8 - delta_time);
}

void update_music_delta()
{
	if (Mix_PlayingMusic() == 0) return;
	
	last_music_pos = cur_music_pos;
	cur_music_pos = Mix_GetMusicPosition(music);

	music_delta = cur_music_pos - last_music_pos;
}

void resize_screen() 
{
	SDL_GetWindowSize(window, &cur_screen_w, &cur_screen_h);
	//printf("window width: %d\nwindow height: %d\n", cur_screen_w, cur_screen_h);
	//cur_screen_w = (cur_screen_h / 9) * 16;
	SDL_RenderSetLogicalSize(renderer, cur_screen_w, cur_screen_h);
	SDL_RenderSetScale(renderer, (float)cur_screen_w / (float)SCREEN_WIDTH, (float)cur_screen_h / (float)SCREEN_HEIGHT);
}

void input()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					menu_set_pause();
					state = PAUSED;
					Mix_PauseMusic();
				}
				else if (e.key.keysym.sym == SDLK_j)
				{
					if (track_press(&level->tracks[0])) continue;
					//printf("cur beat: %d\n\n", level->cur_beat);
					else printf("no beat hit.\n\n");
				}
				else if (e.key.keysym.sym == SDLK_k)
				{
					if (track_press(&level->tracks[1])) continue;
					//printf("cur beat: %d\n\n", level->cur_beat);
					else printf("no beat hit.\n\n");

				}
			}
		else if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.type == SDL_WINDOWEVENT_SIZE_CHANGED) resize_screen();
		}
		else if (e.type == SDL_QUIT) quit = TRUE;
	}
}

void menu_input()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_SPACE)
			{
				if (state == PAUSED)
				{
					Mix_ResumeMusic();
					update_delta_time();
					state = PLAYING;
					continue;
				}

				level_load(level, "level1.lvl");
				Mix_PlayMusic(music, 0);
				update_delta_time();
				state = PLAYING;
			}
			else if (e.key.keysym.sym == SDLK_ESCAPE)
			{
				if (state == PAUSED)
				{
					Mix_HaltMusic();
					state = MAIN_MENU;
					menu_set_main();
					continue;
				}

				quit = TRUE;
			}
		}
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			int button = menu_click(e.button.x, e.button.y);

			if (state == PAUSED)
			{
				if (button == 0)
				{
					Mix_ResumeMusic();
					update_delta_time();
					state = PLAYING;
					continue;
				}
				else if (button == 1)
				{
					Mix_HaltMusic();
					state = MAIN_MENU;
					menu_set_main();
					continue;
				}
			}
			else if (state == MAIN_MENU)
			{
				if (button == 0)
				{
					level_load(level, "level1.lvl");
					Mix_PlayMusic(music, 0);
					update_delta_time();
					state = PLAYING;
				}
				else if (button == 1) quit = TRUE;
			}
		}
		else if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.type == SDL_WINDOWEVENT_SIZE_CHANGED) resize_screen();
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

	window = SDL_CreateWindow("Rhythm Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (window == NULL)
	{
		printf("Window could not be created: %s\n", SDL_GetError());
		quit = TRUE;
	}

	//SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	SDL_RendererInfo r_info;
	SDL_GetRendererInfo(renderer, &r_info);

	if (renderer == NULL)
	{
		printf("Renderer could not be created: %s\n", SDL_GetError());
		quit = TRUE;
	}

	printf("Renderer driver: %s\n\n", r_info.name);

	// IMG
	int img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) & img_flags))
	{
		printf("SDL_Image could not be initialized. Error: %s\n\n", IMG_GetError());
		exit(1);
	}

	up_texture = texture_load(renderer, "up.png");
	beat_textures[0] = up_texture;
	down_texture = texture_load(renderer, "down.png");
	beat_textures[1] = down_texture;

	t_tx1 = texture_load(renderer, "track1.png");

	printf("textures loaded.\n\n");

	// MIX
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 512) < 0 )
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

	// LEVEL
	level = malloc(sizeof(Level));

	for (int i = 0; i < 5; i++)
	{
        Beat* ptr = calloc(10, sizeof(Beat));
        if (ptr == NULL) exit(1);
        level->tracks[i].beats = ptr;
	printf("%f\n", level->tracks[i].beats[0].x);
	}

	//level_load(level, "level1.lvl");

	printf("BPM: %f\n\n", level->bpm);

	menu_init(renderer);

	printf("update loop starting.\n\n");
	printf("makefile test\n\n");

	menu_render(renderer);

	while (quit == FALSE)
	{
		update_delta_time();
		update_music_delta();

		//input();

		if (state == MAIN_MENU)
		{
			menu_input();
			menu_render(renderer);
			continue;
		}
		else if (state == PAUSED)
		{
			menu_input();
			menu_render(renderer);
			continue;
		}

		input();

        	for (int i = 0; i < level->num_tracks; i++)
        	{
            		if (level->tracks[i].beats[level->tracks[i].cur_beat].x < level->tracks[0].x - level->tracks[0].sprite.w &&
					level->tracks[i].cur_beat < level->tracks[i].num_beats)
            		{
                		level->tracks[i].cur_beat++;
                		printf("beat missed\n\n");
            		}
        	}
		//printf("beats' positions tracked\n\n");

		vel =  level->speed * delta_time;
		//printf("music delta: %f\n\n", music_delta);
		for (int i = 0; i < level->num_tracks; i++)
		{
            		for (int j = level->tracks[i].cur_beat; j < level->tracks[i].num_beats; j++)
			{
                		beat_move(&level->tracks[i].beats[j], vel);
			}
		}
		//printf("beats moved\n\n");
		//printf("%f\n", level->beats[0].x);

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderClear(renderer);

		//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		//SDL_RenderFillRect(renderer, &level->tracks[0].sprite);
		//SDL_RenderFillRect(renderer, &level->tracks[1].sprite);
		//printf("track sprites drawn.\n\n");

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_RenderCopy(renderer, t_tx1, NULL, &level->tracks[0].sprite);
		SDL_RenderCopy(renderer, t_tx1, NULL, &level->tracks[1].sprite);

		for (int i = 0; i < level->num_tracks; i++)
		{
			for (int j = level->tracks[i].cur_beat; j < level->tracks[i].num_beats; j++)
			{
				if (level->tracks[i].beats[j].x > SCREEN_WIDTH) continue;
				if (level->tracks[i].beats[j].x < 0) continue;

				//printf("attempting render copy\n\n");
				SDL_RenderCopy(renderer, beat_textures[i], NULL, &level->tracks[i].beats[j].sprite);
			}
		}

		SDL_RenderPresent(renderer);
	}

	level_free(level);

	menu_quit();

	Mix_CloseAudio();
	Mix_FreeMusic(music);
	music = NULL;

	SDL_DestroyTexture(up_texture);
	up_texture = NULL;
	SDL_DestroyTexture(down_texture);
	down_texture = NULL;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	IMG_Quit();
	Mix_Quit();
	SDL_Quit();

	return 0;
}
