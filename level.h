#include <stdio.h>
#include "globals.h"
#include "beat.h"

#ifndef LEVEL_DEF
#define LEVEL_DEF

extern const char* BEAT_FORMAT;
extern const char* BPM_FORMAT;

typedef struct Level {
	Beat* beats;
	int cur_beat;
	int num_beats;
	float bpm;
} Level;

void level_load(Level* lvl, char* file_name);

#endif
