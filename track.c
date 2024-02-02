#include "track.h"

int track_press(Track* t)
{
	if (t->beats[t->cur_beat].x - t->x <= t->sprite.w && t->beats[t->cur_beat].y == t->y)
	{
		printf("beat hit!\n\n");
		t->cur_beat++;
		return TRUE;
	}
	else return FALSE;
}

void track_init(Track* t, int x, int y)
{
	t->x = x;
	t->y = y;
	t->sprite.x = t->x;
	t->sprite.y = t->y;
	t->sprite.w = 64;
	t->sprite.h = 64;
}
