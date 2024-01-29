#include <stdio.h>
#include "globals.h"
#include "beat.h"
#include "track.h"

#ifndef LEVEL_DEF
#define LEVEL_DEF

extern const char* BEAT_FORMAT;
extern const char* BPM_FORMAT;
extern const char* TRACK_FORMAT;

typedef struct Level {
	Beat* beats;
	Track* tracks;
	int cur_beat;
	int num_beats;
	float bpm;
} Level;

void level_load(Level* lvl, char* file_name);

#endif
