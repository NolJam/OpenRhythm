#include "beat.h"

void beat_move(Beat* b, const float speed, Uint64 dt)
{
	if (b->x < -100.0f) return;

	b->x -= speed * dt;
	b->sprite.x = b->x;
	b->sprite.y = b->y;
}
