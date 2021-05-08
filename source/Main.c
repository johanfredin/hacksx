#include <malloc.h>
#include <stdlib.h>
#include "../header/JSONParser.h"
#include "../header/FileReader.h"
#include "../header/Tiled.h"
#include "string.h"

typedef unsigned long u_long;

typedef struct {
    short x, y;        /* offset point on VRAM */
    short w, h;        /* width and height */
} RECT;

#define MAP_PROP_VAL_IF_PRESENT_OR(prop_to_set, prop_to_check, default_value) \
    if (prop_to_check == NULL) {                                              \
        prop_to_set = default_value;                                          \
    } else {                                                                  \
        prop_to_set = prop_to_check;                                          \
    }

typedef struct CollisionBlock {
    RECT *bounds;         // The actual physical bounds that we will collide with in the frame
    u_char amount;        // The amount of blocks on one frame
} CollisionBlock;

typedef struct Teleport {
    RECT origin;
    short dest_x, dest_y;
    u_char dest_frame;  // frame array index of frame to change to when colliding with this teleport
} Teleport;

typedef struct Frame {
    u_char t_amount;
    long *bg;
    long *fg;
    CollisionBlock *collision_blocks;
    Teleport *teleports;
} Frame;

typedef struct {
    char *name;
    char *file;
} CdrData;

Frame *frames;
CdrData **cdr_data_assets;

u_char assets_count;
u_char frame_cnt;
u_char current_frame = 0;

CdrData *cdr_find_data_entry(char *name, CdrData **assets, u_char assets_cnt);
CdrData *cdr_read_file(char *fileName);
void init_frame(Frame *frame, char *bg, char *fg, char *gobj, char *json_map_file);
RECT get_rect(u_short x, u_short y, u_short w, u_short h);
u_char set_level_assets(u_char level);
void cdr_data_free(CdrData *cdr_data);

void map_init(u_char level) {
    u_char i;
    assets_count = set_level_assets(level);

    frames = calloc(7, sizeof(Frame));
    init_frame(&frames[0], "01BG.TIM", "01FG.TIM", "RAICHU.TIM", "C:\\Users\\lowrider\\CLionProjects\\hacksx\\res\\0_0.JSON");
//    init_frame(&frames[1], "00BG.TIM", "00FG.TIM", "PSYDUCK.TIM", "C:\\Users\\lowrider\\CLionProjects\\hacksx\\res\\1_0.JSON");
    // init_frame(&frames[frame_cnt++], "11BG.TIM", "11FG.TIM", "ALOLA.TIM", "1_1.JSON");
    // init_smaller_frame(&frames[frame_cnt++], "YOLOBG.TIM", "YOLOFG.TIM", "100I0.JSON", 256 / 4, 256 / 4);
    // init_smaller_frame(&frames[frame_cnt++], "1_TUNNEL.TIM", NULL, "1_TUN.JSON", 0, 256 / 3);
    // init_smaller_frame(&frames[frame_cnt++], "1_H2.TIM", NULL, "1_H2.JSON", 256 / 4, 256 / 8);

    // Cleanup
    for (i = 0; i < assets_count; i++) {
        cdr_data_free(cdr_data_assets[i]);
    }
    free(cdr_data_assets);
    printf("removed %d assets\n", assets_count);
}

u_char set_level_assets(u_char level) {
    u_char asset_cnt = 0;
    printf("ADDING ASSETS FOR LEVEL\n");
    if (level == 1) {
        cdr_data_assets = calloc(24, sizeof(CdrData));
        cdr_data_assets[asset_cnt++] = cdr_read_file("C:\\Users\\lowrider\\CLionProjects\\hacksx\\res\\0_0.JSON");
//        cdr_data_assets[asset_cnt++] = cdr_read_file("C:\\Users\\lowrider\\CLionProjects\\hacksx\\res\\1_0.JSON");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("1_0.JSON");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("1_1.JSON");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("1_TUN.JSON");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("1_H2.JSON");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("100I0.JSON");
    }
    return asset_cnt;
}

void init_frame(Frame *frame, char *bg, char *fg, char *gobj, char *json_map_file) {
    // Declarations --------------------------
    char *content;
    CdrData *json_cdr_data;
    JSON_Data *map_data;
    Tile_Map *tile_map;
    ObjectLayer_Bounds *curr_b;
    ObjectLayer_Teleport *curr_t;
    CollisionBlock *collision_blocks;
    Teleport *teleports;
    u_char blocks_cnt;
    u_char teleports_cnt;
    u_char i;

    // INIT SPRITES -----------------------------------------------------------
    frame->bg = NULL;
    frame->fg = NULL;

    // Parse json file into tile map
    json_cdr_data = cdr_find_data_entry(json_map_file, cdr_data_assets, assets_count);
    content = json_cdr_data->file;
    map_data = jsonp_parse(content);
    tile_map = tiled_populate_from_json(map_data);
    tiled_print_map(tile_map);
    // Init collision blocks
    blocks_cnt = tile_map->bounds_cnt;
    collision_blocks = malloc(sizeof(CollisionBlock));
    collision_blocks->bounds = calloc(blocks_cnt, sizeof(RECT));
    for (i = 0, curr_b = tile_map->bounds; curr_b != NULL; i++, curr_b = curr_b->next) {
        collision_blocks->bounds[i] = get_rect(curr_b->x, curr_b->y, curr_b->width, curr_b->height);
    }
    collision_blocks->amount = blocks_cnt;
    frame->collision_blocks = collision_blocks;

    // Init teleports
    teleports_cnt = tile_map->teleports_cnt;
    teleports = calloc(teleports_cnt, sizeof(Teleport));
    for (i = 0, curr_t = tile_map->teleports; curr_t != NULL; i++, curr_t = curr_t->next) {

        teleports[i].origin = get_rect(curr_t->x, curr_t->y, curr_t->width, curr_t->height);

        // Replace with real values once json parser updated
        MAP_PROP_VAL_IF_PRESENT_OR(teleports[i].dest_x, curr_t->dest_x, -1)
        MAP_PROP_VAL_IF_PRESENT_OR(teleports[i].dest_y, curr_t->dest_y, -1)
        teleports[i].dest_frame = curr_t->dest_frame;

        LOGR_LOG_TELEPORT(INFO, teleports[i]);
    }

    frame->t_amount = teleports_cnt;
    frame->teleports = teleports;

    // Housekeeping
     jsonp_free(map_data);
     tiled_free(tile_map);
}

void cdr_data_free(CdrData *cdr_data) {
    free(cdr_data->file);
    free(cdr_data);
}

CdrData *cdr_find_data_entry(char *name, CdrData **assets, u_char assets_cnt) {
    int i;
    for (i = 0; i < assets_cnt; i++) {
        if (STREQ(assets[i]->name, name)) {
            return assets[i];
        }
    }
    printf("ERROR, No CdrData with name='%s' in passed in array, terminating...\n", name);
    exit(1);
}

CdrData *cdr_read_file(char *fileName) {
    CdrData *data = malloc(sizeof(CdrData));
    char *buffer = NULL;
    size_t length;
    FILE *file = fr_get_file(fileName);
    printf("File=%s retrieved\n", fileName);
    fseek(file, 0, SEEK_END);
    length = ftell(file) + 1;   // Reserved for null terminator
    fseek(file, 0, SEEK_SET);
    buffer = malloc(length);
    int i;
    for (i = 0; i < length; i++) {
        buffer[i] = 0;
    }
    if (buffer) {
        fread(buffer, 1, length, file);
        buffer[length - 1] = '\0';
    }

    data->name = fileName;
    data->file = buffer;
    return data;
}

RECT get_rect(u_short x, u_short y, u_short w, u_short h) {
    RECT r = {x, y, w, h};
    logr_log(TRACE, "TRACE - Coords assigned at {x:%d, y%d, w:%d, h:%d}", r.x, r.y, r.w, r.h);
    return r;
}


int main() {
    map_init(1);
    return 0;
}
