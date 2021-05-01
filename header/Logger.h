#ifndef PSX_LOGGER_H
#define PSX_LOGGER_H

#include <stdarg.h>

#define ERROR 0
#define WARN 1
#define INFO 2
#define DEBUG 3
#define TRACE 4

#define LOG_LEVEL INFO

#define LOGR_LOG_GOBJ(level, gobj)                \
    logr_log(level, "*********************");     \
    logr_log(level, "*   Game Object     *");     \
    logr_log(level, "*********************");     \
    logr_log(level, "w=%d", gobj->w);             \
    logr_log(level, "h=%d", gobj->h);             \
    logr_log(level, "sprite=%d", gobj->sprite);   \
    logr_log(level, "x_speed=%d", gobj->x_speed); \
    logr_log(level, "y_speed=%d", gobj->y_speed); \
    logr_log(level, "health=%d", gobj->health);   \
    logr_log(level, "spawn_x=%d", gobj->spawn_x); \
    logr_log(level, "spawn_y=%d", gobj->spawn_y); \
    logr_log(level, "type=%d", gobj->type)

/*
 * Log a message at specified level
 * @param level log level (0=err, 1=warn, 2=info, 3=debug, 4=trace)
 * @param msg the message to print
 */
void logr_log(unsigned char level, char *msg, ...);

#endif // PSX_LOGGER_H
