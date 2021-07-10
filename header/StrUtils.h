#ifndef PSX_STR_UTILS_H
#define PSX_STR_UTILS_H

#include <string.h>

#define STREQ(str1, str2) strcmp(str1, str2) == 0
#define SET_STR_LEN(i, str) while ((char) str[i] != '\0') i++

#define STR_TO_LOWERCASE(str)                                           \
    char *p;                                                            \
    for (p = str; *p; p++) *p = tolower(*p)

#define STR_READ_UNTIL(str, substr, stopchr)                                        \
    u_char ch_cnt;                                                                  \
    for (ch_cnt = 0; str[ch_cnt] != stopchr && str[ch_cnt] != '\0'; ch_cnt++) {     \
        substr[ch_cnt] = str[ch_cnt];                                               \
    }                                                                               \
    substr[ch_cnt] = '\0'

#define STR_CONTAINS(str_to_check, str_val_to_find) strstr(str_to_check, str_val_to_find) != NULL

#endif //PSX_STR_UTILS_H

