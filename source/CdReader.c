#include <SYS/TYPES.H>
#include <LIBDS.H>

#include "../header/Logger.h"
#include "../header/CdReader.h"
#include "../header/MemUtils.h"
#include "../header/StrUtils.h"
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

CdrData *cdr_find_data_entry(char* name, CdrData **assets, unsigned char assets_cnt) {
    int i;
    for(i = 0; i < assets_cnt; i++) {
        if(STR_EQ(assets[i]->name, name)) {
            logr_log(TRACE, "CdReader.c", "cdr_find_data_entry" "Name=%s, found at index=%d", assets[i]->name, i);
            return assets[i];
        }
    }
    logr_log(ERROR, "CdReader.c", "cdr_find_data_entry", "No CdrData with name='%s' in passed in array, terminating...", name);
    exit(1);
}

CdrData *cdr_read_file(char *file_path) {
    CdrData *data = MEM_MALLOC_3(CdrData);
    data->name = file_path;
    data->file = (unsigned long *) fr_get_content(file_path);
    return data;
}

CdrData *cdr_read_single_file(char *file_path) {
    CdrData *cdr_data;
    cdr_open();
    cdr_data = cdr_read_file(file_path);
    cdr_close();
    return cdr_data;
}