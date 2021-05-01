//
// Created by lowrider on 2021-03-22.
//

#ifndef PSX_SPIKE_TILED_H
#define PSX_SPIKE_TILED_H

#include "parser.h"

typedef unsigned int u_int;
typedef unsigned short u_short;
typedef unsigned char u_char;

typedef enum Object_Layer_Type {
    BOUNDS,
    TELEPORTS
} Object_Layer_Type;

typedef struct Object_Layer {
    u_int width, height;
    u_int id;
    u_char visible;
    u_int x, y;
    struct Object_Layer *next;
} Object_Layer;

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
    Object_Layer *bounds, *teleports;
    u_char bounds_cnt;
    u_char teleports_cnt;
} Tile_Map;

/**
 * Populates a new Tile_Map type from JSON data received. This function allocates
 * memory and must be either freed manually or by calling #tiled_free
 * @param root root entry in the JSON Data linked list
 * @param print_result whether or not to printf the resulting structure
 * @return a Tile_Map struct allocated on the heap.
 */
Tile_Map *tiled_populate_from_json(JSON_Data *root, unsigned char print_result);
/**
 * Frees all the allocated memory in linked list passed in.
 * @param tm tm entry of linked list
 */
void tiled_free(Tile_Map *tm);

#endif //PSX_SPIKE_TILED_H
