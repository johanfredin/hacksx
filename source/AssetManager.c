#include <LIBSPU.H>

#include "../header/AssetManager.h"
#include "../header/GPUBase.h"
#include "../header/Logger.h"
#include "../header/MemUtils.h"

u_char get_tpage_color_bit_mode(u_short num_color_bits);
u_char get_width_by_color_bits_mode(u_short num_color_bits);
u_long get_attribute_by_color_bits_mode(u_short num_color_bits);

long vag_spu_addr;

void asmg_init_tim_data(GsIMAGE *tim_data, CdrData *cdr_data, u_short num_color_bits) {
    // Declarations
    RECT *frame_buffer;
    RECT *clut;
    u_char *data;

    logr_log(DEBUG, "AssetManager.c", "asmg_init_tim_data", "------------------------");
    logr_log(DEBUG, "AssetManager.c", "asmg_init_tim_data", "Fetching asset=%s", cdr_data->name);
    logr_log(DEBUG, "AssetManager.c", "asmg_init_tim_data", "Color bits=%d", num_color_bits);

    // Load image data
    data = (u_char *)cdr_data->file;

    tim_data = MEM_MALLOC_3(GsIMAGE);
    GsGetTimInfo((u_long *)(data + 4), tim_data);

    // MEM_MALLOC_3 resources
    frame_buffer = MEM_MALLOC_3(RECT);
    clut = MEM_MALLOC_3(RECT);

    // Load image into gpu memory
    frame_buffer->x = tim_data->px;
    frame_buffer->y = tim_data->py;
    frame_buffer->w = (short) tim_data->pw;
    frame_buffer->h = (short) tim_data->ph;
    LoadImage(frame_buffer, tim_data->pixel);
    logr_log(DEBUG, "AssetManager.c", "asmg_init_tim_data", "Framebuffer coords assigned at {x:%d, y:%d, w:%d, h:%d}", frame_buffer->x, frame_buffer->y, frame_buffer->w, frame_buffer->h);

    if (num_color_bits < ASMG_COLOR_BITS_16) {
        // load clut into gpu memory
        clut->x = tim_data->cx;
        clut->y = tim_data->cy;
        clut->w = (short) tim_data->cw;
        clut->h = (short) tim_data->ch;
        LoadImage(clut, tim_data->clut);
        logr_log(DEBUG, "AssetManager.c", "asmg_init_tim_data", "CLUT coords assigned at {x:%d, y:%d, w:%d, h:%d}", clut->x, clut->y, clut->w, clut->h);
    } else {
        logr_log(DEBUG, "AssetManager.c", "asmg_init_tim_data", "16 bit mode so no CLUT");
    }
}

void asmg_load_asset(VramAsset *asset, CdrData *cdr_data, u_short num_color_bits) {
    // Declarations
    GsIMAGE *tim_data;
    RECT *frame_buffer;
    RECT *clut;
    u_char *data;

    // Definitions
    u_short tw_multiplier = get_width_by_color_bits_mode(num_color_bits);
    u_short color_mode = get_tpage_color_bit_mode(num_color_bits);
    u_long sprite_attr = get_attribute_by_color_bits_mode(num_color_bits);

    logr_log(DEBUG, "AssetManager.c", "asmg_load_asset", "------------------------");
    logr_log(DEBUG, "AssetManager.c", "asmg_load_asset", "Fetching asset=%s", cdr_data->name);
    logr_log(DEBUG, "AssetManager.c", "asmg_load_asset", "Color bits=%d", num_color_bits);

    // Load image data
    data = (u_char *)cdr_data->file;

    tim_data = MEM_MALLOC_3(GsIMAGE);
    GsGetTimInfo((u_long *)(data + 4), tim_data);

    // MEM_MALLOC_3 resources
    frame_buffer = MEM_MALLOC_3(RECT);
    clut = MEM_MALLOC_3(RECT);

    // Load image into gpu memory
    frame_buffer->x = tim_data->px;
    frame_buffer->y = tim_data->py;
    frame_buffer->w = (short) tim_data->pw;
    frame_buffer->h = (short) tim_data->ph;
    LoadImage(frame_buffer, tim_data->pixel);
    logr_log(DEBUG, "AssetManager.c", "asmg_load_asset", "Framebuffer coords assigned at {x:%d, y:%d, w:%d, h:%d}", frame_buffer->x, frame_buffer->y, frame_buffer->w, frame_buffer->h);

    if (num_color_bits < ASMG_COLOR_BITS_16) {
        // load clut into gpu memory
        clut->x = tim_data->cx;
        clut->y = tim_data->cy;
        clut->w = (short) tim_data->cw;
        clut->h = (short) tim_data->ch;
        LoadImage(clut, tim_data->clut);
        logr_log(DEBUG, "AssetManager.c", "asmg_load_asset", "CLUT coords assigned at {x:%d, y:%d, w:%d, h:%d}", clut->x, clut->y, clut->w, clut->h);
    } else {
        logr_log(DEBUG, "AssetManager.c", "asmg_load_asset", "16 bit mode so no CLUT");
    }

    asset->clut = clut;
    asset->frame_buffer = frame_buffer;
    asset->is_clut_mode = num_color_bits < ASMG_COLOR_BITS_16;
    asset->color_mode = color_mode;
    asset->sprite_attr = sprite_attr;
    asset->t_width_multiplier = tw_multiplier;
    MEM_FREE_3_AND_NULL(tim_data);
}

GsSPRITE *asmg_load_8_bit_sprite(CdrData *cdr_data, u_short x, u_short y) {
    return asmg_load_sprite(cdr_data, x, y, ASMG_BLEND_NORMAL, ASMG_COLOR_BITS_8);
}


GsSPRITE *asmg_load_sprite(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits) {
    VramAsset *asset;
    GsSPRITE *sprite;

    asset = MEM_MALLOC_3(VramAsset);
    asmg_load_asset(asset, cdr_data, num_color_bits);
    sprite = MEM_MALLOC_3(GsSPRITE);
    sprite->attribute = asset->sprite_attr;
    sprite->x = x;
    sprite->y = y;
    sprite->w = asset->frame_buffer->w * asset->t_width_multiplier;
    sprite->h = asset->frame_buffer->h;
    sprite->tpage = GetTPage(asset->color_mode, 1, asset->frame_buffer->x, asset->frame_buffer->y);
    sprite->u = 0;
    sprite->v = (asset->frame_buffer->y) % 256;
    if (asset->is_clut_mode) {
        sprite->cx = asset->clut->x;
        sprite->cy = asset->clut->y;
    }
    GPUB_SET_BRIGHTNESS(sprite, blend);
    sprite->rotate = GPUB_ROT_ONE * 0; // Rotation, ROT_ONE * (0 to 360)
    sprite->mx = 0;                    // Rotation x coord
    sprite->my = 0;                    // Rotation y coord
    sprite->scalex = GPUB_ROT_ONE * 1;
    sprite->scaley = GPUB_ROT_ONE * 1;

    logr_log(DEBUG, "AssetManager.c", "asmg_load_sprite", "w=%d, h=%d", sprite->w, sprite->h);
    logr_log(DEBUG, "AssetManager.c", "asmg_load_sprite", "cx=%d, cy=%d", sprite->cx, sprite->cy);
    logr_log(DEBUG, "AssetManager.c", "asmg_load_sprite", "u=%d, v=%d", sprite->u, sprite->v);

    ASMG_FREE_VRAM_ASSET(asset);
    return sprite;
}

/**
 * Crops coords and offsets passed in from base sprite and gives to the region.
 * @param base base sprite to take values from
 * @param region the sprite we want to populate
 * @param x x pos of region
 * @param y y pos of region
 * @param u x offset in base sprite texture
 * @param v y offset in base sprite texture
 * @param w width of region
 * @param h height of region
 */
void asmg_get_region(GsSPRITE *base, GsSPRITE *region, u_short x, u_short y, u_short u, u_short v, u_short w, u_short h) {
    region->attribute = base->attribute;
    region->x = (short) x;
    region->y = (short) y;
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
    region->scalex = GPUB_ROT_ONE * 1;
    region->scaley = GPUB_ROT_ONE * 1;
}

u_char get_tpage_color_bit_mode(u_short num_color_bits) {
    if (num_color_bits & ASMG_COLOR_BITS_4) {
        return 0;
    } else if (num_color_bits & ASMG_COLOR_BITS_8) {
        return 0x1;
    } else if (num_color_bits & ASMG_COLOR_BITS_16) {
        return 0x2;
    }
    logr_log(ERROR, "AssetManager.c", "get_tpage_color_bit_mode",
             "Unknown num_color_bits value=%d, needs to be one of [%d, %d, %d]. Terminating...",
             num_color_bits, ASMG_COLOR_BITS_4, ASMG_COLOR_BITS_8, ASMG_COLOR_BITS_16);
    exit(1);
}

u_char get_width_by_color_bits_mode(u_short num_color_bits) {
    if (num_color_bits & ASMG_COLOR_BITS_8) {
        return 2;
    } else if (num_color_bits & ASMG_COLOR_BITS_16) {
        return 1;
    }
    return num_color_bits;
}

u_long get_attribute_by_color_bits_mode(u_short num_color_bits) {
    switch (num_color_bits) {
        case ASMG_COLOR_BITS_4:
            return 0x0;
        case ASMG_COLOR_BITS_8:
            return 0x100 << 16;
        case ASMG_COLOR_BITS_16:
            return 0x200 << 16;
        default:
            logr_log(ERROR, "AssetManager.c", "get_attribute_by_color_bits_mode",
                     "Unknown num_color_bits value=%d, needs to be one of [%d, %d, %d]. Terminating...",
                     num_color_bits, ASMG_COLOR_BITS_4, ASMG_COLOR_BITS_8, ASMG_COLOR_BITS_16);
            exit(1);
    }
}


