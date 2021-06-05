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

u_short to_tm_v_coord(u_short id, u_short tileset_cols) {
    u_short v = 0;
    while(id >= tileset_cols) {
        v++;
        id -= tileset_cols;
    }
    return v;
}

GsSPRITE **tiles_get_gs_tile_sprites_mapped(GsSPRITE *tile_set, Tile_Map *map) {
    u_short i, rows_cnt, cols_cnt;
    Layer_Data *data;
    Tile_Layer *layer = map->layers;  // Update later to parse multiple layers
    GsSPRITE **sprites = MEM_CALLOC_3_PTRS(map->tile_width * map->tile_height, GsSPRITE);

    rows_cnt = 0;
    cols_cnt = 0;
    for (data = layer->data, i = 0; data != NULL; data = data->next, i++, cols_cnt++) {
        u_short u, v, x, y;
        u_short id = data->id;

        if(cols_cnt >= map->width) {
            cols_cnt = 0;
            rows_cnt += 1;
        }

        if (id == 0) {
            logr_log(INFO, "TileFetcher", "get_gs_tile_sprites_mapped", "skipping null tile at i=%d", i);
            sprites[i] = NULL;
            continue;
        }

        sprites[i] = MEM_MALLOC_3(GsSPRITE);
        x = cols_cnt * map->tile_width;
        y = rows_cnt * map->tile_height;

        --id;   // because they are 1 indexed in exported json, but tileset is 0 indexed
        u = map->tile_width * to_tm_u_coord(id, map->tile_width);
        v = map->tile_height * to_tm_v_coord(id, map->tile_height);

        asmg_get_region(tile_set, sprites[i], x, y, u, v, map->tile_width, map->tile_height);
        logr_log(DEBUG, "TileFetcher", "get_gs_tile_sprites_mapped", "Tile retrieved ID=%d, i=%d: {x:%d, y%d, u:%d, v:%d}", id, i, sprites[i]->x, sprites[i]->y, sprites[i]->u, sprites[i]->v);
    }
    return sprites;
}
