#ifndef PSX_GRIDMAP_H
#define PSX_GRIDMAP_H

#include "GameObject.h"

/**
 * Allocates heap for passed in collision block, its bounds
 * and (if debug render mode) the bound line tiles
 */
#define MAP_MALLOC_COLLISION_BLOCK(cb, bounds_cnt)           \
    cb = MEM_MALLOC_3(CollisionBlock);                       \
    if (GPUB_DRAW_BOUNDS) {                                  \
        cb->cb_bound_lines = MEM_CALLOC_3(bounds_cnt, TILE); \
    }                                                        \
    cb->bounds = MEM_CALLOC_3(bounds_cnt, RECT)

typedef struct Teleport {
    RECT origin;
    short dest_x, dest_y;
    TILE t_bound_lines; // Graphical representation of a block (if DebugMode)
    u_char dest_frame;  // frame array index of frame to change to when colliding with this teleport
} Teleport;

typedef struct CollisionBlock {
    RECT *bounds;         // The actual physical bounds that we will collide with in the frame
    u_char amount;        // The amount of blocks on one frame
    TILE *cb_bound_lines; // Graphical representation of a block (if DebugMode)
} CollisionBlock;

typedef struct SpriteLayer {
    GsSPRITE **sprites;
    u_short sprites_cnt;
    char *type;
    u_char prio;
} SpriteLayer;

typedef struct Frame {
    SpriteLayer **bg_layers, **fg_layers;
    u_char n_layers_bg, n_layers_fg, t_amount;
    CollisionBlock *collision_blocks;
    Teleport *teleports;
    GameObject *game_object;
} Frame;

void map_init(u_char level);
void map_tick(Player *player);
void map_draw(Player *player);

#endif // PSX_GRIDMAP_H