#include <Font.h>

#include "../header/Timer.h"

int main() {
    Font *fnt;
    fnt = font_load("FONT.TIM");
    clock_t start = clock();
    font_draw(fnt, "Hej mamma ~", 10, 10);
    timer_log_time_taken(start);
}