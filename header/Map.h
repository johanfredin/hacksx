#ifndef PSX_MAP_H
#define PSX_MAP_H

#include "GameObject.h"

/**
 * Allocates heap for passed in collision block
 */
#define MAP_MALLOC_COLLISION_BLOCK(cb, bounds_cnt)           \
    cb = MEM_MALLOC_3(CollisionBlock);                       \
    cb->bounds = MEM_CALLOC_3(bounds_cnt, RECT)

void map_init(u_char level);
void map_tick(Player *player);
void map_draw(Player *player);

#endif // PSX_MAP_H