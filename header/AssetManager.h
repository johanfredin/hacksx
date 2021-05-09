#ifndef PSX_ASSET_MANAGER_H
#define PSX_ASSET_MANAGER_H

#include "GPUBase.h"
#include "MemUtils.h"
#include "Logger.h"
#include "MockPSXTypes.h"
#include "CdReader.h"

#define ROT_ONE 4096
#define COLOR_BITS_4 4
#define COLOR_BITS_8 8
#define COLOR_BITS_16 16

typedef struct {
    RECT *frame_buffer;
    RECT *clut;
    u_short t_width_multiplier;
    u_short color_mode;
    u_char is_clut_mode;
    u_long sprite_attr;
} VramAsset;

void asmg_load_asset(VramAsset *vram_asset, CdrData *cdr_data, u_short num_color_bits);
GsSPRITE *asmg_load_sprite(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits);
GsSPRITE *asmg_load_sprite_w_offset(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits, u_short offset_x, u_short offset_y);

#endif // PSX_ASSET_MANAGER_H
