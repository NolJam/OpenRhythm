#ifndef LEVEL_DEF
#define LEVEL_DEF

#include <stdio.h>
#include "globals.h"
#include "beat.h"
#include "track.h"
#include "score.h"

extern const char* BEAT_FORMAT;
extern const char* MEASURE_FORMAT;
extern const char* BPM_FORMAT;
extern const char* TRACK_FORMAT;

typedef struct Level {
	//Beat* beats; // dynamically allocated 10 beat chunks
	Track tracks[5]; // 5 tracks total
	//int cur_beat;
	//int num_beats;
	int num_tracks;
	float bpm;
	float speed;
} Level;

void level_load(Level* lvl, char* file_name);

void level_free(Level* lvl);

#endif
