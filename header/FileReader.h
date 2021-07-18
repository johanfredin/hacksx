//
// Created by lowrider on 2021-03-30.
//

#ifndef BADJSON_FILEREADER_H
#define BADJSON_FILEREADER_H

#include <stdio.h>

FILE *fr_get_file(const char *fileName);
char *fr_get_content(const char *fileName);

#endif //BADJSON_FILEREADER_H
