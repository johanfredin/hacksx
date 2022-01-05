#include "../header/Map.h"
#include "../header/AssetManager.h"
#include "../header/GPUBase.h"
#include "../header/MemUtils.h"
#include "../header/TileFetcher.h"
#include "../header/StrUtils.h"
#include "../header/FntLogger.h"

#include <LIBETC.H>

#define MAP_START_FRAME 0

Frame *g_map_frames;
FR_TileSet **g_map_tile_sets;
CdrData **g_map_cdr_data_assets;

u_char g_assets_cnt = 0, g_map_tilesets_count = 0;
u_char g_frame_cnt;
u_char g_current_frame = MAP_START_FRAME;

void init_frame(Frame *frame, char *json_map_file);
RECT get_rect(short x, short y, short w, short h);
void handle_block_collision(GameObject *gobj, Frame *frame);
void handle_teleport_collision(GameObject *gobj, Frame *frame);
void load_level_assets_from_cd(u_char level);

/**
 * Transfer data from the Tile_Set struct parsed from the map editor
 * to our frame specific FR_Tile_Set struct. We want separation of
 * concern between the two so that the Tiled version can be
 * freed afterwards
 * @param map map to transfer data from
 */
FR_TileSet *transfer_to_frame_tileset(Tile_Map *map);

void load_tilesets();

void map_init(u_char level) {
    u_char i;
    load_level_assets_from_cd(level);
    load_tilesets();

    g_map_frames = MEM_CALLOC_3(8, Frame);
    init_frame(&g_map_frames[g_frame_cnt++], "TS8_TL.JSON");
    init_frame(&g_map_frames[g_frame_cnt++], "TS8_BL.JSON");
    init_frame(&g_map_frames[g_frame_cnt++], "TS8_TR.JSON");
    init_frame(&g_map_frames[g_frame_cnt++], "TS8_BR.JSON");
    init_frame(&g_map_frames[g_frame_cnt++], "TS8_IN1.JSON");
    init_frame(&g_map_frames[g_frame_cnt++], "TS8_IN2.JSON");
    init_frame(&g_map_frames[g_frame_cnt++], "TS8_IN3.JSON");

    // Cleanup
//    for (i = 0; i < g_assets_cnt; i++) {
//        CDR_DATA_FREE(g_map_cdr_data_assets[i])
//    }
    MEM_FREE_3_AND_NULL(g_map_cdr_data_assets);
}

void load_level_assets_from_cd(u_char level) {
    logr_log(INFO, "Map.c", "load_level_assets_from_cd", "*********************************");
    logr_log(INFO, "Map.c", "load_level_assets_from_cd", "* ADDING ASSETS FOR LEVEL NR %d *", level);
    logr_log(INFO, "Map.c", "load_level_assets_from_cd", "*********************************");
    if (level == 1) {
        g_map_cdr_data_assets = MEM_CALLOC_3_PTRS(10, CdrData);
        // Load tile sets
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_TL.TIM");
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_TR.TIM");
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_BL.TIM");
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_BR.TIM");
        g_map_cdr_data_assets[g_map_tilesets_count++] = cdr_read_file("TS8_IN.TIM");

        // Load tile maps
        g_assets_cnt += g_map_tilesets_count;
        g_map_cdr_data_assets[g_assets_cnt++] = cdr_read_file("TS8_TL.JSON");
        g_map_cdr_data_assets[g_assets_cnt++] = cdr_read_file("TS8_BL.JSON");
        g_map_cdr_data_assets[g_assets_cnt++] = cdr_read_file("TS8_TR.JSON");
        g_map_cdr_data_assets[g_assets_cnt++] = cdr_read_file("TS8_BR.JSON");
        g_map_cdr_data_assets[g_assets_cnt++] = cdr_read_file("TS8_IN1.JSON");
        g_map_cdr_data_assets[g_assets_cnt++] = cdr_read_file("TS8_IN2.JSON");
        g_map_cdr_data_assets[g_assets_cnt++] = cdr_read_file("TS8_IN3.JSON");
    }
    logr_log(DEBUG, "Map.c", "load_level_assets_from_cd", "%d assets loaded", g_assets_cnt);
}

/**
 * Populates our global array of FR_TileSet structs from the tileset CdrData assets.
 * Retrieve the GS_SPRITE and the asset name.
 */
void load_tilesets() {
    u_char i;
    g_map_tile_sets = MEM_CALLOC_3_PTRS(g_map_tilesets_count, FR_TileSet);
    for (i = 0; i < g_map_tilesets_count; i++) {
        g_map_tile_sets[i] = frames_malloc_fr_tileset();
        g_map_tile_sets[i]->sprite = asmg_load_sprite(g_map_cdr_data_assets[i], 0, 0, 128, ASMG_COLOR_BITS_8);
        g_map_tile_sets[i]->source = g_map_cdr_data_assets[i]->name;
        logr_log(TRACE, "Map.c", "load_tilesets", "tileset=%s loaded", g_map_cdr_data_assets[i]->name);
    }
    logr_log(DEBUG, "Map.c", "load_tilesets", "%d tilesets loaded", g_map_tilesets_count);
}

void init_frame(Frame *frame, char *json_map_file) {
    // Declarations --------------------------
    CdrData *json_cdr_data;
    u_long *content;
    JSON_Data *json_map_data;
    Tile_Map *tile_map;
    ObjectLayer_Bounds *curr_b;
    ObjectLayer_Teleport *curr_t;
    ObjectLayer_Dialog *curr_d;
    CollisionBlock *collision_blocks;
    Teleport *teleports;
    Dialog *dialogs;
    u_char blocks_cnt, teleports_cnt, dialogs_cnt, i;

    // Map coords
    u_short map_w, map_h;
    u_char offset_x = 0, offset_y = 0;

    // Parse json file into tile map
    json_cdr_data = cdr_find_data_entry(json_map_file, g_map_cdr_data_assets, g_assets_cnt);
    logr_log(INFO, "Map.c", "init_frame", "JSON file=%s retrieved", json_map_file);
    content = json_cdr_data->file;
    json_map_data = jsonp_parse((char *)content);
    tile_map = tiled_populate_from_json(json_map_data);
    tiled_print_map(DEBUG, tile_map);
    frame->fr_tilesets = transfer_to_frame_tileset(tile_map);

    // Calc potential x and/or y offsets (e.g frame is smaller than screen w and/or h)
    map_w = tile_map->width * tile_map->tile_width;
    map_h = tile_map->height * tile_map->tile_height;

    if (map_w < gpub_screen_w()) {
        offset_x = (gpub_screen_w() - map_w) / 2;
    }
    if (map_h < gpub_screen_h()) {
        offset_y = (gpub_screen_h() - map_h) / 2;
    }

    // Give potential offsets to frame.
    frame->offset_x = offset_x;
    frame->offset_y = offset_y;
    logr_log(DEBUG, "Map.c", "init_frame", "frame offset_x=%d, offset_y=%d", offset_x, offset_y);

    // Load tilesets (frame may consist of multiple tilesets)
    tf_add_layers_to_frame(frame, frame->fr_tilesets, g_map_tilesets_count, tile_map);

    // Init collision blocks
    blocks_cnt = tile_map->bounds_cnt;
    MAP_MALLOC_COLLISION_BLOCK(collision_blocks, blocks_cnt);
    for (i = 0, curr_b = tile_map->bounds; curr_b != NULL; i++, curr_b = curr_b->next) {
        u_short x = curr_b->x + frame->offset_x;
        u_short y = curr_b->y + frame->offset_y;

        collision_blocks->bounds[i] = get_rect((short) x, (short) y, (short) curr_b->width, (short) curr_b->height);
    }
    collision_blocks->amount = blocks_cnt;
    frame->collision_blocks = collision_blocks;

    // Init dialogs
    dialogs = NULL; // Init to NULL since there can be frames without dialogs
    dialogs_cnt = tile_map->dialogs_cnt;
    dialogs = MEM_CALLOC_3(dialogs_cnt, Dialog);
    for (i = 0, curr_d = tile_map->dialogs; curr_d != NULL; i++, curr_d = curr_d->next) {
        u_short x = curr_d->x + frame->offset_x;
        u_short y = curr_d->y + frame->offset_y;

        dialogs[i].bounds = get_rect((short) x, (short) y, (short) curr_d->width, (short) curr_d->height);
        dialogs[i].n_lines = curr_d->n_lines;
        dialogs[i].max_chars = curr_d->max_chars;
        dialogs[i].text = curr_d->text;
        logr_log(DEBUG, "Map.c", "init_frame", "Dialog n_lines=%d, max_chars=%d, text=%s", dialogs[i].n_lines, dialogs[i].max_chars, dialogs[i].text);
    }
    frame->d_amount = dialogs_cnt;
    frame->dialogs = dialogs;

    // Init teleports
    teleports_cnt = tile_map->teleports_cnt;
    teleports = MEM_CALLOC_3(teleports_cnt, Teleport);
    for (i = 0, curr_t = tile_map->teleports; curr_t != NULL; i++, curr_t = curr_t->next) {
        u_short x = curr_t->x + frame->offset_x;
        u_short y = curr_t->y + frame->offset_y;

        teleports[i].origin = get_rect((short) x, (short) y, (short) curr_t->width, (short) curr_t->height);

        teleports[i].dest_x = curr_t->dest_x;
        teleports[i].dest_y = curr_t->dest_y;
        teleports[i].dest_frame = curr_t->dest_frame;

        logr_log(DEBUG, "Map.c", "init_frame", "Dest x=%d, y=%d, frame=%d, name=%s", teleports[i].dest_x, teleports[i].dest_y, teleports[i].dest_frame, json_map_file);
    }
    frame->t_amount = teleports_cnt;
    frame->teleports = teleports;

    // Housekeeping
    jsonp_free(json_map_data);
    tiled_free(tile_map);
}

FR_TileSet *transfer_to_frame_tileset(Tile_Map *map) {
    Tile_Set *curr_ts;
    FR_TileSet *fr_tile_sets = MEM_CALLOC_3(map->tilesets_cnt, FR_TileSet);
    u_char match_cnt = 0;

    // Iterate tilesets that the tile map is using
    for (curr_ts = map->tile_sets; curr_ts != NULL; curr_ts = curr_ts->next) {
        u_char i;

        // Iterate the map FR_Tileset array to look for a matching image
        for(i = 0; i < g_map_tilesets_count; i++) {
            u_char count;
            char *source = curr_ts->source;
            char substr[16];
            FR_TileSet *map_fr_tile_set = g_map_tile_sets[i];
            /*
             * Compare source in current map tileset with the one for the image
             * We do this by making a lower case comparison and checking if the
             * full source path in our tiled json file contains the current image name (minus the .tim suffix)
             */
            char *tim_name = strcpy(MEM_CALLOC_3(strlen(map_fr_tile_set->source) + 1, char), map_fr_tile_set->source);
            STR_TO_LOWERCASE(tim_name);

            STR_READ_UNTIL(tim_name, substr, '.', count);
            MEM_FREE_3_AND_NULL(tim_name);

            if (STR_CONTAINS(source, substr)) {
                // Match found, assign it to frame tilesets
                map_fr_tile_set->start_id = curr_ts->firstgid;
                fr_tile_sets[match_cnt++] = *map_fr_tile_set;
                break;
            }
        }
    }

    // Verify the expected amount of tilesets were transferred
    if (match_cnt != map->tilesets_cnt) {
        logr_log(ERROR, "Map.c", "transfer_to_frame_tileset",
                 "Incorrect number of matching tilesets between tile map and frame, match_cnt=%d, tileset_cnt=%d, shutting down...",
                 match_cnt, map->tilesets_cnt);
        exit(1);
    }

    return fr_tile_sets;
}

RECT get_rect(short x, short y, short w, short h) {
    RECT r = {x, y, w, h};
    logr_log(TRACE, "Map.c", "get_rect", "Coords assigned at {x:%d, y%d, w:%d, h:%d}", r.x, r.y, r.w, r.h);
    return r;
}

void map_draw(Player *player) {
    SpriteLayer *curr_sl;
    Frame *frame = &g_map_frames[g_current_frame];

    if (frame->bg_layers != NULL) {
        for (curr_sl = frame->bg_layers; curr_sl != NULL; curr_sl = curr_sl->next) {
            u_short i;
            for (i = 0; i < curr_sl->sprites_cnt; i++) {
                GPUB_GS_SORT_FAST_OBJ(curr_sl->sprites[i]);
            }
        }
    }
    if (frame->game_object != NULL) {
        gobj_draw(frame->game_object);
    }

    gobj_player_draw(player);

    if (frame->fg_layers != NULL) {
        for (curr_sl = frame->fg_layers; curr_sl != NULL; curr_sl = curr_sl->next) {
            u_short i;
            for (i = 0; i < curr_sl->sprites_cnt; i++) {
                GPUB_GS_SORT_FAST_OBJ(curr_sl->sprites[i]);
            }
        }
    }

    FNT_PRINT_BLOCKS(g_current_frame, frame)

}

void map_tick(Player *player) {
    Frame *frame = &g_map_frames[g_current_frame];
    // TEMP
    u_long btn = PadRead(1);
    if (btn & PADselect) {
        if (g_current_frame >= 3) {
            g_current_frame = 0;
        } else {
            g_current_frame++;
        }
    }

    gobj_player_tick(player);
    handle_teleport_collision(player->gobj, frame);
    handle_block_collision(player->gobj, frame);
    if (frame->game_object != NULL) {
        gobj_tick(frame->game_object);
        handle_block_collision(frame->game_object, frame);
        handle_teleport_collision(frame->game_object, frame);
    }
}

void handle_block_collision(GameObject *gobj, Frame *frame) {
    u_char right_col, left_col, top_col, bottom_col;
    CollisionBlock *blocks = frame->collision_blocks;
    int i = 0;

    // Sprite  bounds
    short px = gobj->sprite->x;
    short py = gobj->sprite->y;
    u_short pw = gobj->sprite->w;
    u_short ph = gobj->sprite->h;
    u_short pxw = px + pw;
    u_short pyh = py + ph;

    while (i < blocks->amount) {
        // Collision block bounds
        RECT *bounds = &blocks->bounds[i];
        short bx = bounds->x;
        short by = bounds->y;
        short bw = bounds->w;
        short bh = bounds->h;
        short bxw = (short)(bx + bw);
        short byh = (short)(by + bh);

        right_col = pxw >= bx && px <= bx && pyh > by && py < byh;
        left_col = px <= bxw && pxw > bxw && pyh > by && py < byh;
        top_col = pyh >= by && py < by && pxw > bx && px < bxw;
        bottom_col = py <= byh && pyh > byh && pxw > bx && px < bxw;

        switch (gobj->type) {
            case GOBJ_TYPE_PLAYER:
                if (right_col) {
                    gobj->sprite->x = (short)(bx - pw);
                }
                if (left_col) {
                    gobj->sprite->x = bxw;
                }
                if (top_col) {
                    gobj->sprite->y = (short)(by - ph);
                }
                if (bottom_col) {
                    gobj->sprite->y = byh;
                }
                break;
            case GOBJ_TYPE_NPC:
                if (right_col || left_col) {
                    GOBJ_SWITCH_X_DIR(gobj);
                }
                if (top_col || bottom_col) {
                    GOBJ_SWITCH_Y_DIR(gobj);
                }
                break;
        }
        i++;
    }
}

void handle_teleport_collision(GameObject *gobj, Frame *frame) {
    u_char right_col, leftCol, top_col, bottom_col;
    Teleport *teleports = frame->teleports;
    int i = 0;

    // Player bounds
    short px = gobj->sprite->x;
    short py = gobj->sprite->y;
    u_short pw = gobj->sprite->w;
    u_short ph = gobj->sprite->h;
    u_short pxw = px + pw;
    u_short pyh = py + ph;
    while (i < frame->t_amount) {
        Teleport t = teleports[i];
        RECT bounds = t.origin;
        short bx = bounds.x;
        short by = bounds.y;
        u_short bw = bounds.w;
        u_short bh = bounds.h;
        u_short bxw = bx + bw;
        u_short byh = by + bh;

        right_col = pxw >= bx && px <= bx && pyh > by && py < byh;
        leftCol = px <= bxw && pxw > bxw && pyh > by && py < byh;
        top_col = pyh >= by && py < by && pxw > bx && px < bxw;
        bottom_col = py <= byh && pyh > byh && pxw > bx && px < bxw;

        if (right_col | leftCol | top_col | bottom_col) {
            switch (gobj->type) {
                case GOBJ_TYPE_PLAYER:
                    g_current_frame = t.dest_frame; // Update curr frame to dest frame so that we can get its offsets (if any)
                    if (t.dest_x > 0) {
                        gobj->sprite->x = t.dest_x + g_map_frames[g_current_frame].offset_x;
                    }
                    if (t.dest_y > 0) {
                        gobj->sprite->y = t.dest_y + g_map_frames[g_current_frame].offset_y;
                    }
                    if (frame->game_object != NULL && frame->game_object->type == GOBJ_TYPE_NPC) {
                        GOBJ_RESET_POS(frame->game_object);
                    }
                    break;
                case GOBJ_TYPE_NPC:
                    if (right_col || leftCol) {
                        GOBJ_SWITCH_X_DIR(gobj);
                    }
                    if (top_col || bottom_col) {
                        GOBJ_SWITCH_Y_DIR(gobj);
                    }
                    break;
            }
        }
        i++;
    }
}