#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "globals.h"
#include "beat.h"
#include "track.h"
#include "level.h"
#include "menu.h"
#include "score.h"
#include "text.h"
#include "files.h"

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
Mix_Chunk* hitGood = NULL;
Mix_Chunk* hitBad = NULL;
Mix_Chunk* miss = NULL;
Mix_Chunk* menuForward = NULL;
Mix_Chunk* menuBack = NULL;

SDL_Texture* up_texture = NULL;
SDL_Texture* down_texture = NULL;
SDL_Texture* left_texture = NULL;
SDL_Texture* right_texture = NULL;
SDL_Texture* beat_textures[4];

SDL_Texture* t_tx1 = NULL;

Level* level = NULL;

GameState state = MAIN_MENU;

static SDL_Texture* texture_load(SDL_Renderer* r, const char* path)
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

static void update_delta_time()
{
        last_ticks = cur_ticks;
        cur_ticks = SDL_GetTicks64();
        delta_time = cur_ticks - last_ticks;

        // if (delta_time > MAX_DT) delta_time = MAX_DT;

        if (delta_time < 4) SDL_Delay(4 - delta_time); // MAYBE LIMIT FPS
}

static void update_music_delta()
{
	if (Mix_PlayingMusic() == 0) return;
	
	last_music_pos = cur_music_pos;
	cur_music_pos = Mix_GetMusicPosition(music);

	music_delta = cur_music_pos - last_music_pos;
}

static void resize_screen() 
{
	SDL_GetWindowSize(window, &cur_screen_w, &cur_screen_h);
	//printf("window width: %d\nwindow height: %d\n", cur_screen_w, cur_screen_h);
	//cur_screen_w = (cur_screen_h / 9) * 16;
	SDL_RenderSetLogicalSize(renderer, cur_screen_w, cur_screen_h);
	SDL_RenderSetScale(renderer, (float)cur_screen_w / (float)SCREEN_WIDTH, (float)cur_screen_h / (float)SCREEN_HEIGHT);
}

static void input()
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
					Mix_PlayChannel(-1, menuBack, 0);
				}
				else if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w)
				{
					if (level->num_tracks > 0 && track_press(&level->tracks[0]))
					{
						Mix_HaltChannel(-1);
						Mix_PlayChannel(-1, hitGood, 0);
						
						continue;
					}
					//printf("cur beat: %d\n\n", level->cur_beat);
					//else printf("no beat hit.\n\n");
				}
				else if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a)
				{
					if (level->num_tracks > 1 && track_press(&level->tracks[1]))
					{
						Mix_HaltChannel(-1);
						Mix_PlayChannel(-1, hitGood, 0);

						continue;
					}
					//printf("cur beat: %d\n\n", level->cur_beat);
					//else printf("no beat hit.\n\n");
				}
				else if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d)
				{
					if (level->num_tracks > 2 && track_press(&level->tracks[2]))
					{
						Mix_HaltChannel(-1);
						Mix_PlayChannel(-1, hitGood, 0);

						continue;
					}
					//else printf("no beat hit.\n\n");
				}
				else if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s)
				{
					if (level->num_tracks > 3 && track_press(&level->tracks[3]))
					{
						Mix_HaltChannel(-1);
						Mix_PlayChannel(-1, hitGood, 0);

						continue;
					}
					//else printf("no beat hit.\n\n");
				}
			}
		else if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.type == SDL_WINDOWEVENT_SIZE_CHANGED) resize_screen();
		}
		else if (e.type == SDL_QUIT) quit = TRUE;
	}
}

static void menu_input()
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN)
		{
			//if (e.key.keysym.sym == SDLK_SPACE)
			//{
			//	if (state == PAUSED)
			//	{
			//		Mix_ResumeMusic();
			//		menu_set_play();
			//		update_delta_time();
			//		state = PLAYING;
			//		continue;
			//	}

			//	level_load(level, 0);
			//	Mix_FadeInMusicPos(music, 0, 10, level->start_pos);
			//	//Mix_PlayMusic(music, 0);
			//	menu_set_play();
			//	update_delta_time();
			//	state = PLAYING;
			//}
			if (e.key.keysym.sym == SDLK_ESCAPE)
			{
				if (state == PAUSED)
				{
					score_reset();
					Mix_HaltMusic();
					Mix_FreeMusic(music);
					music = NULL;
					Mix_PlayChannel(-1, menuForward, 0);
					state = MAIN_MENU;
					menu_set_main(renderer);
					score_miss_reset();
					continue;
				}
				else if (state == LEVEL_SELECT)
				{
					state = MAIN_MENU;
					menu_set_main(renderer);
					continue;
				}
				else quit = TRUE;
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
					menu_set_play();
					update_delta_time();
					state = PLAYING;
					continue;
				}
				else if (button == 1)
				{
					score_reset();
					Mix_HaltMusic();
					Mix_FreeMusic(music);
					music = NULL;
					Mix_PlayChannel(-1, menuForward, 0);
					state = MAIN_MENU;
					menu_set_main(renderer);
					score_miss_reset();
					continue;
				}
			}
			else if (state == MAIN_MENU)
			{
				if (button == 0)
				{
					menu_set_level();
					state = LEVEL_SELECT;
					Mix_PlayChannel(-1, menuForward, 0);
				}
				else if (button == 1) quit = TRUE;
			}
			else if (state == LEVEL_SELECT)
			{
				level_load(level, button);

				printf("loading music: %s\n", song_names[button]);
				printf("song name length: %zd\n\n", strlen(song_names[button]));
				music = Mix_LoadMUS(song_names[button]);
				if (music == NULL)
				{
					printf("music couldn't be loaded: %s\n\n", Mix_GetError());
					//exit(1);
				}

				Mix_FadeInMusicPos(music, 0, 10, level->start_pos);
				menu_set_play();
				update_delta_time();
				state = PLAYING;
			}
		}
		else if (e.type == SDL_MOUSEWHEEL && state == LEVEL_SELECT)
		{
			menu_move_levels(e.wheel.y);
		}
		else if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.type == SDL_WINDOWEVENT_SIZE_CHANGED) resize_screen();
		}
		else if (e.type == SDL_QUIT) quit = TRUE;
	}
}

// ============================================================================================================================
// =========================================================MAIN===============================================================
// ============================================================================================================================

int main(int argc, char* argv[])
{
	//LPWSTR cur_directory = NULL;
	//GetCurrentDirectory(1024, cur_directory);
	//files_list_levels("C:\\Program Files\\Nolan Jameson\\OpenRhythm");
	files_list_levels(".\\");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize: %s\n", SDL_GetError());
		quit = TRUE;
	}

	window = SDL_CreateWindow("Rhythm Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP);
	//SDL_SetWindowSize(window, LOGICAL_WIDTH, LOGICAL_HEIGHT);

	if (window == NULL)
	{
		printf("Window could not be created: %s\n", SDL_GetError());
		quit = TRUE;
	}

	//SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	SDL_RendererInfo r_info;
	SDL_GetRendererInfo(renderer, &r_info);

	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	//SDL_RenderSetScale(renderer, (float)LOGICAL_WIDTH / (float)SCREEN_WIDTH, (float)LOGICAL_HEIGHT / (float)SCREEN_HEIGHT);

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
	left_texture = texture_load(renderer, "left.png");
	beat_textures[1] = left_texture;
	right_texture = texture_load(renderer, "right.png");
	beat_textures[2] = right_texture;
	down_texture = texture_load(renderer, "down.png");
	beat_textures[3] = down_texture;
	//up_texture = texture_load(renderer, "up.png");
	//beat_textures[0] = up_texture;
	//down_texture = texture_load(renderer, "down.png");
	//beat_textures[1] = down_texture;

	t_tx1 = texture_load(renderer, "track1.png");

	printf("textures loaded.\n\n");

	// MIX
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 512) < 0 )
	{
		printf("mixer couldn't init. error: %s\n", Mix_GetError());
		exit(1);
	}
	//music = Mix_LoadMUS("TH.wav");
	//if (music == NULL)
	//{
	//	printf("music couldn't be loaded");
	//	exit(1);
	//}
	hitGood = Mix_LoadWAV("hitGood.wav");
	Mix_VolumeChunk(hitGood, 64);

	hitBad = Mix_LoadWAV("hitBad.wav");
	Mix_VolumeChunk(hitBad, 64);

	miss = Mix_LoadWAV("miss.wav");
	Mix_VolumeChunk(miss, 64);

	menuForward = Mix_LoadWAV("menuForward.wav");
	Mix_VolumeChunk(menuForward, 64);

	menuBack = Mix_LoadWAV("menuBack.wav");
	Mix_VolumeChunk(menuBack, 64);

	// TTF
	text_init();

	// LEVEL
	level = malloc(sizeof(Level));

	level->start_pos = 0.0;

	for (int i = 0; i < 5; i++)
	{
        Beat* ptr = calloc(10, sizeof(Beat));
        if (ptr == NULL) exit(1);
        level->tracks[i].beats = ptr;
		//printf("%f\n", level->tracks[i].beats[0].x);
	}

	level_init();

	// MENU
	menu_init(renderer);

	menu_render(renderer);

	Mix_PlayChannel(-1, menuForward, 0);

	while (quit == FALSE)
	{
		update_delta_time();
		//update_music_delta();

		//input();

		if (state != PLAYING)
		{
			menu_input();
			menu_render(renderer);
			continue;
		}

		if (Mix_PlayingMusic() == 0)
		{
			Mix_FreeMusic(music);
			music = NULL;
			menu_set_main(renderer);
			score_miss_reset();
			score_reset();
			state = MAIN_MENU;
			Mix_PlayChannel(-1, menuForward, 0);
		}

		input();

        	for (int i = 0; i < level->num_tracks; i++)
        	{
            		if (level->tracks[i].beats[level->tracks[i].cur_beat].x < level->tracks[0].x - level->tracks[0].sprite.w &&
					level->tracks[i].cur_beat < level->tracks[i].num_beats)
            		{
                		level->tracks[i].cur_beat++;
						int game_over = score_miss_increment();
						Mix_HaltChannel(-1);
						Mix_PlayChannel(-1, miss, 0);
                		//printf("beat missed\n\n");
						if (game_over)
						{
							score_miss_reset();
							score_reset();
							menu_set_main(renderer);
							state = MAIN_MENU;
							Mix_HaltMusic();
							Mix_HaltChannel(-1);
							Mix_PlayChannel(-1, menuBack, 0);
						}
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

		SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
		SDL_RenderClear(renderer);

		//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		//SDL_RenderFillRect(renderer, &level->tracks[0].sprite);
		//SDL_RenderFillRect(renderer, &level->tracks[1].sprite);
		//printf("track sprites drawn.\n\n");

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_RenderCopy(renderer, t_tx1, NULL, &level->tracks[0].sprite);
		SDL_RenderCopy(renderer, t_tx1, NULL, &level->tracks[1].sprite);
		SDL_RenderCopy(renderer, t_tx1, NULL, &level->tracks[2].sprite);
		SDL_RenderCopy(renderer, t_tx1, NULL, &level->tracks[3].sprite);

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

		menu_render(renderer);

		SDL_RenderPresent(renderer);
	}

	level_free(level);

	menu_quit();

	text_quit();

	Mix_CloseAudio();
	if (music != NULL) Mix_FreeMusic(music);
	music = NULL;
	Mix_FreeChunk(hitGood);
	hitGood = NULL;
	Mix_FreeChunk(hitBad);
	hitBad = NULL;
	Mix_FreeChunk(miss);
	miss = NULL;
	Mix_FreeChunk(menuForward);
	menuForward = NULL;
	Mix_FreeChunk(menuBack);
	menuBack = NULL;

	SDL_DestroyTexture(up_texture);
	up_texture = NULL;
	SDL_DestroyTexture(down_texture);
	down_texture = NULL;
	SDL_DestroyTexture(left_texture);
	left_texture = NULL;
	SDL_DestroyTexture(right_texture);
	right_texture = NULL;
	SDL_DestroyTexture(t_tx1);
	t_tx1 = NULL;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	IMG_Quit();
	Mix_Quit();
	SDL_Quit();

	return 0;
}
