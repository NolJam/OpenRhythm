#include "track.h"

int track_press(Track* t, Beat* b)
{
	if (b->x - t->x <= t->sprite.w)
	{
		printf("beat hit!\n\n");
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
