#include "../header/GPUBase.h"

GsOT orderingTable[GPUB_NUM_BUFFERS];

int currentBuffer = 0;

Color bg_color;

u_short gpub_gs_prio;
u_short w, h;
u_char gpub_fps;

void gpub_init_screen(u_short screen_w, u_short screen_h, u_char region_mode, u_char fps) {
    w = screen_w;
    h = screen_h;
}

void gpub_clear_display() {
    // Mock
}

void gpub_display() {
    // Mock
}

void gpub_set_bg_color(Color *color) {
    bg_color.r = color->r;
    bg_color.g = color->g;
    bg_color.b = color->b;
}

// @return pointer to ordering table at current buffer
GsOT *gpub_curr_ot() {
    return &orderingTable[currentBuffer];
}

u_short gpub_screen_w() {
    return w;
}

u_short gpub_screen_h() {
    return h;
}