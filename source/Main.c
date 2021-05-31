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
    for(y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            GsSPRITE curr = tiles[x * y];
            printf("[x=%d, y=%d, u=%d, v=%d], ", curr.x, curr.y, curr.u, curr.v);
        }
        printf("\n");
    }
    return 0;
}
