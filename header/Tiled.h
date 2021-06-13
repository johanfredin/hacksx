//
// Created by lowrider on 2021-03-22.
//

#ifndef PSX_SPIKE_TILED_H
#define PSX_SPIKE_TILED_H

#include "JSONParser.h"

typedef struct ObjectLayer_Bounds {
    unsigned int width, height;
    unsigned int id;
    unsigned char visible;
    unsigned int x, y;
    struct ObjectLayer_Bounds *next;
} ObjectLayer_Bounds;

typedef struct ObjectLayer_Teleport {
    unsigned int width, height;
    unsigned int id;
    unsigned char visible;
    unsigned int x, y;
    short dest_x, dest_y;
    unsigned char dest_frame;
    struct ObjectLayer_Teleport *next;
} ObjectLayer_Teleport;


typedef struct Layer_Data {
    unsigned short id;
    struct Layer_Data *next;
} Layer_Data;

typedef struct Tile_Layer {
    Layer_Data *data;
    unsigned short height, width, id, x, y, prio, active_sprites_cnt;
    char *name, *type, *layer_type;
    unsigned char visible;
    struct Tile_Layer *next;
} Tile_Layer;

typedef struct Tile_Map {
    unsigned short width, height, tile_width, tile_height;
    Tile_Layer *layers;
    ObjectLayer_Bounds *bounds;
    ObjectLayer_Teleport *teleports;
    unsigned char bounds_cnt, layers_cnt;
    unsigned char teleports_cnt;
    unsigned short offset_x, offset_y;
} Tile_Map;

/**
 * Make sure that passed in key=&quot;name&quot;<br>
 * Make sure that passed in val=&quot;value&quot;
 * @param key key to check
 * @param val value to check
 */
#define TILED_VALIDATE_PROP(key, val) \
    if(!(STREQ(key, "name"))) { \
        logr_log(ERROR, "Tiled.h", "TILED_VALIDATE_PROP", "property key='name' expected here, instead was=%s, exiting...", key); \
        exit(1); \
    }                                     \
    if(!(STREQ(val, "value"))) { \
        logr_log(ERROR, "Tiled.h", "TILED_VALIDATE_PROP", "property key='value' expected here, instead was='%s', exiting...", val); \
        exit(1); \
    }

/**
 * Populates a new Tile_Map type from JSON data received. This function allocates
 * memory and must be either freed manually or by calling #tiled_free
 * @param root root entry in the JSON Data linked list
 * @return a Tile_Map struct allocated on the heap.
 */
Tile_Map *tiled_populate_from_json(JSON_Data *root);

void tiled_print_map(Tile_Map *map);

/**
 * Frees all the allocated memory in linked list passed in.
 * @param tm tm entry of linked list
 */
void tiled_free(Tile_Map *tm);

#endif //PSX_SPIKE_TILED_H
