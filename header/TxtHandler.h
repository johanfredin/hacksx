//
// Created on 12/12/2021.
//

#ifndef PSX_DEV_TXTHANDLER_H
#define PSX_DEV_TXTHANDLER_H

#include "../header/AssetManager.h"

#define TXT_DLG_CYCLE_COMPLETE(dlg) (dlg)->acc_ticks >= (dlg)->ticks_per_frame
#define TXT_MSG_CHARS_ACQUIRED(msg) (msg)->acc_chars >= (msg)->strlen
#define TXT_IS_STATIC_DIALOG(dlg) (dlg)->ticks_per_frame == 0
#define TXT_MAKE_STATIC_MSG(msg) (msg)->acc_chars = (msg)->strlen

typedef struct Font {
    // The sprite representing the whole font image
    GsSPRITE *fnt_sprite;
    // Character width of font image
    u_char cw;
    // Character height of font image
    u_char ch;
    // Padding between characters
    u_char padding;
} Font;

typedef struct Message {
    GsSPRITE *fnt_sprites;
    u_short strlen;
    u_short acc_chars;
    u_char active;
} Message;

typedef struct Dialog {
    Message *messages;
    u_char n_messages;
    u_char x;
    u_char y;
    u_char ticks_per_frame: 4;
    u_char acc_ticks: 4;
} Dialog;

/**
 * Load a font from cd and store on the heap.
 * @param name name of the font file e.g my_font.tim
 * @param cw Character width of font image
 * @param ch Character height of font image
 * @param padding Padding between characters
 * @return pointer to font from cd
 */
Font *txt_fnt_init(char *name, u_char cw, u_char ch, u_char padding);

Dialog *txt_dlg_init(char **strs, u_char n_strs, Font *fnt, u_short ticks_per_frame, u_char x, u_char y);

//Message *txt_animated_msg_init(Message *msg, u_short ticks_per_frame);
Message *txt_msg_init(Font *font, u_char x, u_char y, char *str, u_char make_static);

void txt_msg_draw(Message *msg);
void txt_dlg_draw(Dialog *dlg);
void txt_dlg_tick(Dialog *dlg);

#endif //PSX_DEV_TXTHANDLER_H
