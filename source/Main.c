#include <string.h>
#include <stdio.h>
#include "../header/MemUtils.h"

#include "../header/GameObject.h"
#include "../header/JSONParser.h"
#include "../header/CdReader.h"
#include "../header/Tiled.h"
#include "../header/Map.h"

void test_map() {
    CdrData *json_cdr_data = cdr_read_file("ts8_tl.json");
    u_long *content = json_cdr_data->file;
    JSON_Data *json_map_data = jsonp_parse((char *) content);
    Tile_Map *tile_map = tiled_populate_from_json(json_map_data);
//    tiled_print_map(INFO, tile_map);
    map_init(1);
}

void string_stuff() {
    // Let's try to turn this into an array of strings
    int i;
    char yolo[] = "Hey;Youdo good;mofo\0";
//    char **arr = MEM_CALLOC_STR(strlen(yolo));
    char **arr = MEM_MALLOC_3_STRS(strlen(yolo));

    size_t n_strs = 0;

    char *tok = strtok(yolo, ";");
    while (tok != NULL) {
        size_t n = strlen(tok) + 1;
        arr[n_strs] = MEM_MALLOC_3_STRS(n);
        strcpy(arr[n_strs], tok);
        tok = strtok(NULL, ";");
        n_strs++;
    }
    MEM_FREE_STRS(arr, i, n_strs);
}

int main() {
    logr_log_tmp("Hello Hacksx!");
//    test_map();
    string_stuff();
}

