#ifndef LEVEL_DEF
#define LEVEL_DEF

#include <stdio.h>
#include "beat.h"
#include "globals.h"

const char* BEAT_FORMAT = "b %d %f\n";
const char* BPM_FORMAT = "> %f\n";

typedef struct Level {
	Beat* beats;
	int num_beats;
	float bpm;
} Level;

void level_load(Level* lvl, char* file_name);

#endif
