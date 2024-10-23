#ifndef LEVEL_DEF
#define LEVEL_DEF

#include <stdio.h>
#include <SDL_mixer.h>
#include "globals.h"
#include "beat.h"
#include "track.h"
#include "score.h"
#include "files.h"

//char** level_names;

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
	double start_pos;
} Level;

void level_init();

void level_load(Level* lvl, int lvl_num);

char* level_get_name(int lvl_num);

void level_free(Level* lvl);

#endif
