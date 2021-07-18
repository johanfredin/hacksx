#ifndef PSX_FRAMES_H
#define PSX_FRAMES_H

#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

#include "GameObject.h"

typedef struct Teleport {
    RECT origin;
    u_short dest_x, dest_y;
    u_char dest_frame;  // frame array index of frame to change to when colliding with this teleport
} Teleport;

typedef struct CollisionBlock {
    RECT *bounds;         // The actual physical bounds that we will collide with in the frame
    u_char amount;        // The amount of blocks on one frame
} CollisionBlock;

typedef struct SpriteLayer {
    GsSPRITE **sprites;
    u_short sprites_cnt;
    u_short prio;
    struct SpriteLayer *next;
} SpriteLayer;

typedef struct FR_TileSet {
    char *source;
    GsSPRITE *sprite;
    u_short start_id;
    struct FR_TileSet *next;
} FR_TileSet;

typedef struct Frame {
    SpriteLayer *bg_layers, *fg_layers;
    u_char t_amount, offset_x, offset_y;
    CollisionBlock *collision_blocks;
    Teleport *teleports;
    GameObject *game_object;
    FR_TileSet *tf_tilesets;
} Frame;

Frame *frames_malloc();
void frames_insert_sl_sorted(SpriteLayer **root, SpriteLayer *new_layer);

#endif
