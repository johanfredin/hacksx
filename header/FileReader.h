#ifndef BADJSON_FILEREADER_H
#define BADJSON_FILEREADER_H

#include <stdio.h>

FILE *fr_get_file(char *fileName);
char *fr_get_content(char *fileName);

#endif //BADJSON_FILEREADER_H
