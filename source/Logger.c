#include "../header/Logger.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

char *get_level(unsigned char level) {
    switch (level) {
        case WARN:
            return "WRN";
        case ERROR:
            return "ERR";
        case INFO:
            return "INF";
        case DEBUG:
            return "DBG";
        case TRACE:
            return "TRC";
        default:
            logr_log(ERROR, "Logger.c", "get_level", "Unknown log level=%d, shutting down...", level);
            exit(1);
    }
}

void logr_log(unsigned char level, char *src_file, char *src_func, char *msg, ...) {
    va_list args;
    if (LOG_LEVEL >= level) {
        printf("%s - %s - %s - ", get_level(level), src_file, src_func);
                va_start(args, msg);
        while (*msg != '\0') {
            if (*msg != '%') {
                putchar(*msg);
                msg++;
                continue;
            }
            ++msg;
            switch (*msg) {
                case 's':
                    printf("%s", va_arg(args, char *));
                    break;
                case 'd':
                    printf("%d", va_arg(args, int));
                    break;
            }
            msg++;
        }
        putchar('\n');
    }
}