//
// Created on 1/1/2022.
//
#include "../header/GPUBase.h"
#include "../header/TextBox.h"
#include "../header/MemUtils.h"
#include "../header/FntLogger.h"

#include <LIBETC.H>

#define set_tile_pos(t, tx, ty, tw, th) \
    setXY0(t, tx, ty);                  \
    (t)->w = tw;                        \
    (t)->h = th

u_long curr_btn = 0;
u_long prev_btn = 0;

void add_canvas_to_dlg(DlgBox *box, short x, short y, short w, short h, VerticalGoraudColor *color) {
    SetPolyG4(&box->canvas);
    setRGB0(&box->canvas, color->r, color->g, color->b);
    setRGB1(&box->canvas, color->r, color->g, color->b);
    setRGB2(&box->canvas, color->r + color->ch_r, color->g + color->ch_g, color->b + color->ch_b);
    setRGB3(&box->canvas, color->r + color->ch_r, color->g + color->ch_g, color->b + color->ch_b);
    setXYWH(&box->canvas, x, y, w, h);
}

void add_border_to_box(DlgBox *box, short x, short y, short w, short h, u_char idx, Color *color) {
    SetTile(&box->border[idx]);
    setRGB0(&box->border[idx], color->r, color->g, color->b);
    set_tile_pos(&box->border[idx], x, y, w, h);
}

void tbx_draw(DlgBox *box) {
    u_short i;
    if (box->complete || !box->started) {
        return;
    }

    GPUB_GS_SORT_POLY(&box->canvas);
    for (i = 0; i < 8; i++) {
        GPUB_GS_SORT_POLY(&box->border[i]);
    }

    FNT_PRINT_BOX(box)
    txt_dlg_draw(box->dlg);
}

void tbx_tick(DlgBox *box) {
    short x = box->x, y = box->y;
    short w = box->w, h = box->h;

    if (box->complete) {
        return;
    }

    curr_btn = PadRead(1);
    if (curr_btn & PADstart) {
        box->started = 1;
    }

    if (box->started) {
        if (txt_dlg_complete(box->dlg)) {
            // Roll up dialog box when all dialog text is done and
            // player pressed to stop watching
            if (h <= 0) {
                box->complete = 1;
            } else {
                box->dlg->visible = 0;
                h -= TBX_SPEED;
            }
        } else {
            // Roll dialog box all the way down to target height
            if (h >= box->target_h) {
                h = box->target_h;
                txt_dlg_tick(box->dlg);
                if (curr_btn & PADLup) {
                    txt_next_msg(box->dlg, curr_btn != prev_btn);
                }
            } else {
                h += TBX_SPEED;
            }
        }
    }

    setXYWH(&box->canvas, x, y, w, h);
    set_tile_pos(&box->border[0], x, y - 2, w, 1);                   // top
    set_tile_pos(&box->border[1], x - 1, y - 1, w + 2, 1);    // top
    set_tile_pos(&box->border[2], x - 1, y, 1, h);                   // left
    set_tile_pos(&box->border[3], x - 2, y, 1, h);                   // left
    set_tile_pos(&box->border[4], x, h + y + 1, w, 1);               // bottom
    set_tile_pos(&box->border[5], x - 1, h + y, w + 2, 1);    // bottom
    set_tile_pos(&box->border[6], w + x, y, 1, h);                   // right
    set_tile_pos(&box->border[7], w + x + 1, y, 1, h);               // right
    box->w = w;
    box->h = h;
}

/*
 * Playground function not visible outside
 */
void update_free_float_box(DlgBox *box) {
    short x = box->x, y = box->y;
    short w = box->w, h = box->h;
    curr_btn = PadRead(1);
    if (curr_btn & PADLup) {
        h -= TBX_SPEED;
    }
    if (curr_btn & PADLdown) {
        h += TBX_SPEED;
    }
    if (curr_btn & PADLleft) {
        w -= TBX_SPEED;
    }
    if (curr_btn & PADLright) {
        w += TBX_SPEED;
    }

    if ((PADLup | PADLdown | PADLright | PADLleft) & curr_btn) {
        if (w >= box->target_w) {
            w = box->target_w;
        }
        if (h >= box->target_h) {
            h = box->target_h;
        }

        if (w <= 0) {
            w = 0;
        }
        if (h <= 0) {
            h = 0;
        }

        setXYWH(&box->canvas, x, y, w, h);
        set_tile_pos(&box->border[0], x, y - 2, w, 1);                   // top
        set_tile_pos(&box->border[1], x - 1, y - 1, w + 2, 1);    // top
        set_tile_pos(&box->border[2], x - 1, y, 1, h);                   // left
        set_tile_pos(&box->border[3], x - 2, y, 1, h);                   // left
        set_tile_pos(&box->border[4], x, h + y + 1, w, 1);               // bottom
        set_tile_pos(&box->border[5], x - 1, h + y, w + 2, 1);    // bottom
        set_tile_pos(&box->border[6], w + x, y, 1, h);                   // right
        set_tile_pos(&box->border[7], w + x + 1, y, 1, h);               // right
        box->x = x;
        box->y = y;
        box->w = w;
        box->h = h;
    }
}

DlgBox *tbx_init_dlg_box(short x, short y, short w, short h, short target_w, short target_h, VerticalGoraudColor *color, Dialog *content) {
    Color white = {255, 255, 255};
    Color grey = {155, 155, 155};
    u_char i = 0;
    DlgBox *box = MEM_MALLOC_3(DlgBox);

    add_canvas_to_dlg(box, x, y, w, h, color);

    // Border
    add_border_to_box(box, x, (short) (y - 2), w, 1, i++, &grey);                                       // top - outer
    add_border_to_box(box, (short) (x - 1), (short) (y - 1), (short) (w + 2), 1, i++, &white);    // top - inner
    add_border_to_box(box, (short) (x - 1), y, 1, h, i++, &white);                                      // left - outer
    add_border_to_box(box, (short) (x - 2), y, 1, h, i++, &grey);                                       // left - inner
    add_border_to_box(box, x, (short) (h + y + 1), w, 1, i++, &grey);                                   // bottom - outer
    add_border_to_box(box, (short) (x - 1), (short) (h + y), (short) (w + 2), 1, i++, &white);    // bottom - inner
    add_border_to_box(box, (short) (w + x), y, 1, h, i++, &white);                                      // right - outer
    add_border_to_box(box, (short) (w + x + 1), y, 1, h, i++, &grey);                                   // right - inner

    box->x = x;
    box->y = y;
    box->w = w;
    box->h = h;
    box->target_w = target_w;
    box->target_h = target_h;
    box->dlg = content;
    box->started = 0;
    box->complete = 0;
    return box;
}

