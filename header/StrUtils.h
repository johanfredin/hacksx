//
// Created by lowrider on 4/4/2021.
//

#ifndef PSX_STR_UTILS_H
#define PSX_STR_UTILS_H

#include <string.h>

#define STREQ(str1, str2) strcmp(str1, str2) == 0
#define SET_STR_LEN(i, str) while ((char) str[i] != '\0') i++

#endif //PSX_STR_UTILS_H

