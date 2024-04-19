#include "track.h"

int track_press(Track* t)
{
	if (t == NULL || t->cur_beat >= t->num_beats) return FALSE;

	if (t->beats[t->cur_beat].x - t->x <= t->sprite.w)
	{
		BeatScore b = GOOD;
		score_increment(b);
		t->cur_beat++;
		return TRUE;
	}
	return FALSE;
}

void track_init(Track* t, int x, int y)
{
	if (t == NULL) return;

	t->x = x;
	t->y = y;
	t->sprite.x = t->x - 8;
	t->sprite.y = t->y - 8;
	t->sprite.w = 80;
	t->sprite.h = 80;
}
