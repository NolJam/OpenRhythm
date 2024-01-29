#include <SDL2/SDL.h>
#include <stdio.h>

#include "globals.h"
#include "beat.h"

#ifndef TRACK_DEF
#define TRACK_DEF

typedef struct Track {
	float x;
	float y;
	SDL_Rect sprite;
} Track;

int track_press(Track* t, Beat* b);

void track_init(Track* t);

#endif
