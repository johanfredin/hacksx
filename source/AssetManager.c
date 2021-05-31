#include "../header/AssetManager.h"


void asmg_load_asset(VramAsset *asset, CdrData *cdr_data, u_short num_color_bits) {
    logr_log(INFO, "AssetManager.c", "asmg_load_asset", "Loading asset=%s", cdr_data->name);
}

GsSPRITE *asmg_load_sprite(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits) {
    VramAsset *asset;
    GsSPRITE *sprite;

    asset = MEM_MALLOC_3(VramAsset);
    asmg_load_asset(asset, cdr_data, num_color_bits);
    sprite = MEM_MALLOC_3(GsSPRITE);
    sprite->attribute = 1L;
    sprite->x = (short) x;
    sprite->y = (short) y;
    sprite->w = 32;
    sprite->h = 32;
    sprite->tpage = GetTPage(asset->color_mode, 1, 32, 32);
    sprite->u = 0; //(asset->frame_buffer->x * asset->twidthMultiplier) % 256;
    sprite->v = 0;
    if (asset->is_clut_mode) {
        sprite->cx = 1;
        sprite->cy = 1;
    }

    sprite->r = sprite->g = sprite->b = blend;
    sprite->rotate = GPUB_ROT_ONE * 0; // Rotation, ASMG_ROT_ONE * (0 to 360)
    sprite->mx = 0;               // Rotation x coord
    sprite->my = 0;               // Rotation y coord
    sprite->scalex = ONE * 1;
    sprite->scaley = ONE * 1;

    logr_log(DEBUG, "AssetManager.c", "asmg_load_sprite", "w=%d, h=%d", sprite->w, sprite->h);
    logr_log(DEBUG, "AssetManager.c", "asmg_load_sprite", "cx=%d, cy=%d", sprite->cx, sprite->cy);
    logr_log(DEBUG, "AssetManager.c", "asmg_load_sprite", "u=%d, v=%d", sprite->u, sprite->v);

    ASMG_FREE_VRAM_ASSET(asset);
    return sprite;
}

GsSPRITE *asmg_load_sprite_w_offset(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits,
                                    u_short offset_x, u_short offset_y) {
    return asmg_load_sprite(cdr_data, x + offset_x, y + offset_y, blend, num_color_bits);
}

void asmg_get_region(GsSPRITE *base, GsSPRITE *region, u_short x, u_short y, u_short u, u_short v, u_short w, u_short h) {
    region->attribute = base->attribute;
    region->x = x;
    region->y = y;
    region->w = w;
    region->h = h;
    region->tpage = base->tpage;
    region->u = u;
    region->v = v;
    region->cx = base->cx;
    region->cy = base->cy;
    GPUB_SET_BRIGHTNESS(region, 128);
    region->rotate = GPUB_ROT_ONE * 0; // Rotation, ROT_ONE * (0 to 360)
    region->mx = 0;                    // Rotation x coord
    region->my = 0;                    // Rotation y coord
    region->scalex = ONE * 1;
    region->scaley = ONE * 1;
}

GsBG *asmg_get_gs_bg(Tile_Map *tm, CdrData *sprite_data) {
    GsSPRITE *tile_set;
    GsBG *gs_bg;
    GsMAP *gs_map;
    GsCELL *gs_cells;
    u_short x, y, i;
    Layer_Data *curr_layer = tm->layers[0].data; //TODO: Fix so that several layers can be accessed later

    // Load tile_set
    tile_set = asmg_load_sprite(sprite_data, 0, 0, 128, ASMG_COLOR_BITS_8);

    // Init BG
    gs_bg = MEM_MALLOC_3(GsBG);
    gs_bg->x = gs_bg->y = 0;
    gs_bg->w = (short) (tm->tile_width * tm->width);
    gs_bg->h = (short) (tm->tile_height * tm->height);
    gs_bg->scrollx = gs_bg->scrolly = 0;
    gs_bg->attribute = tile_set->attribute;
    GPUB_SET_BRIGHTNESS(gs_bg, 128);

    // Init map
    gs_map = MEM_MALLOC_3(GsMAP);
    gs_map->cellw = tm->tile_width;
    gs_map->cellh = tm->tile_height;
    gs_map->ncellw = tm->width;
    gs_map->ncellh = tm->height;

    // Init cells
    gs_cells = MEM_CALLOC_3( gs_map->ncellw * gs_map->ncellh, GsCELL);

    gs_map->base = gs_cells;
    gs_map->index = (unsigned short *) &gs_cells;

    gs_bg->map = gs_map;
    i = 0;

    // Iterate frame tile by tile and fetch appropriate texture region from tile set
    for (y = 0; y < gs_map->ncellh; y++) {
        for (x = 0; x < gs_map->ncellw; x++) {
            if (curr_layer == NULL) {
                logr_log(ERROR, "AssetManager.c", "asmg_get_gs_bg", "Current layer data entry is null before end of loop, must be a mismatch, terminating...");
                exit(1);
            }

            if (curr_layer->id == 0) {
                logr_log(DEBUG, "AssetManager.c", "asmg_get_gs_bg", "id=0 data entry found at i=%d, skipping since 0=no tile", i);
            } else {
                GsCELL cell;
                cell.u = 0;
                cell.v = 0;
                cell.tpage = tile_set->tpage;
                cell.cba = GetClut(x * gs_map->cellw, y * gs_map->cellh);
                cell.flag = 0;
                gs_cells[i] = cell;
            }
            i++;
            curr_layer = curr_layer->next;
        }
    }
    return gs_bg;
}
