#include "../header/Map.h"

#define START_FRAME 0

Frame *frames;
CdrData **cdr_data_assets;

u_char assets_count;
u_char frame_cnt;
u_char current_frame = START_FRAME;

void init_frame(Frame *frame, char *bg, char *fg, char *gobj, char *json_map_file);
RECT get_rect(short x, short y, short w, short h);
RECT get_rect_with_offset(short x, short y, short w, short h, short x_offset, short y_offset);
TILE get_tile(short x, short y, short w, short h, u_short r, u_short g, u_short b);
void handle_block_collision(GameObject *gobj, Frame *frame);
void handle_teleport_collision(GameObject *gobj, Frame *frame);
u_char set_level_assets(u_char level);
SpriteLayer *load_layers(CdrData *img_data, Tile_Map *tile_map);

void map_init(u_char level) {
    u_char i;
    assets_count = set_level_assets(level);

    frames = MEM_CALLOC_3(10, Frame);
    init_frame(&frames[frame_cnt++], "TILES_8.TIM", NULL, NULL, "SANDBOX.JSON");
    // init_frame(&frames[frame_cnt++], "01BG.TIM", "01FG.TIM", "RAICHU.TIM", "0_1.JSON");
    // init_frame(&frames[frame_cnt++], "10BG.TIM", "10FG.TIM", "RAICHU_2.TIM", "1_0.JSON");
    // init_frame(&frames[frame_cnt++], "11BG.TIM", "11FG.TIM", "ALOLA.TIM", "1_1.JSON");
    // init_frame(&frames[frame_cnt++], "YOLOBG.TIM", "YOLOFG.TIM", NULL, "04.JSON");
    // init_frame(&frames[frame_cnt++], "1_TUNNEL.TIM", NULL, NULL, "05.JSON");
    // init_frame(&frames[frame_cnt++], "1_H2.TIM", NULL, NULL, "06.JSON");

    // Cleanup
    for (i = 0; i < assets_count; i++) {
        cdr_data_free(cdr_data_assets[i]);
    }
    MEM_FREE_3_AND_NULL(cdr_data_assets);
}

u_char set_level_assets(u_char level) {
    u_char asset_cnt = 0;
    logr_log(INFO, "Map.c", "set_level_assets", "*********************************");
    logr_log(INFO, "Map.c", "set_level_assets", "* ADDING ASSETS FOR LEVEL NR %d *", level);
    logr_log(INFO, "Map.c", "set_level_assets", "*********************************");
    cdr_open();
    if (level == 1) {
        cdr_data_assets = MEM_CALLOC_3_PTRS(24, CdrData);
        cdr_data_assets[asset_cnt++] = cdr_read_file("SANDBOX.JSON");
        cdr_data_assets[asset_cnt++] = cdr_read_file("SB_BG.TIM");
        cdr_data_assets[asset_cnt++] = cdr_read_file("TILES_8.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("0_1.JSON");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("1_0.JSON");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("1_1.JSON");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("04.JSON");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("05.JSON");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("06.JSON");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("00BG.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("00FG.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("01BG.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("01FG.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("10BG.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("11BG.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("10FG.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("11FG.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("1_H2.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("1_TUNNEL.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("PSYDUCK.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("RAICHU.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("YOLOBG.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("YOLOFG.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("RAICHU_2.TIM");
        // cdr_data_assets[asset_cnt++] = cdr_read_file("ALOLA.TIM");
    }
    cdr_close();
    logr_log(DEBUG, "Map.c", "set_level_assets", "%d assets read", asset_cnt);
    return asset_cnt;
}

void init_frame(Frame *frame, char *bg, char *fg, char *gobj, char *json_map_file) {
    // Declarations --------------------------
    CdrData *bg_cdr_data, *json_cdr_data;
    u_long *content;
    JSON_Data *map_data;
    Tile_Map *tile_map;
    ObjectLayer_Bounds *curr_b;
    ObjectLayer_Teleport *curr_t;
    CollisionBlock *collision_blocks;
    Teleport *teleports;
    u_char blocks_cnt, teleports_cnt, i;

    // Parse json file into tile map
    json_cdr_data = cdr_find_data_entry(json_map_file, cdr_data_assets, assets_count);
    content = json_cdr_data->file;
    map_data = jsonp_parse((char *)content);
    tile_map = tiled_populate_from_json(map_data);

    // INIT SPRITES -----------------------------------------------------------
    bg_cdr_data = cdr_find_data_entry(bg, cdr_data_assets, assets_count);
    frame->bg = asmg_load_sprite_w_offset(bg_cdr_data, 0, 0, 128, COLOR_BITS_4, tile_map->offset_x, tile_map->offset_y); // BG can not be NULL so no check
    frame->sprite_layers = load_layers(bg_cdr_data, tile_map);
    if (fg != NULL) {
        CdrData *fg_cdr_data = cdr_find_data_entry(fg, cdr_data_assets, assets_count);
        frame->fg = asmg_load_sprite_w_offset(fg_cdr_data, 0, 0, 128, COLOR_BITS_8, tile_map->offset_x, tile_map->offset_y);
    }
    if (gobj != NULL) {
        CdrData *gobj_cdr_data = cdr_find_data_entry(gobj, cdr_data_assets, assets_count);
        frame->game_object = gobj_init(asmg_load_sprite(gobj_cdr_data, 90, 120, 128, COLOR_BITS_8), 16, 16, 1, 1, 100, GOBJ_TYPE_NPC);
    }

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

        teleports[i].dest_x = (curr_t->dest_x == 0) ? -1 : curr_t->dest_x;
        teleports[i].dest_y = (curr_t->dest_y == 0) ? -1 : curr_t->dest_y;
        teleports[i].dest_frame = curr_t->dest_frame;

        if (GPUB_DRAW_BOUNDS) {
            teleports[i].t_bound_lines = get_tile(x, y, curr_t->width, curr_t->height, 0, 0, 255);
        }
        logr_log(INFO, "Map.c", "init_frame", "Dest x=%d y=%dframe=%dname=%s", teleports[i].dest_x, teleports[i].dest_y, teleports[i].dest_frame, json_map_file);
    }
    frame->t_amount = teleports_cnt;
    frame->teleports = teleports;

    // Housekeeping
    jsonp_free(map_data);
    tiled_free(tile_map);
}

SpriteLayer *load_layers(CdrData *img_data, Tile_Map *tile_map) {
    u_short x, y, u, v, i;
    u_short map_w, map_h, cols, rows;
    SpriteLayer *sprite_layers;

    Layer_Data *curr = tile_map->layers->data;  // Will be several sprite_layers in real life later!

    // Calculate tiles amount on x and y axis
    map_w = tile_map->width * tile_map->tile_width;
    map_h = tile_map->height * tile_map->tile_height;
    cols = map_w / tile_map->tile_width;
    rows = map_h / tile_map->tile_height;

    sprite_layers = MEM_MALLOC_3(SpriteLayer);
    sprite_layers->sprite_regions = MEM_CALLOC_3(cols * rows, GsSPRITE);

    // We will need to fetch multiple sprite_layers later and figure out what goes where,
    // but for now just fetch the bg sprite
    GsSPRITE *sprite = asmg_load_sprite(img_data, 0, 0, 128, COLOR_BITS_8);
    i = 0;
    v = 0;
    // Iterate frame tile by tile and fetch appropriate texture region from tile set
    for (y = 0; y < rows; y++, v += tile_map->tile_height) {
        for (x = u = 0; x < cols; x++, u += tile_map->tile_width) {
            if(curr == NULL) {
                logr_log(ERROR, "Map.c", "load_layers", "Current layer data entry is null before end of loop, must be a mismatch, terminating...");
                exit(1);
            }

            asmg_get_region(sprite, &sprite_layers->sprite_regions[i], u, v, tile_map->tile_width, tile_map->tile_height);
            LOGR_LOG_SPRITE(DEBUG, sprite_layers->sprite_regions[i]);
            curr = curr->next;
            i++;
        }
    }
    return sprite_layers;
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
    Frame *frame = &frames[current_frame];
    CollisionBlock *blocks = frame->collision_blocks;
    Teleport *teleports = frame->teleports;

    if (frame->fg != NULL) {
        GsSortFastSprite(frame->fg, gpub_curr_ot(), 0);
    }
    if (frame->game_object != NULL) {
        gobj_draw(frame->game_object);
    }
    gobj_player_draw(player);
    if (frame->bg != NULL) {
        GsSortFastSprite(frame->bg, gpub_curr_ot(), 0);
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
