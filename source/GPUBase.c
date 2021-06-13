#include "../header/GPUBase.h"

#include "MemUtils.h"

void gpub_init_screen() {
   // Mock
}

void gpub_display() {
    // Mock
}

void gpub_clear_display() {
    // Mock
}

// GsGetActiveBuff() should have been called prior to calling this function
// @return pointer to ordering table at current buffer
GsOT* gpub_curr_ot() {
    return NULL;
}