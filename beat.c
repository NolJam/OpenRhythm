#include "beat.h"

void beat_move(Beat* b, const float speed)
{
	if (b == NULL) return;

	if (b->x < -100.0f) return;

	b->x -= speed;
	b->sprite.x = b->x;
	b->sprite.y = b->y;
}
