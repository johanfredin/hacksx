#include "../header/Logger.h"

void logr_log(unsigned char level, char* c, ...) {
    va_list args;
    if (LOG_LEVEL >= level) {
        va_start(args, c);
        while (*c != '\0') {
            if (*c != '%') {
                putchar(*c);
                c++;
                continue;
            }
            ++c;
            switch (*c) {
            case 's':
                printf("%s", va_arg(args, char*));
                break;
            case 'd':
                printf("%d", va_arg(args, int));
                break;
            }
            c++;
        }
        putchar('\n');
    }
}