
#ifndef PSX_FONT_H
#define PSX_FONT_H

#include "../header/AssetManager.h"

typedef struct Font {
    // Name of font
    const char *name;
    // The sprite representing the font image
    GsSPRITE *fnt_sprite;
    // Character width of font image (4 bits max)
    u_char cw: 4;
    // Character height of font image (4 bits max)
    u_char ch: 4;
} Font;

/**
 * Load a font from cd and store on the heap.
 * @param name name of the font file e.g my_font.tim
 * @return pointer to font from cd
 */
Font *font_load(char *name);
/**
 * Draw text to the screen
 * @param font the font to use
 * @param str the text to draw
 * @param x x position
 * @param y y position
 */
void font_draw(Font *font, char *str, u_char x, u_char y);

#endif // PSX_FONT_H
