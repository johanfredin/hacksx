#include "../header/MockPSXTypes.h"

void FntPrint(char *c, ...) {
    logr_log(INFO, "MockPSXTypes.c", "FntPrint", "FNT print...");
}

u_long PadRead(u_long btn) {
    return 1L;
}

u_short GetTPage(int tp, int abr, int x, int y) {
    return 1;
}