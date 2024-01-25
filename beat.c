#include "beat.h"

void beat_move(Beat* b, const float speed, Uint64 dt)
{
	b->x -= speed * dt;
	b->sprite.x = b->x;
	b->sprite.y = b->y;
}
