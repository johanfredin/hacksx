#ifndef PSX_DEV_FNTLOGGER_H
#define PSX_DEV_FNTLOGGER_H

#define FNT_PRINT_ANIM 0     // Set to 1 to FntPrint player animation to screen
#define FNT_PRINT_COORDS 0   // Set to 1 to FntPrint game object positions to screen
#define FNT_IS_FONT_BG 0     // Set to 1 to get FntPrint inside a black box

#define FNT_PRINT_ANIMATION()                                                                                                   \
if (FNT_PRINT_ANIM) {                                                                                                           \
    FntPrint("Acc=%d, curr_u=%d, heading=%d moving=%d\n", anim->acc_ticks, anim->curr_u, gobj->heading, GOBJ_IS_MOVING(gobj));  \
}

#define FNT_PRINT_GOBJ_COORDS(gobj)                                 \
if(FNT_PRINT_COORDS) {                                              \
    FntPrint("x=%d, y=%d\n", gobj->sprite->x, gobj->sprite->y);     \
    FntPrint("heading=%d\n", gobj->heading);                        \
}

#define FNT_PRINT_BLOCKS(curr_frame_n, frame_ptr)                               \
if (FNT_PRINT_COORDS) {                                                         \
    FntPrint("Current frame=%d\n", curr_frame_n);                               \
    FntPrint("Blocks in frame=%d\n", frame_ptr->collision_blocks->amount);      \
    FntPrint("Teleports in frame=%d\n", frame->t_amount);                       \
}

#endif //PSX_DEV_FNTLOGGER_H
