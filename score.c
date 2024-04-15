#include "score.h"

int level_score = 0;

int great_streak = 0;

int miss_streak = 0;

int miss_repair = 0;

int score_miss_increment()
{
	miss_streak++;
	printf("misses: %d\n\n", miss_streak);
	if (miss_streak > 5) return TRUE;
	else return FALSE;
}

void score_miss_decrement()
{
	miss_repair++;
	if (miss_streak > 0 && miss_repair > 1)
	{
		miss_streak--;
		miss_repair = 0;
	}
	printf("misses: %d\n\n", miss_streak);
}

void score_miss_reset()
{
	miss_streak = 0;
	miss_repair = 0;
}

void score_increment(BeatScore s)
{
	level_score += (int)s;
	printf("score: %d\n\n", level_score);
	score_miss_decrement();
	menu_update_score(level_score);
}

void score_reset()
{
	level_score = 0;
}

int get_level_score()
{
	return level_score;
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

