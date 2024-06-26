#ifndef GAME_DEF
#define GAME_DEF

#include <stdio.h>
#include "globals.h"
#include "menu.h"

typedef enum BeatScore {
	MISS,
	OK,
	GOOD,
	GREAT,
	PERFECT,
} BeatScore;

int score_miss_increment();

void score_miss_decrement();

void score_miss_reset();

void score_increment(BeatScore s);

void score_reset();

int get_level_score();

void score_streak_increment();

void score_streak_reset();

int score_get_streak();

#endif
