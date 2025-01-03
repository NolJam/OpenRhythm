#ifndef GLOBALS_DEF
#define GLOBALS_DEF

#define FALSE 0
#define TRUE 1

#define SCREEN_WIDTH 1920 // 32 32-pix chunks
#define SCREEN_HEIGHT 1080 // 18 32-pix chunks

#define LOGICAL_WIDTH 1024
#define LOGICAL_HEIGHT 576

#define MAX_LEVELS 100

#include <SDL.h>
//#include <SDL_image.h>

extern SDL_Renderer* renderer;

extern char** level_names;
extern char** song_names;

extern int g_num_levels;

typedef enum GameState {
	PLAYING,
	MAIN_MENU,
	LEVEL_SELECT,
	PAUSED,
} GameState;

#endif
