#include "../header/GameObject.h"
#include "../header/JSONParser.h"
#include "../header/CdReader.h"
#include "../header/Tiled.h"
#include "../header/Map.h"

int main() {
    logr_log_tmp("Hello Hacksx!");
//    CdrData *json_cdr_data = cdr_read_file("ts8_tl.json");
//    u_long *content = json_cdr_data->file;
//    JSON_Data *json_map_data = jsonp_parse((char *)content);
//    Tile_Map *tile_map = tiled_populate_from_json(json_map_data);
//    tiled_print_map(INFO, tile_map);
    map_init(1);
}

