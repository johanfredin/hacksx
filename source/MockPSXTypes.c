#include "../header/MockPSXTypes.h"

void FntPrint(char *c, ...) {
    logr_log(INFO, "FNT print...");
}

u_long PadRead(btn) {
    return 1L;
}
