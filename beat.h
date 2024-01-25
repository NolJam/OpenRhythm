#ifndef SDL_DEF
#define SDL_DEF

#include <SDL2/SDL.h>

#endif

#ifndef BEAT_HEADER
#define BEAT_HEADER

typedef struct Beat {
	float x;
	float y;
	SDL_Rect sprite;
} Beat;

void beat_move(Beat* b, const float speed, Uint64 dt);

#endif
