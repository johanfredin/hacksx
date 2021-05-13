#include "../header/CdReader.h"
#include "../header/FileReader.h"


void cdr_open() {
  // Mock
}

void cdr_close() {
    // Mock
}

CdrData *cdr_create_data_entry(char *name) {
    CdrData *asset = MEM_MALLOC_3(CdrData);
    asset->name = name;
    asset->file = NULL;
    return asset;
}

CdrData *cdr_find_data_entry(char* name, CdrData **assets, u_char assets_cnt) {
    int i;
    for(i = 0; i < assets_cnt; i++) {
        if(STREQ(assets[i]->name, name)) {
            logr_log(TRACE, "TRACE - Name=%s, found at index=%d", assets[i]->name, i);
            return assets[i];
        }
    }
    logr_log(ERROR, "CdReader.c", "cdr_find_data_entry", "No CdrData with name='%s' in passed in array, terminating...", name);
    exit(1);
}

CdrData *cdr_read_file(char *file_path) {
    CdrData *data = malloc(sizeof(CdrData));
    data->name = file_path;
    data->file = (long *) fr_get_content(file_path);
    return data;
}

void cdr_data_free(CdrData *cdr_data) {
    MEM_FREE_3_AND_NULL(cdr_data->file);
    MEM_FREE_3_AND_NULL(cdr_data);
}

CdrData *cdr_read_single_file(char *file_path) {
    CdrData *cdr_data;
    cdr_open();
    cdr_data = cdr_read_file(file_path);
    cdr_close();
    return cdr_data;
}