#include "score.h"

int level_score = 0;

int great_streak = 0;

void score_increment(BeatScore s)
{
	level_score += (int)s;
	printf("score: %d\n\n", level_score);
}

void score_streak_increment()
{
	great_streak += 1;
}

void score_streak_reset()
{
	great_streak = 0;
}

int score_get_streak()
{
	return great_streak;
}

