#ifndef PSX_LOGGER_H
#define PSX_LOGGER_H

#define OFF 0
#define ERROR 1
#define WARN 2
#define INFO 3
#define DEBUG 4
#define TRACE 5

#define LOG_LEVEL INFO

/**
 * Log a message at specified level
 * @param level log level (0=err, 1=warn, 2=info, 3=debug, 4=trace)
 * @param src_file the name of the C source/header file that called the log function (will be included in log message)
 * @param src_func the name of the function that called the log function (will be included in log message)
 * @param msg the message to print
 */
void logr_log(unsigned char level, char *src_file, char *src_func, char *msg, ...);

#define LOGR_LOG_GOBJ(level, gobj)                                                    \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "*********************");     \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "*   Game Object     *");     \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "*********************");     \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "w=%d", (gobj)->w);             \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "h=%d", (gobj)->h);             \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "sprite=%d", (gobj)->sprite);   \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "x_speed=%d", (gobj)->x_speed); \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "y_speed=%d", (gobj)->y_speed); \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "health=%d", (gobj)->health);   \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "spawn_x=%d", (gobj)->spawn_x); \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "spawn_y=%d", (gobj)->spawn_y); \
    logr_log(level, "Logger.h", "LOGR_LOG_GOBJ", "type=%d", (gobj)->type)

#define LOGR_LOG_TELEPORT(level, teleport)                                              \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "*********************");          \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "*   Teleport        *");          \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "*********************");          \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "x=%d", (&(teleport))->origin.x);    \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "y=%d", (&(teleport))->origin.y);    \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "w=%d", (&(teleport))->origin.w);    \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "h=%d", (&(teleport))->origin.h);    \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "dest_x=%d", (&(teleport))->dest_x); \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "dest_y=%d", (&(teleport))->dest_y); \
    logr_log(level, "Logger.h", "LOGR_LOG_TELEPORT", "dest_frame=%d", (&(teleport))->dest_frame)

#define LOGR_LOG_SPRITE(level, sprite) \
    logr_log(level, "Logger.h", "LOGR_LOG_SPRITE", "SPRITE ADDED: {x:%d, y:%d, w:%d, h:%d, u:%d, v:%d}", sprite.x, sprite.y, sprite.w, sprite.h, sprite.u, sprite.v)

#define LOGR_LOG_GS_OBJ(level, gs_obj) logr_log(level, "Logger.h", "LOGR_LOG_GS_OBJ", "sprite x, y, u, v = {%d, %d, %d, %d}", (gs_obj)->x, (gs_obj)->y, (gs_obj)->u, (gs_obj)->v)

#define LOGR_LOG_DLG(level, dlg) \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "*********************");                           \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "*   Dialog          *");                           \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "*********************");                           \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "x=%d", (dlg)->x);                                  \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "y=%d", (dlg)->y);                                  \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "ticks_per_frame=%d", (dlg)->ticks_per_frame);      \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "n_messages=%d", (dlg)->n_messages);                \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "acc_ticks=%d", (dlg)->acc_ticks)

#define LOGR_LOG_MSG(level, msg) \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "*********************");                   \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "*   Message         *");                   \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "*********************");                   \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "strlen=%d", (msg)->strlen);                \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "acc_chars=%d", (msg)->acc_chars);          \
    logr_log(level, "Logger.h", "LOGR_LOG_DLG", "active=%d", (msg)->active)


#endif // PSX_LOGGER_H
