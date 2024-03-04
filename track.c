#include "track.h"

int track_press(Track* t)
{
	if (t == NULL) return FALSE;

	if (t->beats[t->cur_beat].x - t->x <= t->sprite.w)
	{
		BeatScore b = GOOD;
		score_increment(b);
		t->cur_beat++;
		return TRUE;
	}
	else return FALSE;
}

void track_init(Track* t, int x, int y)
{
	if (t == NULL) return;

	t->x = x;
	t->y = y;
	t->sprite.x = t->x;
	t->sprite.y = t->y;
	t->sprite.w = 80;
	t->sprite.h = 80;
}
