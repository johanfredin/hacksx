#ifndef PSX_TILEFETCHER_H
#define PSX_TILEFETCHER_H

#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

#include "Frames.h"
#include "Tiled.h"

typedef struct TF_TileSet {
    char *source;
    GsSPRITE *sprite;
    u_short start_id;
} TF_TileSet;

void tf_add_layers_to_frame(Frame *frame, struct TF_TileSet **tile_sets, u_char n_tilesets, Tile_Map *map);

#endif //PSX_TILEFETCHER_H
