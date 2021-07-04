#ifndef PSX_TILEFETCHER_H
#define PSX_TILEFETCHER_H

#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

#include "Frames.h"
#include "Tiled.h"

void tf_add_layers_to_frame(Frame *frame, GsSPRITE *tile_sets, u_char tileset_count, Tile_Map *map);

#endif //PSX_TILEFETCHER_H
