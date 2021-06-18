//
// Created by lowrider on 6/13/2021.
//

#ifndef HACKSX_FRAMES_H
#define HACKSX_FRAMES_H

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


#endif //HACKSX_FRAMES_H
