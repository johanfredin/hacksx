#include "../header/AssetManager.h"

void asmg_load_asset(VramAsset *asset, CdrData *cdr_data, u_short num_color_bits) {
    logr_log(INFO, "AssetManager.c", "asmg_load_asset", "Loading asset=%s", cdr_data->name);
}

GsSPRITE *asmg_load_sprite(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits) {
    GsSPRITE *dummy = MEM_MALLOC_3(GsSPRITE);
    dummy->x = 1;
    dummy->y = 1;
    dummy->attribute = 1;
    dummy->rotate = 1;
    dummy->w = dummy->h = dummy->u = dummy->v = 1;
    logr_log(INFO, "AssetManager.c", "asmg_load_sprite", "Loading sprite=%s", cdr_data->name);
    return dummy;
}

GsSPRITE *asmg_load_sprite_w_offset(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits, u_short offset_x, u_short offset_y) {
    return asmg_load_sprite(cdr_data, x + offset_x, y + offset_y, blend, num_color_bits);
}

