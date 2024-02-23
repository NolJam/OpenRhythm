#ifndef GLOBALS_DEF
#define GLOBALS_DEF

#define FALSE 0
#define TRUE 1

#define SCREEN_WIDTH 1024 // 32 32-pix chunks
#define SCREEN_HEIGHT 576 // 18 32-pix chunks

typedef enum GameState {
	MAIN_MENU,
	PLAYING,
	PAUSED
} GameState;

#endif
