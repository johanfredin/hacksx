//
// Created by lowrider on 2021-03-30.
//

#include "../header/FileReader.h"

FILE *fr_get_file(const char *fileName) {
    char *abs_path = _fullpath(NULL, fileName, 512);
    FILE *file;
    file = fopen(abs_path, "r");
    if (file == NULL) {
        printf("File='%s' not found!", fileName);
        exit(1);
    }
    return file;
}

char *fr_get_content(const char* fileName) {
    char *dot = strrchr(fileName, '.');
    if(dot && STREQ(dot, ".TIM")) {
        logr_log(WARN, "WARN\tcdr_read_file\tSkipping TIM files in hacksx, file skipped=%s", fileName);
        return NULL;
    }

    char *abs_path = _fullpath(NULL, NULL, 512);
    abs_path = strcat(abs_path, "\\..\\res\\");
    abs_path = strcat(abs_path, fileName);
    FILE *file;
    file = fopen(abs_path, "r");
    if (file == NULL) {
        printf("File='%s' not found!", fileName);
        exit(1);
    }

    char *buffer = NULL;
    size_t length;
    logr_log(INFO, "INFO\tcdr_read_file\tFile=%s retrieved", fileName);
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
    return buffer;
}
