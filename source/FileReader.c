#include <stdint.h>
#include <stdlib.h>

#include <Logger.h>
#include <StrUtils.h>
#include <unistd.h>
#include "../header/FileReader.h"

char *fr_get_content(char* fileName) {
    char buff[FILENAME_MAX];
    char *abs_path = getcwd(buff, sizeof(buff));
    abs_path = strcat(abs_path, "/../res/");
    abs_path = strcat(abs_path, fileName);
    FILE *file = fopen(abs_path, "r");
    if (file == NULL) {
        printf("File='%s' not found!", fileName);
        exit(1);
    }

    char *buffer = NULL;
    size_t length;
    logr_log(INFO, "FileReader.c", "fr_get_content", "File=%s retrieved", fileName);
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
