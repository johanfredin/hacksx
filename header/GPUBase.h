#ifndef PSX_GPU_BASE_H
#define PSX_GPU_BASE_H

#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

// Constants
#define GPUB_NUM_BUFFERS 0x2
#define GPUB_OT_LENGTH 0xA
#define GPUB_MODE_NTSC 0x0
#define GPUB_MODE_PAL 0x1
#define GPUB_ROT_ONE 0x1000
#define GPUB_PACKET_MAX 1000 * 0x40
#define GPUB_FRAME_BUFFER_WIDTH 0x400
#define GPUB_FRAME_BUFFER_HEIGHT 0x200

#define GPUB_FPS_30 2
#define GPUB_FPS_60 0

#define GPUB_SET_BRIGHTNESS(rgb_holder, brightness) rgb_holder->r = rgb_holder->g = rgb_holder->b =  brightness

#define GPUB_GS_SORT_FAST_OBJ(gs_obj) GsSortFastSprite(gs_obj, gpub_curr_ot(), gpub_gs_prio--)

#define GPUB_GS_SORT_POLY(poly) GsSortPoly(poly, gpub_curr_ot(), gpub_gs_prio--)

#define GPUB_GS_SORT_OBJ(gs_obj) GsSortSprite(gs_obj, gpub_curr_ot(), gpub_gs_prio--)

#define GPUB_BG_COLOR_R 0
#define GPUB_BG_COLOR_G 0
#define GPUB_BG_COLOR_B 0

#define GPUB_SET_IF_NOT_NULL(assignee, assigner)    \
if (assigner == 0) {                                \
    logr_log(ERROR, "GPUBase.c", "GPUB_SET_IF_NOT_NULL", "width and/or height is 0. Make sure to call gpub_init_screen with the width and height you want."); \
    exit(1);                                        \
} else {                                            \
    assignee = assigner;                            \
}                                                   \

typedef struct Color {
    u_char r, g, b;
} Color;

// Prototypes
void gpub_init_screen(u_short screen_w, u_short screen_h, u_char region_mode, u_char fps);
void gpub_display();
void gpub_set_bg_color(Color *color);
void gpub_clear_display();
GsOT* gpub_curr_ot();

/**
 * Get the width of the screen. if width have not been set yet in gpub_init_screen. Program will exit
 * @return the width of the screen if gpub_init_screen has been called
 */
u_short gpub_screen_w();
/**
 * Get the height of the screen. if height have not been set yet in gpub_init_screen. Program will exit
 * @return the height of the screen if gpub_init_screen has been called
 */
u_short gpub_screen_h();

extern u_short gpub_gs_prio;

#endif // PSX_GPU_BASE_H
