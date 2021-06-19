//
// Created by lowrider on 5/31/2021.
//

#include <MemUtils.h>
#include <AssetManager.h>
#include <StrUtils.h>
#include "TileFetcher.h"

SpriteLayer *init_sprite_layer() {
    SpriteLayer *sl = MEM_MALLOC_3(SpriteLayer);
    sl->sprites = NULL;
    sl->sprites_cnt = 0;
    sl->next = NULL;
    return sl;
}

u_short to_tm_u_coord(u_short id, u_short tileset_cols) {
    while (id >= tileset_cols) {
        id -= tileset_cols;
    }
    return id;
}

u_short to_tm_v_coord(u_short id, u_short tileset_cols) {
    u_short v = 0;
    while (id >= tileset_cols) {
        v++;
        id -= tileset_cols;
    }
    return v;
}

void tf_add_layers_to_frame(Frame *frame, GsSPRITE *tile_set, Tile_Map *map) {
    Tile_Layer *tl_curr;
    SpriteLayer *bg_layer = NULL;
    SpriteLayer *fg_layer = NULL;

    u_short i, n_layers_bg = 0, n_layers_fg = 0, idx_bg = 0, idx_fg = 0;

    // Iterate all layers
    for (tl_curr = map->layers; tl_curr != NULL; tl_curr = tl_curr->next) {
        SpriteLayer sl;
        GsSPRITE **layer_sprites = MEM_CALLOC_3_PTRS(tl_curr->active_sprites_cnt, GsSPRITE);
        Layer_Data *ld_curr;
        u_short cols_cnt = 0;
        u_short tiles_cnt = 0;
        u_short rows_cnt = 0;
        u_short active_tiles_cnt = 0;

        logr_log(DEBUG, "TileFetcher", "tl_get_layers", "    FETCHING LAYER %s       ", tl_curr->name);

        // Iterate the layer data to get tileset position for each tile
        for (ld_curr = tl_curr->data; ld_curr != NULL; ld_curr = ld_curr->next) {

            u_short u, v, x, y;
            u_short id = ld_curr->id;

            if (cols_cnt >= map->width) {
                cols_cnt = 0;
                rows_cnt += 1;
            }

            if (id == 0) {
                logr_log(DEBUG, "TileFetcher", "tl_get_layers", "skipping null tile at n_layers_total=%d", tiles_cnt);
            } else {
                GsSPRITE *sprite = MEM_MALLOC_3(GsSPRITE);
                x = cols_cnt * map->tile_width;
                y = rows_cnt * map->tile_height;

                --id;   // because they are 1 indexed in exported json, but tileset is 0 indexed
                u = map->tile_width * to_tm_u_coord(id, map->tile_width);
                v = map->tile_height * to_tm_v_coord(id, map->tile_height);

                asmg_get_region(tile_set, sprite, x, y, u, v, map->tile_width, map->tile_height);
                LOGR_LOG_GS_OBJ(DEBUG, sprite);
                layer_sprites[active_tiles_cnt] = sprite;
                active_tiles_cnt++;
            }
            tiles_cnt++;
            cols_cnt++;
        }

        sl.sprites = layer_sprites;
        sl.sprites_cnt = active_tiles_cnt;
//        sl.prio = tl_curr->prio;
//        sl.type = tl_curr->layer_type;
        if (tl_curr->layer_type == NULL) {
            logr_log(ERROR, "TilesFetcher.C", "tf_add_layers_to_frame", "Layer type must not be NULL, exiting...");
            exit(1);
        } else if(STREQ(tl_curr->layer_type, "bg")) {
            n_layers_bg++;
        } else if(STREQ(tl_curr->layer_type, "fg")) {
            n_layers_fg++;
        } else {
            logr_log(ERROR, "TilesFetcher.C", "tf_add_layers_to_frame", "layer type %d not a valid type, must be either bg or fg, exiting...", sl.type);
            exit(1);
        }
    }

    // Split into sub-groups for bg and fg layers
//    bg_layers = MEM_CALLOC_3(n_layers_bg, SpriteLayer);
//
//    if(n_layers_fg > 0) {   // Fg layers not required
//        fg_layers = MEM_CALLOC_3(n_layers_fg, SpriteLayer);
//    }
//
//    for (i = 0; i < n_layers_total; i++) {
//        SpriteLayer sl = sprite_layers[i];
//        if (STREQ(sl.type, "bg")) {
//            bg_layers[idx_bg++] = sl;
//        } else {    // We have already thrown error above if layer not one of (bg, fg) so no point in checking again
//            fg_layers[idx_fg++] = sl;
//        }
//        MEM_FREE_3_AND_NULL(sl);
    }
//    MEM_FREE_3_AND_NULL(sprite_layers);

    // Now we have our 2 arrays, time to sort on prio so they will be drawn in correct order
//    qsort(bg_layers, n_layers_bg, sizeof(SpriteLayer), compareSpriteLayers);

//    if(n_layers_fg > 0) {   // Fg layers not required
//        qsort(fg_layers, n_layers_fg, sizeof(SpriteLayer), compareSpriteLayers);
//    }

    // Finally give layers to frame
//    frame->bg_layers = bg_layers;
//    frame->fg_layers = fg_layers;
//    frame->n_layers_fg = n_layers_fg;
//    frame->n_layers_bg = n_layers_bg;
//}
