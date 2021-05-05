//
// Created by lowrider on 2021-03-22.
//

#ifndef PSX_SPIKE_TILED_H
#define PSX_SPIKE_TILED_H

#include "JSONParser.h"

typedef enum Object_Layer_Type {
    BOUNDS,
    TELEPORTS
} Object_Layer_Type;


typedef struct ObjectLayer_Bounds {
    u_int width, height;
    u_int id;
    u_char visible;
    u_int x, y;
    struct ObjectLayer_Bounds *next;
} ObjectLayer_Bounds;

typedef struct ObjectLayer_Teleport {
    u_int width, height;
    u_int id;
    u_char visible;
    u_int x, y, dest_x, dest_y;
    u_char dest_frame;
    struct ObjectLayer_Teleport *next;
} ObjectLayer_Teleport;


typedef struct Layer_Data {
    u_short id;
    struct Layer_Data *next;
} Layer_Data;

typedef struct Tile_Layer {
    Layer_Data *data;
    u_int height, width, id;
    char *name, *type;
    u_char visible;
    int x, y;
    struct Tile_Layer *next;
} Tile_Layer;

typedef struct Tile_Map {
    int width, height, tile_width, tile_height;
    Tile_Layer *layers;
    ObjectLayer_Bounds *bounds;
    ObjectLayer_Teleport *teleports;
    u_char bounds_cnt;
    u_char teleports_cnt;
} Tile_Map;

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
