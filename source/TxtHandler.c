#include <string.h>
#include "../header/TxtHandler.h"
#include "../header/MemUtils.h"
#include "../header/GPUBase.h"

#define DLG_CYCLE_COMPLETE(msg, dlg) (msg)->acc_ticks >= (dlg)->ticks_per_frame
#define IS_STATIC_DIALOG(dlg) (dlg)->ticks_per_frame == 0


static const char FONT_CHARS[100] = "!\"#$%&'()*+,-./0123456789:;<=>?@"
                                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
                                    "abcdefghijklmnopqrstuvwxyz{\\|}~ \0";

u_char g_active_msg_idx = 0;

Font *txt_fnt_init(char *name, u_char cw, u_char ch, u_char padding) {
    CdrData *cdr_data = cdr_read_file(name);
    GsSPRITE *fnt_sprite = asmg_load_sprite(cdr_data, 0, 0, 128, ASMG_COLOR_BITS_8);
    Font *font = MEM_MALLOC_3(Font);
    font->fnt_sprite = fnt_sprite;
    font->cw = cw;
    font->ch = ch;
    font->padding = padding;
    CDR_DATA_FREE(cdr_data)
    return font;
}

Dialog *txt_dlg_init(char **strs, char *id, u_char n_messages, Font *fnt, u_short ticks_per_frame, u_char x, u_char y, u_char visible) {
    Dialog *dlg = MEM_MALLOC_3(Dialog);
    Message *msgs = MEM_CALLOC_3(n_messages, Message);
    u_char i;
    dlg->id = id;
    dlg->x = x;
    dlg->y = y;
    dlg->ticks_per_frame = ticks_per_frame;
    dlg->n_messages = n_messages;
    dlg->visible = visible;
    for (i = 0; i < n_messages; i++) {
       txt_msg_init(&msgs[i], fnt, x, y, strs[i], dlg->ticks_per_frame == 0, i == 0);
    }
    dlg->messages = msgs;
    g_active_msg_idx = 0;
    LOGR_LOG_DLG(INFO, dlg);
    return dlg;
}

void *txt_msg_init(Message *msg, Font *font, u_char x, u_char y, char *str, u_char make_static, u_char active) {
    u_short i, x_offset = 0, y_offset = 0;
    u_short n_chars = strlen(str);
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
        msg->acc_chars = n_chars;
    }
    msg->fnt_sprites = fnt_sprites;
    msg->active = active;
    LOGR_LOG_MSG(INFO, msg);
}

void txt_dlg_draw(Dialog *dlg) {
    if (dlg->visible) {
        txt_msg_draw(&dlg->messages[g_active_msg_idx]);
    }
}

void txt_msg_draw(Message *msg) {
    u_short j;
    for (j = 0; j < msg->acc_chars; j++) {
        GPUB_GS_SORT_FAST_OBJ(&msg->fnt_sprites[j]);
    }
}

void txt_next_msg(Dialog *dlg, u_char can_skip) {
    Message *curr_msg = &dlg->messages[g_active_msg_idx];
    if (TXT_MSG_CHARS_ACQUIRED(curr_msg) && can_skip) {
        curr_msg->active = 0;
        if (g_active_msg_idx < dlg->n_messages - 1) {
            Message *next = &dlg->messages[++g_active_msg_idx];
            next->active = 1;
        }
    } else {
        // Speed up the process
        curr_msg->acc_ticks += dlg->ticks_per_frame;
    }
}

void txt_dlg_tick(Dialog *dlg) {
    Message *msg;
    if (IS_STATIC_DIALOG(dlg) || !dlg->visible) {
        /*
         * Static dialogs do not need to get updated and
         * not visible dialogs SHOULD not get updated
         */
        return;
    }

    msg = &dlg->messages[g_active_msg_idx];
    if (msg->active) {
        if (TXT_MSG_CHARS_ACQUIRED(msg)) {
            // All characters rendered
            return;
        }

        if (DLG_CYCLE_COMPLETE(msg, dlg)) {
            msg->acc_ticks = 0;
            msg->acc_chars++;
        }
        msg->acc_ticks++;
    }
}

u_char txt_dlg_complete(Dialog *dlg) {
    u_char i;
    for (i = 0; i < dlg->n_messages; i++) {
        if (TXT_MSG_CHARS_ACQUIRED(&dlg->messages[i]) == 0) {
            return 0;
        }
    }
    return TXT_DLG_LAST_MSG_READ(dlg);
}