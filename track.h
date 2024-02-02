#ifndef TRACK_DEF
#define TRACK_DEF

#include <SDL2/SDL.h>
#include <stdio.h>

#include "globals.h"
#include "beat.h"

typedef struct Track {
	float x;
	float y;
	SDL_Rect sprite;
	Beat* beats;
	int cur_beat;
	int num_beats;
	int beat_block;
} Track;

int track_press(Track* t);

void track_init(Track* t, int x, int y);

#endif
