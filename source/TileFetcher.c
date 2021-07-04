#include "../header/TileFetcher.h"
#include "../header/MemUtils.h"
#include "../header/AssetManager.h"
#include "../header/StrUtils.h"
#include "../header/Logger.h"

SpriteLayer *init_sprite_layer() {
    SpriteLayer *sl = MEM_MALLOC_3(SpriteLayer);
    sl->sprites = NULL;
    sl->next = NULL;
    sl->sprites_cnt = 0;
    sl->prio = 0;
    return sl;
}

u_short to_tm_u_coord(u_short id, u_short cols, u_short tile_w);
u_short to_tm_v_coord(u_short id, u_short rows, u_short tile_h);
GsSPRITE *map_tile(u_char tileset_idx, u_short id, u_short x, u_short y, GsSPRITE *tile_sets, u_char tileset_count, Tile_Map *map);

void tf_add_layers_to_frame(Frame *frame, GsSPRITE *tile_sets, u_char tileset_count, Tile_Map *map) {
    SpriteLayer *root_bg_layer = NULL;
    SpriteLayer *root_fg_layer = NULL;
    Tile_Layer *tl_curr;

    logr_log(DEBUG, "TileFetcher.c", "tl_get_layers", "Entered function");

    // Iterate all layers
    for (tl_curr = map->layers; tl_curr != NULL; tl_curr = tl_curr->next) {
        Layer_Data *ld_curr;
        u_short curr_col = 0;
        u_short tiles_cnt = 0;
        u_short rows_cnt = 0;
        u_short active_tiles_cnt = 0;
        SpriteLayer *sl = init_sprite_layer();
        GsSPRITE **layer_sprites = MEM_CALLOC_3_PTRS(tl_curr->active_sprites_cnt, GsSPRITE);

        if (tl_curr->layer_type == NULL) {
            logr_log(ERROR, "TileFetcher.c", "tf_add_layers_to_frame", "Layer type must not be NULL, exiting...");
            exit(1);
        }

        logr_log(DEBUG, "TileFetcher.c", "tl_get_layers", "FETCHING LAYER %s", tl_curr->name);

        // Iterate the layer data to get tileset position for each tile
        for (ld_curr = tl_curr->data; ld_curr != NULL; ld_curr = ld_curr->next) {
            u_short id = ld_curr->id;

            if (curr_col >= map->width) {
                curr_col = 0;
                rows_cnt += 1;
            }

            if (id == 0) {
                logr_log(TRACE, "TileFetcher.c", "tl_get_layers", "skipping null tile at n_layers_total=%d", tiles_cnt);
            } else {
                u_short x = curr_col * map->tile_width;
                u_short y = rows_cnt * map->tile_height;
                --id;   // because they are 1 indexed in exported json, but tileset is 0 indexed

                GsSPRITE *sprite = map_tile(0, id, x, y, tile_sets, tileset_count, map);
                if(sprite == NULL) {
                    logr_log(ERROR, "TileFetcher.c", "tl_get_layers", "Id of col=%d at layer=%s exceeds max dimensions for tilesets, terminating...", id, tl_curr->layer_type);
                    exit(1);
                }
                layer_sprites[active_tiles_cnt++] = sprite;
            }
            tiles_cnt++;
            curr_col++;
        }

        sl->sprites = layer_sprites;
        sl->sprites_cnt = active_tiles_cnt;
        sl->prio = tl_curr->prio;
        if(STREQ(tl_curr->layer_type, "bg")) {
            frames_insert_sl_sorted(&root_bg_layer, sl);
            logr_log(DEBUG, "TileFetcher.c", "tf_add_layers_to_frame", "Bg layer with prio=%d added to frame", sl->prio);
        } else if(STREQ(tl_curr->layer_type, "fg")) {
            frames_insert_sl_sorted(&root_fg_layer, sl);
            logr_log(DEBUG, "TileFetcher.c", "tf_add_layers_to_frame", "Fg layer with prio=%d added to frame", sl->prio);
        } else {
            logr_log(ERROR, "TileFetcher.c", "tf_add_layers_to_frame", "layer type %d not a valid type, must be either bg or fg, exiting...", tl_curr->layer_type);
            exit(1);
        }
    }

    // Finally give layers to frame
    frame->bg_layers = root_bg_layer;
    frame->fg_layers = root_fg_layer;
}

GsSPRITE *map_tile(u_char tileset_idx, u_short id, u_short x, u_short y, GsSPRITE *tile_sets, u_char tileset_count, Tile_Map *map) {
    u_short u, v;
    GsSPRITE *curr_tileset = &tile_sets[tileset_idx];
    curr_tileset->w = curr_tileset->h = 512;
    u_short tileset_tw = curr_tileset->w / map->tile_width;
    u_short tileset_th = curr_tileset->h / map->tile_height;
    u_short max_dimension = tileset_tw * tileset_th;
    GsSPRITE *tile;

    if(id >= max_dimension) {
        if(tileset_idx >= tileset_count) {
            return NULL; // Return NULL if we have exceeded all tilesets and still not within range
        }
        return map_tile(++tileset_idx, id, x, y, tile_sets, tileset_count, map);
    }

    tile = MEM_MALLOC_3(GsSPRITE);
    u = to_tm_u_coord(id, tileset_tw, map->tile_width);
    v = to_tm_v_coord(id, tileset_th, map->tile_height);

    asmg_get_region(curr_tileset, tile, x, y, u, v, map->tile_width, map->tile_height);
    LOGR_LOG_GS_OBJ(TRACE, tile);
    return tile;
}

u_short to_tm_u_coord(u_short id, u_short cols, u_short tile_w) {
    while (id >= cols) {
        id -= cols;
    }
    return id * tile_w;
}

u_short to_tm_v_coord(u_short id, u_short rows, u_short tile_h) {
    u_short v = 0;
    while (id >= rows) {
        v++;
        id -= rows;
    }
    return v * tile_h;
}
