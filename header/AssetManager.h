#ifndef PSX_ASSET_MANAGER_H
#define PSX_ASSET_MANAGER_H

#include "GPUBase.h"
#include "MemUtils.h"
#include "Logger.h"
#include "MockPSXTypes.h"
#include "CdReader.h"
#include "Tiled.h"

#define ASMG_ROT_ONE 4096
#define ASMG_COLOR_BITS_4 4
#define ASMG_COLOR_BITS_8 8
#define ASMG_COLOR_BITS_16 16

typedef struct {
    RECT *frame_buffer;
    RECT *clut;
    u_short t_width_multiplier;
    u_short color_mode;
    u_char is_clut_mode;
    u_long sprite_attr;
} VramAsset;

void asmg_load_asset(VramAsset *vram_asset, CdrData *cdr_data, u_short num_color_bits);
void asmg_get_region(GsSPRITE *sprite, GsSPRITE *region, u_short x, u_short y, u_short u, u_short v, u_short w, u_short h);
GsSPRITE *asmg_load_sprite(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits);
GsSPRITE *asmg_load_sprite_w_offset(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits, u_short offset_x, u_short offset_y);
GsBG *asmg_get_gs_bg(Tile_Map *tm, CdrData *sprite_data);

#endif // PSX_ASSET_MANAGER_H
