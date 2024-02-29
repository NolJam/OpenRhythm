#ifndef GAME_DEF
#define GAME_DEF

typedef enum BeatScore {
	MISS,
	OK,
	GOOD,
	GREAT,
	PERFECT,
} BeatScore;

extern int level_score;

void score_increment(BeatScore s);

#endif
