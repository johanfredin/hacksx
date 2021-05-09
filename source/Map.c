#include "../header/Map.h"

#define START_FRAME 0

Frame *frames;
CdrData **cdr_data_assets;

u_char assets_count;
u_char frame_cnt;
u_char current_frame = START_FRAME;

void init_frame(Frame *frame, char *bg, char *fg, char *gobj, char *json_map_file);
void init_smaller_frame(Frame *frame, char *bg, char *fg, char *json_map_file, u_char x, u_char y);
RECT get_rect(u_short x, u_short y, u_short w, u_short h);
RECT get_rect_with_offset(u_short x, u_short y, u_short w, u_short h, short x_offset, short y_offset);
TILE get_tile(u_short x, u_short y, u_short w, u_short h, u_short r, u_short g, u_short b);
void handle_block_collision(GameObject *gobj, Frame *frame);
void handle_teleport_collision(GameObject *gobj, Frame *frame);
u_char set_level_assets(u_char level);

void map_init(u_char level) {
    u_char i;
    assets_count = set_level_assets(level);

    frames = MEM_CALLOC_3(10, Frame);
//

    init_smaller_frame(&frames[frame_cnt++], "1_H2.TIM", NULL, "06.JSON", 256 / 4, 256 / 8);

    // Cleanup
    for (i = 0; i < assets_count; i++) {
        cdr_data_free(cdr_data_assets[i]);
    }
    MEM_FREE_3_AND_NULL(cdr_data_assets);
}

u_char set_level_assets(u_char level) {
    u_char asset_cnt = 0;
    logr_log(INFO, "*********************************");
    logr_log(INFO, "* ADDING ASSETS FOR LEVEL NR %d *", level);
    logr_log(INFO, "*********************************");
    cdr_open();
    switch (level) {
        case 1:
            cdr_data_assets = MEM_CALLOC_3_PTRS(24, CdrData);
            cdr_data_assets[asset_cnt++] = cdr_read_file("0_0.JSON");
            cdr_data_assets[asset_cnt++] = cdr_read_file("0_1.JSON");
            cdr_data_assets[asset_cnt++] = cdr_read_file("1_0.JSON");
            cdr_data_assets[asset_cnt++] = cdr_read_file("1_1.JSON");
            cdr_data_assets[asset_cnt++] = cdr_read_file("04.JSON");
            cdr_data_assets[asset_cnt++] = cdr_read_file("05.JSON");
            cdr_data_assets[asset_cnt++] = cdr_read_file("06.JSON");
            cdr_data_assets[asset_cnt++] = cdr_read_file("00BG.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("00FG.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("01BG.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("01FG.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("10BG.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("11BG.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("10FG.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("11FG.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("1_H2.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("1_TUNNEL.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("PSYDUCK.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("RAICHU.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("YOLOBG.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("YOLOFG.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("RAICHU_2.TIM");
            cdr_data_assets[asset_cnt++] = cdr_read_file("ALOLA.TIM");
            break;
    }
    cdr_close();
    logr_log(DEBUG, "%d assets read", asset_cnt);
    return asset_cnt;
}

void init_frame(Frame *frame, char *bg, char *fg, char *gobj, char *json_map_file) {
    // Declarations --------------------------
    CdrData *bg_cdr_data;
    CdrData *fg_cdr_data;
    u_long *content;
    CdrData *gobj_cdr_data;
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
    bg_cdr_data = cdr_find_data_entry(bg, cdr_data_assets, assets_count);
    frame->bg = asmg_load_sprite(bg_cdr_data, 0, 0, 128, COLOR_BITS_8); // BG can not be NULL so no check
    if (fg != NULL) {
        fg_cdr_data = cdr_find_data_entry(fg, cdr_data_assets, assets_count);
        frame->fg = asmg_load_sprite(fg_cdr_data, 0, 0, 128, COLOR_BITS_8);
    }
    if (gobj != NULL) {
        gobj_cdr_data = cdr_find_data_entry(gobj, cdr_data_assets, assets_count);
        frame->game_object = gobj_init(asmg_load_sprite(gobj_cdr_data, 90, 120, 128, COLOR_BITS_8), 16, 16, 2, 3, 100,
                                       GOBJ_TYPE_NPC);
    }

    // Parse json file into tile map
    json_cdr_data = cdr_find_data_entry(json_map_file, cdr_data_assets, assets_count);
    content = json_cdr_data->file;
    map_data = jsonp_parse((char *) content);
//    jsonp_print_data(map_data);
    tile_map = tiled_populate_from_json(map_data);
     tiled_print_map(tile_map);

    // Init collision blocks
    blocks_cnt = tile_map->bounds_cnt;
    MAP_MALLOC_COLLISION_BLOCK(collision_blocks, blocks_cnt);
    for (i = 0, curr_b = tile_map->bounds; curr_b != NULL; i++, curr_b = curr_b->next) {
        collision_blocks->bounds[i] = get_rect(curr_b->x, curr_b->y, curr_b->width, curr_b->height);
        if (GPUB_DRAW_BOUNDS) {
            collision_blocks->cb_bound_lines[i] = get_tile(curr_b->x, curr_b->y, curr_b->width, curr_b->height, 255, 0, 0);
        }
    }
    collision_blocks->amount = blocks_cnt;
    frame->collision_blocks = collision_blocks;

    // Init teleports
    teleports_cnt = tile_map->teleports_cnt;
    teleports = MEM_CALLOC_3(teleports_cnt, Teleport);
    for (i = 0, curr_t = tile_map->teleports; curr_t != NULL; i++, curr_t = curr_t->next) {

        teleports[i].origin = get_rect(curr_t->x, curr_t->y, curr_t->width, curr_t->height);

        teleports[i].dest_x = (curr_t->dest_x == 0) ? -1 : curr_t->dest_x;
        teleports[i].dest_y = (curr_t->dest_y == 0) ? -1 : curr_t->dest_y;
        teleports[i].dest_frame = curr_t->dest_frame;

        if (GPUB_DRAW_BOUNDS) {
            teleports[i].t_bound_lines = get_tile(curr_t->x, curr_t->y, curr_t->width, curr_t->height, 0, 0, 255);
        }
        logr_log(INFO, "INFO\tMap\tinit_frame\tDest x=%d\ty=%d\tframe=%d\tname=%s", teleports[i].dest_x, teleports[i].dest_y, teleports[i].dest_frame, json_map_file);
    }
    frame->t_amount = teleports_cnt;
    frame->teleports = teleports;

    // Housekeeping
    jsonp_free(map_data);
    tiled_free(tile_map);
}

RECT get_rect(u_short x, u_short y, u_short w, u_short h) {
    RECT r = {x, y, w, h};
    logr_log(TRACE, "Coords assigned at {x:%d, y%d, w:%d, h:%d}", r.x, r.y, r.w, r.h);
    return r;
}

RECT get_rect_with_offset(u_short x, u_short y, u_short w, u_short h, short x_offset, short y_offset) {
    RECT r = {x + x_offset, y + y_offset, w, h};
    logr_log(TRACE, "Coords assigned at {x:%d, y%d, w:%d, h:%d}", r.x, r.y, r.w, r.h);
    return r;
}

TILE get_tile(u_short x, u_short y, u_short w, u_short h, u_short r, u_short g, u_short b) {
    TILE bounds;
    SetTile(&bounds);
    bounds.x0 = x;
    bounds.y0 = y;
    bounds.w = w;
    bounds.h = h;
    setRGB0(&bounds, r, g, b);
    logr_log(TRACE, "TILE bounds={x:%d, y:%d, w:%d, h:%d}", bounds.x0, bounds.y0, bounds.w, bounds.h);
    return bounds;
}

#define MAP_VAL_OR_NULL(sprite, val_to_set_if_not_null) if(val != NULL) val = val_to_set

void init_smaller_frame(Frame *frame, char *bg, char *fg, char *json_map_file, u_char x, u_char y) {
    init_frame(frame, bg, fg, NULL, json_map_file);
    frame->bg->x = x;
    frame->bg->y = y;
    if(frame->fg != NULL) {
        frame->fg->y = y;
        frame->fg->x = x;
    }
}

void map_draw() {
    Frame *frame = &frames[current_frame];
    CollisionBlock *blocks = frame->collision_blocks;
    Teleport *teleports = frame->teleports;

    if (frame->fg != NULL) {
        GsSortFastSprite(frame->fg, gpub_curr_ot(), 0);
    }
    if (frame->bg != NULL) {
        GsSortFastSprite(frame->bg, gpub_curr_ot(), 2);
    }
    if (frame->game_object != NULL) {
        gobj_draw(frame->game_object);

    }

    if (GPUB_PRINT_COORDS) {
        FntPrint("Current framee=%d\n", current_frame);
        FntPrint("Blocks in frame=%d\n", blocks->amount);
        FntPrint("Teleports in frame=%d\n", frame->t_amount);
    }

    if (GPUB_DRAW_BOUNDS) {
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
        u_short bw = bounds->w;
        u_short bh = bounds->h;
        u_short bxw = bx + bw;
        u_short byh = by + bh;

        right_col = pxw >= bx && px <= bx && pyh > by && py < byh;
        left_col = px <= bxw && pxw > bxw && pyh > by && py < byh;
        top_col = pyh >= by && py < by && pxw > bx && px < bxw;
        bottom_col = py <= byh && pyh > byh && pxw > bx && px < bxw;

        switch (gobj->type) {
            case GOBJ_TYPE_PLAYER:
                if (right_col) {
                    gobj->sprite->x = bx - pw;
                }
                if (left_col) {
                    gobj->sprite->x = bxw;
                }
                if (top_col) {
                    gobj->sprite->y = by - ph;
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
                    if (t.dest_x > -1) {
                        gobj->sprite->x = t.dest_x;
                    }
                    if (t.dest_y > -1) {
                        gobj->sprite->y = t.dest_y;
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
