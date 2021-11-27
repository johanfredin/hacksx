#include "../header/Timer.h"
#include "../header/Logger.h"

void timer_log_time_taken(clock_t start_time) {
    start_time = clock() - start_time;
    double time_taken = ((double) start_time) / CLOCKS_PER_SEC;
    logr_log(INFO, "Timer.c", "log_time_taken", "Elapsed time=%f", time_taken);
}


