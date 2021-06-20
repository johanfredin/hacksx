#ifndef PSX_FRAMES_H
#define PSX_FRAMES_H

#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

#include "GameObject.h"

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
    u_short prio;
    struct SpriteLayer *next;
} SpriteLayer;

typedef struct Frame {
    SpriteLayer *bg_layers, *fg_layers;
    u_char t_amount;
    CollisionBlock *collision_blocks;
    Teleport *teleports;
    GameObject *game_object;
} Frame;

void frames_insert_sl_sorted(SpriteLayer **root, SpriteLayer *new_layer);

#endif
