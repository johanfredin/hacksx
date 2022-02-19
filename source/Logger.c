#include "../header/Logger.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

char *get_level(unsigned char level) {
    switch (level) {
        case OFF:
            return "OFF";
        case WARN:
            return "WARN";
        case ERROR:
            return "ERROR";
        case INFO:
            return "INFO";
        case DEBUG:
            return "DEBUG";
        case TRACE:
            return "TRACE";
        default:
            logr_log(ERROR, "Logger.c", "get_level", "Unknown log level=%d, shutting down...", level);
            exit(1);
    }
}

void logr_log_tmp(char *msg, ...) {
    logr_log(OFF, "Logger.c", "TMP", msg);
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
                case 'X':
                case 'x':
                    printf("%X", va_arg(args, int));
                    break;
                case 'c':
                    printf("%c", va_arg(args, char));
                    break;
                case 'p':
                    printf("%p", va_arg(args, int));
                    break;
            }
            msg++;
        }
        putchar('\n');
    }
}