#ifndef BEAT_HEADER
#define BEAT_HEADER

#include <SDL.h>

typedef struct Beat {
	float x;
	float y;
	SDL_Rect sprite;
} Beat;

void beat_move(Beat* b, const float speed);

#endif
