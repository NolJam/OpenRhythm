#ifndef GAME_DEF
#define GAME_DEF

#include "globals.h"

typedef enum BeatScore {
	MISS,
	OK,
	GOOD,
	GREAT,
	PERFECT,
} BeatScore;

extern int level_score;

int great_streak;

void score_increment(BeatScore s);

void score_streak_increment();

void score_streak_reset();

void score_get_streak();

#endif
