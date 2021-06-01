#include "../header/Map.h"
#include "../header/TileFetcher.h"

int main() {
    u_short i, y, x;
    CdrData data = *cdr_read_file("sandbox.json");
    Tile_Map map = *tiled_populate_from_json(jsonp_parse((char *) data.file));
    tiled_print_map(&map);

    GsSPRITE sprite;
    sprite.x = sprite.y = sprite.u = sprite.v = 0;
    GPUB_SET_BRIGHTNESS((&sprite), 128);
    sprite.w = sprite.h = 256;

    GsSPRITE *tiles = tiles_get_gs_tile_sprites_mapped(&sprite, &map);
    for (i = 0; i < 256-8; i++) {
        GsSPRITE curr = tiles[i];
        printf("[%d, %d], ", curr.x, curr.y);
    }
    printf("\n");
    return 0;
}
