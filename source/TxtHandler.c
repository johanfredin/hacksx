#include <string.h>
#include "../header/TxtHandler.h"
#include "../header/MemUtils.h"
#include "../header/GPUBase.h"
#include "../header/Logger.h"


static const char FONT_CHARS[100] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{\\|}~ \0";

Font *txt_fnt_init(char *name, u_char cw, u_char ch, u_char padding) {
    CdrData *cdr_data = cdr_read_file(name);
    GsSPRITE *fnt_sprite = asmg_load_sprite(cdr_data, 0, 0, 128, ASMG_COLOR_BITS_8);
    Font *font = MEM_MALLOC_3(Font);
    font->fnt_sprite = fnt_sprite;
    font->cw = cw;
    font->ch = ch;
    font->padding = padding;
    return NULL;
}

Dialog *txt_dlg_init(char **strs, u_char n_strs, Font *fnt, u_short ticks_per_frame, u_char x, u_char y) {
    Dialog *dlg = MEM_MALLOC_3(Dialog);
    Message *msgs = MEM_CALLOC_3(n_strs, Message);
    u_char i;
    dlg->x = x;
    dlg->y = y;
    dlg->ticks_per_frame = ticks_per_frame;
    dlg->n_messages = n_strs;
    dlg->acc_ticks = 0;
    for (i = 0; i < n_strs; i++) {
        msgs[i] = *txt_msg_init(fnt, x, y, strs[i], dlg->ticks_per_frame == 0);
    }
    dlg->messages = msgs;
    LOGR_LOG_DLG(INFO, dlg);
    return dlg;
}

Message *txt_msg_init(Font *font, u_char x, u_char y, char *str, u_char make_static) {
    u_short i, x_offset = 0, y_offset = 0;
    u_short n_chars = strlen(str);
    Message *msg = MEM_MALLOC_3(Message);
    GsSPRITE *fnt_sprites = MEM_CALLOC_3(n_chars, GsSPRITE);
    logr_log(TRACE, "TxtHandler.c", "txt_msg_init", "n_chars=%d", n_chars);
    for (i = 0; i < n_chars; i++) {
        u_short j = 0, u = 0, v = 0, match = 0;
        char curr_ch = str[i];
        // Go to newline and jump to next character if newline char found
        if (curr_ch == '\n') {
            y_offset += (font->ch + font->padding);
            x_offset = 0;
            continue;
        }

        while (FONT_CHARS[j] != '\0') {
            if (curr_ch == FONT_CHARS[j]) {
                logr_log(TRACE, "TxtHandler.c", "txt_msg_init", "char='%c' found at index=%d", curr_ch, j);
                match = 1;

                // Match found in font, add new sprite to array
                asmg_get_region(font->fnt_sprite, &fnt_sprites[i], x + x_offset, y + y_offset, u, v, font->cw, font->ch);
                x_offset += (font->cw + font->padding);
                break;
            }
            if (u + font->cw >= font->fnt_sprite->w) {
                v += font->ch;
                u = 0;
            } else {
                u += font->cw;
            }
            j++;
        }
        if (!match) {
            logr_log(ERROR, "TxtHandler.c", "txt_msg_init", "No match found for char=%c, shutting down", curr_ch);
            exit(1);
        }
    }
    msg->strlen = n_chars;
    if (make_static) {
        TXT_MAKE_STATIC_MSG(msg);
    }
    msg->fnt_sprites = fnt_sprites;
    msg->active = 0;
    LOGR_LOG_MSG(INFO, msg);
    return msg;
}

void txt_dlg_draw(Dialog *dlg) {
    u_short i;
    for (i = 0; i < dlg->n_messages; i++) {
        txt_msg_draw(&dlg->messages[i]);
    }
}

void txt_msg_draw(Message *msg) {
    u_short j;
    for (j = 0; j < msg->strlen; j++) {
        GPUB_GS_SORT_FAST_OBJ(&msg->fnt_sprites[j]);
    }
}

void txt_dlg_tick(Dialog *dlg) {
    u_char i;
    if (TXT_IS_STATIC_DIALOG(dlg)) {
        // Static dialogs do not need to get updated
        return;
    }
    for (i = 0; i < dlg->n_messages; i++) {
        Message *msg = &dlg->messages[i];
        if (TXT_MSG_CHARS_ACQUIRED(msg)) {
            // All characters rendered
            continue;
        }

        if (TXT_DLG_CYCLE_COMPLETE(dlg)) {
            dlg->acc_ticks = 0;
            msg->acc_chars++;
        } else {
            dlg->acc_ticks++;
        }
    }

}


