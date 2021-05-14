#ifndef PSX_LOGGER_H
#define PSX_LOGGER_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define ERROR 0
#define WARN 1
#define INFO 2
#define DEBUG 3
#define TRACE 4

#define LOG_LEVEL DEBUG

/*
 * Log a message at specified level
 * @param level log level (0=err, 1=warn, 2=info, 3=debug, 4=trace)
 * @param src_file the name of the C source/header file that called the log function (will be included in log message)
 * @param src_func the name of the function that called the log function (will be included in log message)
 * @param msg the message to print
 */
void logr_log(unsigned char level, char *src_file, char *src_func, char *msg, ...);

#define LOGR_LOG_GOBJ(level, gobj)                                                    \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "*********************");     \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "*   Game Object     *", level);     \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "*********************", level);     \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "w=%d", gobj->w, level);             \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "h=%d", gobj->h, level);             \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "sprite=%d", gobj->sprite, level);   \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "x_speed=%d", gobj->x_speed, level); \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "y_speed=%d", gobj->y_speed, level); \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "health=%d", gobj->health, level);   \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "spawn_x=%d", gobj->spawn_x, level); \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "spawn_y=%d", gobj->spawn_y, level); \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "type=%d", gobj->type, level)

#define LOGR_LOG_TELEPORT(level, teleport)                                                     \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "*********************", level);          \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "*   Teleport        *", level);          \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "*********************", level);          \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "x=%d", (&teleport)->origin.x, level);    \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "y=%d", (&teleport)->origin.y, level);    \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "w=%d", (&teleport)->origin.w, level);    \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "h=%d", (&teleport)->origin.h, level);    \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "dest_x=%d", (&teleport)->dest_x, level); \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "dest_y=%d", (&teleport)->dest_y, level); \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "dest_frame=%d", (&teleport)->dest_frame, level)

#define LOGR_LOG_SPRITE(level, sprite) \
logr_log(level, "Logger.h", "LOGR_LOG_SPRITE", "SPRITE ADDED: {x:%d, y:%d, w:%d, h:%d, u:%d, v:%d}", sprite->x, sprite->y, sprite->w, sprite->h, sprite->u, sprite->v)


#endif // PSX_LOGGER_H
