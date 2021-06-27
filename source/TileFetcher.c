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
    tile_set->w = tile_set->h = 512;

    Tile_Layer *tl_curr;
    SpriteLayer *root_bg_layer = NULL;
    SpriteLayer *root_fg_layer = NULL;

    logr_log(DEBUG, "TileFetcher.c", "tl_get_layers", "Entered funtion");

    // Iterate all layers
    for (tl_curr = map->layers; tl_curr != NULL; tl_curr = tl_curr->next) {
        if (tl_curr->layer_type == NULL) {
            logr_log(ERROR, "TileFetcher.c", "tf_add_layers_to_frame", "Layer type must not be NULL, exiting...");
            exit(1);
        }

        SpriteLayer *sl = init_sprite_layer();
        GsSPRITE **layer_sprites = MEM_CALLOC_3_PTRS(tl_curr->active_sprites_cnt, GsSPRITE);
        Layer_Data *ld_curr;
        u_short cols_cnt = 0;
        u_short tiles_cnt = 0;
        u_short rows_cnt = 0;
        u_short active_tiles_cnt = 0;

        logr_log(DEBUG, "TileFetcher.c", "tl_get_layers", "FETCHING LAYER %s", tl_curr->name);

        // Iterate the layer data to get tileset position for each tile
        for (ld_curr = tl_curr->data; ld_curr != NULL; ld_curr = ld_curr->next) {
            u_short u, v, x, y;
            u_short id = ld_curr->id;
            u_short max_dimension = ((tile_set->w / map->tile_width) * (tile_set->h / map->tile_height));

            if (cols_cnt >= map->width) {
                cols_cnt = 0;
                rows_cnt += 1;
            }

            if (id == 0) {
                logr_log(TRACE, "TileFetcher.c", "tl_get_layers", "skipping null tile at n_layers_total=%d", tiles_cnt);
            } else {
                GsSPRITE *sprite = MEM_MALLOC_3(GsSPRITE);
                x = cols_cnt * map->tile_width;
                y = rows_cnt * map->tile_height;

                --id;   // because they are 1 indexed in exported json, but tileset is 0 indexed

                if(id >= max_dimension) {
                    logr_log(ERROR, "TileFetcher.c", "tl_get_layers", "Id of col=%d at layer=%s exceeds max dimensions=%d for tileset, terminating...", id, tl_curr->layer_type, max_dimension);
                    exit(1);
                }

                u = map->tile_width * to_tm_u_coord(id, map->tile_width);
                v = map->tile_height * to_tm_v_coord(id, map->tile_height);

                asmg_get_region(tile_set, sprite, x, y, u, v, map->tile_width, map->tile_height);
                LOGR_LOG_GS_OBJ(TRACE, sprite);
                layer_sprites[active_tiles_cnt] = sprite;
                active_tiles_cnt++;
            }
            tiles_cnt++;
            cols_cnt++;
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

