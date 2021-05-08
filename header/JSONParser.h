//
// Created by lowrider on 2021-03-22.
//

#ifndef PSX_SPIKE_PARSER_H
#define PSX_SPIKE_PARSER_H

#include <stdlib.h>
#include "StrUtils.h"
#include "MemUtils.h"
#include "Logger.h"
#include "MissingTypes.h"

typedef struct {
    unsigned char integer: 1;
    unsigned char decimal: 1;
    unsigned char str: 1;
    unsigned char boolean: 1;
    unsigned char arr: 1;
    unsigned char obj: 1;
} Type;

typedef struct JSON_Data {
    char *key;
    void *value;
    Type type;
    struct JSON_Data *next;
} JSON_Data;

JSON_Data *jsonp_parse(char *file_content);
void jsonp_print_data(JSON_Data *root);
void jsonp_free(JSON_Data *root);

#endif //PSX_SPIKE_PARSER_H
