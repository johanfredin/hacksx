#include "../header/AssetManager.h"

void asmg_load_asset(VramAsset *asset, CdrData *cdr_data, u_short num_color_bits) {
    logr_log(INFO, "Loading asset=%s", cdr_data->name);
}

GsSPRITE *asmg_load_sprite(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits) {
    GsSPRITE *dummy = MEM_MALLOC_3(GsSPRITE);
    dummy->x = 1;
    dummy->y = 1;
    dummy->attribute = 1;
    dummy->rotate = 1;
    dummy->w = dummy->h = dummy->u = dummy->v = 1;
    logr_log(INFO, "Loading sprite=%s", cdr_data->name);
    return dummy;
}

