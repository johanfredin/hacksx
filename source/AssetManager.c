#include "../header/AssetManager.h"

void asmg_load_asset(VramAsset *asset, CdrData *cdr_data, u_short num_color_bits) {
    logr_log(INFO, "Loading asset=%s", cdr_data->name);
}

GsSPRITE *asmg_load_sprite(CdrData *cdr_data, u_short x, u_short y, u_short blend, u_short num_color_bits) {
    logr_log(INFO, "Loading srpite=%s", cdr_data->name);
    return NULL;
}

