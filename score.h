#ifndef GAME_DEF
#define GAME_DEF

#include <stdio.h>
#include "globals.h"

typedef enum BeatScore {
	MISS,
	OK,
	GOOD,
	GREAT,
	PERFECT,
} BeatScore;

void score_increment(BeatScore s);

void score_reset();

void score_streak_increment();

void score_streak_reset();

int score_get_streak();

#endif
