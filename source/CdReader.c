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
    logr_log(ERROR, "ERROR - No CdrData with name='%s' in passed in array, terminating...", name);
    exit(1);
}

CdrData *cdr_read_file(char *file_path) {
    CdrData *data = malloc(sizeof(CdrData));
    char *buffer = NULL;
    size_t length;
    FILE *file = fr_get_file(file_path);
    printf("File=%s retrieved\n", file_path);
    fseek(file, 0, SEEK_END);
    length = ftell(file) + 1;   // Reserved for null terminator
    fseek(file, 0, SEEK_SET);
    buffer = malloc(length);
    int i;
    for (i = 0; i < length; i++) {
        buffer[i] = 0;
    }
    if (buffer) {
        fread(buffer, 1, length, file);
        buffer[length - 1] = '\0';
    }

    data->name = file_path;
    data->file = buffer;
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