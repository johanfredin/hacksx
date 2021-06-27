#include "../header/AssetManager.h"
#include "../header/CdReader.h"
#include "../header/GPUBase.h"
#include "../header/GameObject.h"
#include "../header/Logger.h"
#include "../header/Map.h"
#include "../header/MemUtils.h"
#include "../header/TileFetcher.h"
#include "../header/Tiled.h"

#include <LIBETC.H>

#define MAP_START_FRAME 0

Frame *frames;
CdrData **cdr_data_assets;

u_char assets_cnt = 0;
u_char frame_cnt;
u_char current_frame = MAP_START_FRAME;

void init_frame(Frame *frame, char *tileset_file, char *gobj, char *json_map_file);
RECT get_rect(short x, short y, short w, short h);
TILE get_tile(short x, short y, short w, short h, u_short r, u_short g, u_short b);
void handle_block_collision(GameObject *gobj, Frame *frame);
void handle_teleport_collision(GameObject *gobj, Frame *frame);
void set_level_assets(u_char level);

void map_init(u_char level) {
    u_char i;
    set_level_assets(level);

    frames = MEM_CALLOC_3(8, Frame);
    init_frame(&frames[frame_cnt++], "TS_8B.TIM", NULL, "0_0.JSON");
    init_frame(&frames[frame_cnt++], "TS_8B.TIM", NULL, "0_1.JSON");
    init_frame(&frames[frame_cnt++], "TS_8B.TIM", NULL, "1_0.JSON");
    init_frame(&frames[frame_cnt++], "TS_8B.TIM", NULL, "1_1.JSON");
    init_frame(&frames[frame_cnt++], "TS_8HOUSE.TIM", NULL, "04.JSON");
    init_frame(&frames[frame_cnt++], "TS_8B.TIM", NULL, "05.JSON");
    init_frame(&frames[frame_cnt++], "TS_8B.TIM", NULL, "06.JSON");

    // Cleanup
    for (i = 0; i < assets_cnt; i++) {
        cdr_data_free(cdr_data_assets[i]);
    }
    MEM_FREE_3_AND_NULL(cdr_data_assets);
}

void set_level_assets(u_char level) {
    logr_log(INFO, "Map.c", "set_level_assets", "*********************************");
    logr_log(INFO, "Map.c", "set_level_assets", "* ADDING ASSETS FOR LEVEL NR %d *", level);
    logr_log(INFO, "Map.c", "set_level_assets", "*********************************");
    cdr_open();
    if (level == 1) {
        cdr_data_assets = MEM_CALLOC_3_PTRS(10, CdrData);
        // Load tile sets
        cdr_data_assets[assets_cnt++] = cdr_read_file("TS_8HOUSE.TIM");
        cdr_data_assets[assets_cnt++] = cdr_read_file("TS_8B.TIM");

        // Load tile maps
        cdr_data_assets[assets_cnt++] = cdr_read_file("0_0.JSON");
        cdr_data_assets[assets_cnt++] = cdr_read_file("0_1.JSON");
        cdr_data_assets[assets_cnt++] = cdr_read_file("1_0.JSON");
        cdr_data_assets[assets_cnt++] = cdr_read_file("1_1.JSON");
        cdr_data_assets[assets_cnt++] = cdr_read_file("04.JSON");
        cdr_data_assets[assets_cnt++] = cdr_read_file("05.JSON");
        cdr_data_assets[assets_cnt++] = cdr_read_file("06.JSON");
    }
    cdr_close();
    logr_log(DEBUG, "Map.c", "set_level_assets", "%d assets loaded", assets_cnt);
}

void init_frame(Frame *frame, char *tileset_file, char *gobj, char *json_map_file) {
    // Declarations --------------------------
    CdrData *json_cdr_data, *tileset_cdr_data;
    u_long *content;
    GsSPRITE *tile_set;
    JSON_Data *map_data;
    Tile_Map *tile_map;
    ObjectLayer_Bounds *curr_b;
    ObjectLayer_Teleport *curr_t;
    CollisionBlock *collision_blocks;
    Teleport *teleports;
    u_char blocks_cnt, teleports_cnt, i;

    // Map coords
    u_short map_w, map_h;
    u_char offset_x = 0, offset_y = 0;

    // Parse json file into tile map
    json_cdr_data = cdr_find_data_entry(json_map_file, cdr_data_assets, assets_cnt);
    logr_log(INFO, "Map.c", "init_frame", "JSON file=%s retrieved", json_map_file);
    content = json_cdr_data->file;
    map_data = jsonp_parse((char *)content);
    tile_map = tiled_populate_from_json(map_data);

    // INIT SPRITES -----------------------------------------------------------
    tileset_cdr_data = cdr_find_data_entry(tileset_file, cdr_data_assets, assets_cnt);
    logr_log(DEBUG, "Map.c", "init_frame", "tileset file=%s retrieved", tileset_file);
    tile_set = asmg_load_sprite(tileset_cdr_data, 0, 0, 128, ASMG_COLOR_BITS_8);    // Load tileset
    tf_add_layers_to_frame(frame, tile_set, tile_map);                                          // Map tiles to frame

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


    // Init collision blocks
    blocks_cnt = tile_map->bounds_cnt;
    MAP_MALLOC_COLLISION_BLOCK(collision_blocks, blocks_cnt);
    for (i = 0, curr_b = tile_map->bounds; curr_b != NULL; i++, curr_b = curr_b->next) {
        u_short x = curr_b->x + tile_map->offset_x;
        u_short y = curr_b->y + tile_map->offset_y;

        collision_blocks->bounds[i] = get_rect(x, y, curr_b->width, curr_b->height);
        if (GPUB_DRAW_BOUNDS) {
            collision_blocks->cb_bound_lines[i] = get_tile(x, y, curr_b->width, curr_b->height, 255, 0, 0);
        }
    }
    collision_blocks->amount = blocks_cnt;
    frame->collision_blocks = collision_blocks;

    // Init teleports
    teleports_cnt = tile_map->teleports_cnt;
    teleports = MEM_CALLOC_3(teleports_cnt, Teleport);
    for (i = 0, curr_t = tile_map->teleports; curr_t != NULL; i++, curr_t = curr_t->next) {
        u_short x = curr_t->x + tile_map->offset_x;
        u_short y = curr_t->y + tile_map->offset_y;

        teleports[i].origin = get_rect(x, y, curr_t->width, curr_t->height);

        teleports[i].dest_x = curr_t->dest_x;
        teleports[i].dest_y = curr_t->dest_y;
        teleports[i].dest_frame = curr_t->dest_frame;

        if (GPUB_DRAW_BOUNDS) {
            teleports[i].t_bound_lines = get_tile(x, y, curr_t->width, curr_t->height, 0, 0, 255);
        }
        logr_log(DEBUG, "Map.c", "init_frame", "Dest x=%d, y=%d, frame=%d, name=%s", teleports[i].dest_x, teleports[i].dest_y, teleports[i].dest_frame, json_map_file);
    }
    frame->t_amount = teleports_cnt;
    frame->teleports = teleports;

    // Housekeeping
    jsonp_free(map_data);
    tiled_free(tile_map);
}

RECT get_rect(short x, short y, short w, short h) {
    RECT r = {x, y, w, h};
    logr_log(TRACE, "Map.c", "get_rect", "Coords assigned at {x:%d, y%d, w:%d, h:%d}", r.x, r.y, r.w, r.h);
    return r;
}

TILE get_tile(short x, short y, short w, short h, u_short r, u_short g, u_short b) {
    TILE bounds;
    SetTile(&bounds);
    bounds.x0 = x;
    bounds.y0 = y;
    bounds.w = w;
    bounds.h = h;
    setRGB0(&bounds, r, g, b);
    logr_log(TRACE, "Map.c", "get_tile", "TILE bounds={x:%d, y:%d, w:%d, h:%d}", bounds.x0, bounds.y0, bounds.w, bounds.h);
    return bounds;
}

void map_draw(Player *player) {
    SpriteLayer *curr_sl;
    Frame *frame = &frames[current_frame];

    if (frame->bg_layers != NULL) {
        for (curr_sl = frame->bg_layers; curr_sl != NULL; curr_sl = curr_sl->next) {
            u_short i;
            for (i = 0; i < curr_sl->sprites_cnt; i++) {
                GsSortFastSprite(curr_sl->sprites[i], gpub_curr_ot(), curr_sl);
            }
        }
    }
    if (frame->game_object != NULL) {
        gobj_draw(frame->game_object);
    }
//    gobj_player_draw(player);

    if (frame->fg_layers != NULL) {
        for (curr_sl = frame->fg_layers; curr_sl != NULL; curr_sl = curr_sl->next) {
            u_short i;
            for (i = 0; i < curr_sl->sprites_cnt; i++) {
                GsSortFastSprite(curr_sl->sprites[i], gpub_curr_ot(), curr_sl);
            }
        }
    }

    if (GPUB_PRINT_COORDS) {
        CollisionBlock *blocks = frame->collision_blocks;
        FntPrint("Current framee=%d\n", current_frame);
        FntPrint("Blocks in frame=%d\n", blocks->amount);
        FntPrint("Teleports in frame=%d\n", frame->t_amount);
    }

    if (GPUB_DRAW_BOUNDS) {
        CollisionBlock *blocks = frame->collision_blocks;
        Teleport *teleports = frame->teleports;
        int blockIdx = 0, t_idx = 0;
        while (blockIdx < blocks->amount) {
            DrawPrim(&blocks->cb_bound_lines[blockIdx]);
            blockIdx++;
        }
        while (t_idx < frame->t_amount) {
            DrawPrim(&teleports[t_idx].t_bound_lines);
            t_idx++;
        }
    }
}

void map_tick(Player *player) {
    Frame *frame = &frames[current_frame];
    // TEMP
    u_long btn = PadRead(1);
    if (btn & PADselect) {
        if (current_frame >= 3) {
            current_frame = 0;
        } else {
            current_frame++;
        }
    }

    gobj_player_tick(player);
    handle_teleport_collision(player->gobj, frame);
    handle_block_collision(player->gobj, frame);
    if (frame->game_object != NULL) {
        gobj_tick(frame->game_object, player);
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

        if (right_col || leftCol || top_col || bottom_col) {
            switch (gobj->type) {
                case GOBJ_TYPE_PLAYER:
                    if (t.dest_x > 0) {
                        gobj->sprite->x = t.dest_x + frame->offset_x;
                    }
                    if (t.dest_y > 0) {

                        gobj->sprite->y = t.dest_y + frame->offset_y;
                    }
                    if (frame->game_object != NULL && frame->game_object->type == GOBJ_TYPE_NPC) {
                        GOBJ_RESET_POS(frame->game_object);
                    }
                    current_frame = t.dest_frame;
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
