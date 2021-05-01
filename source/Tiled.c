//
// Created by lowrider on 2021-03-22.
//

#include "../header/Tiled.h"

void add_tile_layers_to_map(Tile_Map *tm, JSON_Data *jobj_root);
void add_data_to_layer(Tile_Layer *layer, JSON_Data *root);
void add_object_layers_to_map(Tile_Map *tm, JSON_Data *root, Object_Layer_Type type);

Tile_Map *tiled_populate_from_json(JSON_Data *root) {
    JSON_Data *curr;
    Tile_Map *tm;
    if (root == NULL) {
        logr_log(ERROR, "ERROR: root is NULL");
        exit(1);
    }
    tm = MEM_MALLOC_3(Tile_Map);
    for (curr = root; curr != NULL; curr = curr->next) {
        char *key = curr->key;
        void *value = curr->value;
        if (STREQ(key, "height")) {
            tm->height = *(int *) value;
        } else if (STREQ(key, "width")) {
            tm->width = *(int *) value;
        } else if (STREQ(key, "tileheight")) {
            tm->tile_height = *(int *) value;
        } else if (STREQ(key, "tilewidth")) {
            tm->tile_width = *(int *) value;
        } else if (STREQ(key, "layers")) {
            // Linked list of json bounds found
            add_tile_layers_to_map(tm, (JSON_Data *) value);
        }
    }

    return tm;
}

void tiled_free(Tile_Map *tm) {
    Tile_Layer *tile_layer;
    tile_layer = tm->layers;
    while (tile_layer != NULL) {
        Tile_Layer *aux;
        aux = tile_layer;
        tile_layer = tile_layer->next;
        MEM_FREE_LINKED_LIST(aux->data, Layer_Data);
        MEM_FREE_3_AND_NULL(aux);
    }
    MEM_FREE_LINKED_LIST(tm->bounds, Object_Layer);
    MEM_FREE_LINKED_LIST(tm->teleports, Object_Layer);
    MEM_FREE_3_AND_NULL(tm);
}


void tiled_print_map(Tile_Map *map) {
    Tile_Layer *tile_layer;
    Object_Layer *bounds_layer, *teleports_layer;
    logr_log(INFO, "Map parsed from JSON");
    logr_log(INFO, "-------------------- ");
    logr_log(INFO, "Bounds cnt=%d", map->bounds_cnt);
    logr_log(INFO, "Teleports cnt=%d", map->teleports_cnt);
    logr_log(INFO, "{ ");
    logr_log(INFO, "  width=%d ", map->width);
    logr_log(INFO, "  height=%d ", map->height);
    logr_log(INFO, "  tile_width=%d ", map->tile_width);
    logr_log(INFO, "  tile_height=%d ", map->tile_height);
    logr_log(INFO, "  layers=[ ");
    for (tile_layer = map->layers; tile_layer != NULL; tile_layer = tile_layer->next) {
        logr_log(INFO, "    { ");
        logr_log(INFO, "      name=%s ", tile_layer->name);
        logr_log(INFO, "      type=%s ", tile_layer->type);
        logr_log(INFO, "      id=%d ", tile_layer->id);
        logr_log(INFO, "      x=%d ", tile_layer->x);
        logr_log(INFO, "      y=%d ", tile_layer->y);
        logr_log(INFO, "      width=%d ", tile_layer->width);
        logr_log(INFO, "      height=%d ", tile_layer->height);
        logr_log(INFO, "      visible=%d ", tile_layer->visible);
        logr_log(INFO, "    } ");
    }
    logr_log(INFO, "  ] ");
    logr_log(INFO, "  bounds=[ ");
    for (bounds_layer = map->bounds; bounds_layer != NULL; bounds_layer = bounds_layer->next) {
        logr_log(INFO, "    { ");
        logr_log(INFO, "      id=%d ", bounds_layer->id);
        logr_log(INFO, "      visible=%d ", bounds_layer->visible);
        logr_log(INFO, "      x=%d ", bounds_layer->x);
        logr_log(INFO, "      y=%d ", bounds_layer->y);
        logr_log(INFO, "      width=%d ", bounds_layer->width);
        logr_log(INFO, "      height=%d ", bounds_layer->height);
        logr_log(INFO, "      visible=%d ", bounds_layer->visible);
        logr_log(INFO, "    } ");
    }
    logr_log(INFO, "  ] ");
    logr_log(INFO, "  teleports=[ ");
    for (teleports_layer = map->teleports; teleports_layer != NULL; teleports_layer = teleports_layer->next) {
        logr_log(INFO, "    { ");
        logr_log(INFO, "      id=%d ", teleports_layer->id);
        logr_log(INFO, "      visible=%d ", teleports_layer->visible);
        logr_log(INFO, "      x=%d ", teleports_layer->x);
        logr_log(INFO, "      y=%d ", teleports_layer->y);
        logr_log(INFO, "      width=%d ", teleports_layer->width);
        logr_log(INFO, "      height=%d ", teleports_layer->height);
        logr_log(INFO, "      visible=%d ", teleports_layer->visible);
        logr_log(INFO, "    } ");
    }
    logr_log(INFO, "  ] ");
    logr_log(INFO, "} ");
}

void add_tile_layers_to_map(Tile_Map *tm, JSON_Data *jobj_root) {
    Tile_Layer *tl_root = MEM_MALLOC_3(Tile_Layer);
    JSON_Data *jobj_curr;
    Tile_Layer *tl_curr, *tl_prev;

    for (jobj_curr = jobj_root, tl_curr = tl_root; jobj_curr != NULL; jobj_curr = jobj_curr->next) {
        JSON_Data *curr_layer;
        Tile_Layer *next = NULL;
        for (curr_layer = (JSON_Data *) jobj_curr->value; curr_layer != NULL; curr_layer = curr_layer->next) {
            char *key = curr_layer->key;
            void *value = curr_layer->value;

            if (STREQ(key, "data")) {
                add_data_to_layer(tl_curr, (JSON_Data *) value);
            } else if (STREQ(key, "height")) {
                tl_curr->height = *(int *) value;
            } else if (STREQ(key, "id")) {
                tl_curr->id = *(int *) value;
            } else if (STREQ(key, "name")) {
                u_char is_bounds = STREQ((char *) value, "bounds");
                u_char is_teleports = STREQ((char *) value, "teleports");
                if (is_bounds || is_teleports) {
                    // FF until we reach objects array
                    while (!(STREQ(curr_layer->key, "objects"))) {
                        curr_layer = curr_layer->next;
                    }
                    // Value should now be at objects root
                    value = curr_layer->value;  // Sync
                    add_object_layers_to_map(
                            tm,
                            (JSON_Data *) value,
                            is_bounds ? BOUNDS : TELEPORTS
                    );
                    MEM_FREE_3_AND_NULL(tl_curr);
                    tl_curr = tl_prev;

                } else {
                    tl_curr->name = (char *) value;
                }
            } else if (STREQ(key, "type")) {
                tl_curr->type = (char *) value;
            } else if (STREQ(key, "visible")) {
                tl_curr->visible = *(u_char *) value;
            } else if (STREQ(key, "width")) {
                tl_curr->width = *(int *) value;
            } else if (STREQ(key, "x")) {
                tl_curr->x = *(int *) value;
            } else if (STREQ(key, "y")) {
                tl_curr->y = *(int *) value;
            }
        }
        if (jobj_curr->next != NULL) {
            next = MEM_MALLOC_3(Tile_Layer);
        }
        tl_curr->next = next;
        tl_prev = tl_curr;
        tl_curr = next;

    }
    tm->layers = tl_root;
}

void add_object_layers_to_map(Tile_Map *tm, JSON_Data *root, Object_Layer_Type type) {
    u_char objects_cnt;
    Object_Layer *ol_root, *ol_curr; // Our object layers linked list
    JSON_Data *curr_obj_layer; // Our current json object being iterated
    ol_root = MEM_MALLOC_3(Object_Layer);
    ol_curr = ol_root;
    for (curr_obj_layer = root, objects_cnt = 0; curr_obj_layer != NULL; curr_obj_layer = curr_obj_layer->next, objects_cnt++) {    // Iterate objects
        JSON_Data *entry_root = (JSON_Data *) curr_obj_layer->value;
        JSON_Data *entry_curr;
        for (entry_curr = entry_root; entry_curr != NULL; entry_curr = entry_curr->next) { // Iterate object properties
            char *key = entry_curr->key;
            void *value = entry_curr->value;

            if (STREQ(key, "height")) {
                ol_curr->height = *(u_int *) value;
            } else if (STREQ(key, "id")) {
                ol_curr->id = *(u_int *) value;
            } else if (STREQ(key, "visible")) {
                ol_curr->visible = *(u_char *) value;
            } else if (STREQ(key, "width")) {
                ol_curr->width = *(u_int *) value;
            } else if (STREQ(key, "x")) {
                ol_curr->x = *(u_int *) value;
            } else if (STREQ(key, "y")) {
                ol_curr->y = *(u_int *) value;
            }
        }
        MEM_MALLOC_3_AND_MOVE_TO_NEXT_IF_MORE_DATA(curr_obj_layer, ol_curr, Object_Layer)
    }

    if (type == BOUNDS) {
        tm->bounds = ol_root;
        tm->bounds_cnt = objects_cnt;
    } else {
        tm->teleports = ol_root;
        tm->teleports_cnt = objects_cnt;
    }
}

void add_data_to_layer(Tile_Layer *layer, JSON_Data *root) {
    Layer_Data *data_root = MEM_MALLOC_3(Layer_Data);
    Layer_Data *data = data_root;
    JSON_Data *curr;
    for (curr = root; curr != NULL; curr = curr->next) {
        data->id = *(u_short *) curr->value;
        MEM_MALLOC_3_AND_MOVE_TO_NEXT_IF_MORE_DATA(curr, data, Layer_Data)
    }
    layer->data = data_root;
}
