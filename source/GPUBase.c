#include <stdlib.h>
#include "../header/GPUBase.h"

u_short w, h;

void gpub_init_screen(u_short screen_w, u_short screen_h, u_char region_mode) {
    w = screen_w;
    h = screen_h;
   // Mock
}

void gpub_display() {
    // Mock
}

void gpub_clear_display() {
    // Mock
}

u_short gpub_screen_w() {
    if (w == 0) {
        logr_log(ERROR, "GPUBase.c", "gpub_screen_w", "width is 0. Make sure to call gpub_init_screen with the width and height you want.");
        exit(1);
    }
    return w;
}

u_short gpub_screen_h() {
    if (h == 0) {
        logr_log(ERROR, "GPUBase.c", "gpub_screen_h", "height is 0. Make sure to call gpub_init_screen with the width and height you want.");
        exit(1);
    }
    return h;
}

// GsGetActiveBuff() should have been called prior to calling this function
// @return pointer to ordering table at current buffer
GsOT* gpub_curr_ot() {
    return NULL;
}