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
    sprite->x = x;
    sprite->y = y;
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
    sprite->rotate = GPUB_ROT_ONE * 0; // Rotation, ROT_ONE * (0 to 360)
    sprite->mx = 0;               // Rotation x coord
    sprite->my = 0;               // Rotation y coord
    sprite->scalex = ONE * 1;
    sprite->scaley = ONE * 1;

    logr_log(DEBUG, "AssetManager.c", "asmg_load_sprite", "w=%d, h=%d", sprite->w, sprite->h);
    logr_log(DEBUG, "AssetManager.c", "asmg_load_sprite", "cx=%d, cy=%d", sprite->cx, sprite->cy);
    logr_log(DEBUG, "AssetManager.c", "asmg_load_sprite", "u=%d, v=%d", sprite->u, sprite->v);

    MEM_FREE_3_AND_NULL(asset);
    return sprite;
}

GsSPRITE *asmg_load_sprite_w_offset(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits, u_short offset_x, u_short offset_y) {
    return asmg_load_sprite(cdr_data, x + offset_x, y + offset_y, blend, num_color_bits);
}

void asmg_get_region(GsSPRITE *sprite, GsSPRITE *region, u_short u, u_short v, u_short w, u_short h) {
    region->x = sprite->x;
    region->y = sprite->y;
    region->u = u;
    region->v = v;
    region->attribute = sprite->attribute;
    region->rotate = sprite->rotate;
    region->tpage = sprite->tpage;
    region->scalex = sprite->scalex;
    region->scaley = sprite->scaley;
    region->rotate = sprite->rotate;
    region->w = w;
    region->h = h;
    region->r = sprite->r;
    region->g = sprite->g;
    region->b = sprite->b;
    region->cx = sprite->cx;
    region->cy = sprite->cy;
    region->mx = sprite->mx;
    region->my = sprite->my;
}
