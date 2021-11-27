#include "../header/MemUtils.h"
#include "../header/Font.h"

static const char *FONT_CHARS = "\"!#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{\\|}~ ";

void set_fnt_sprite(GsSPRITE *sprite, Font *font, const char *str);

Font *font_load(char *name) {
    CdrData *cdr_data = cdr_read_file(name);
    GsSPRITE *fnt_sprite = asmg_load_sprite(cdr_data, 0, 0, 128, ASMG_COLOR_BITS_8);
    Font *font = MEM_MALLOC_3(Font);
    font->name = name;
    font->fnt_sprite = fnt_sprite;
    font->cw = 0x8;
    font->ch = 0xA;
    return font;
}

void font_draw(Font *font, char *str, u_char x, u_char y) {
    GsSPRITE font_sprite;
    set_fnt_sprite(&font_sprite, font, str);
//    GPUB_GS_SORT_OBJ(fnt_sprite);
}

void set_fnt_sprite(GsSPRITE *sprite, Font *font, const char *str) {
    u_short i=0;
    while(str[i] != '\0') {
        char ch = str[i];
        u_short j = 0;
        u_char match = 0;
        while(FONT_CHARS[j] != '\0') {
            char fch = FONT_CHARS[j];
            if (ch & fch) {
                logr_log(TRACE, "Font.c", "set_fnt_sprite", "char='%c' found at index=%d", ch, j);
                match = 1;
                break;
            }
            j++;
        }
        if (!match) {
            logr_log(ERROR, "Font.c", "set_fnt_sprite", "No match found for char=%c, shutting down", ch);
            exit(1);
        }
        i++;
    }
}