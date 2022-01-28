#include <string.h>
#include <stdio.h>
#include "../header/MemUtils.h"

#include "../header/GameObject.h"
#include "../header/Map.h"

void test_map() {
    map_init(1);
}

void string_stuff() {
    // Let's try to turn this into an array of strings
    int i;
    char yolo[] = "Hey;Youdo good;mofo\0";
    char **arr = MEM_MALLOC_3_STRS(strlen(yolo));

    size_t n_strs = 0;

    char *tok = strtok(yolo, "!");
    while (tok != NULL) {
        size_t n = strlen(tok) + 1;
        arr[n_strs] = MEM_MALLOC_3_STRS(n);
        strcpy(arr[n_strs], tok);
        printf("tok=%s\n", tok);
        tok = strtok(NULL, ";");
        n_strs++;
    }
    MEM_FREE_STRS(arr, i, n_strs);
}

int main() {
    logr_log_tmp("Hello Hacksx!");
    string_stuff();
//    test_map();
}

