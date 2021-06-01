//
// Created by lowrider on 5/31/2021.
//

#include <AssetManager.h>
#include "TileFetcher.h"

u_short to_tm_u_coord(u_short id, u_short tileset_cols) {
    while(id >= tileset_cols) {
        id -= tileset_cols;
    }
    return id;
}

u_short to_tm_v_coord(u_short id, u_short tileset_cols, u_char curr_idx) {
    u_short u = 0;
    while(curr_idx >= tileset_cols) {
        u++;
        curr_idx -= tileset_cols;
    }
    return u;
}

GsSPRITE *tiles_get_gs_tile_sprites_mapped(GsSPRITE *tile_set, Tile_Map *map) {
    u_char i, x, y;
    u_short rows_cnt, cols_cnt;
    Layer_Data *data;
    GsSPRITE *sprites = MEM_CALLOC_3(256, GsSPRITE);
    Tile_Layer *layer = map->layers;  // Update later to parse multiple layers
    u_short tileset_rows = tile_set->h / map->tile_height;
    u_short tileset_cols = tile_set->w / map->tile_width;

    rows_cnt = 0;
    cols_cnt = 0;
    for (data = layer->data, i = 0; data != NULL; data = data->next, i++, cols_cnt++) {
        if(cols_cnt >= map->tile_width) {
            cols_cnt = 0;
            rows_cnt += 1;
        }

        u_short u, v;
        u_short id = data->id;
        if (id == 0) {
            logr_log(INFO, "Main", "get_gs_tile_sprites_mapped", "skipping null tile");
            continue;
        }

        x = cols_cnt * map->tile_width;
        y = rows_cnt * map->tile_height;

        --id;   // because they are 1 indexed in exported json, but tileset is 0 indexed
        u = map->tile_width * to_tm_u_coord(id, tileset_cols);
        v = map->tile_height * to_tm_v_coord(id, tileset_rows, i);

        asmg_get_region(tile_set, &sprites[i], x, y, u, v, map->tile_width, map->tile_height);
    }
    return sprites;
}
