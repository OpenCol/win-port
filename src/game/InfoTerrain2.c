/* InfoTerrain2 -- shipped at 1058:8d08.
 *
 * Ported by tools/port.py from matched/game/InfoTerrain2.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_info_font_small;          DGROUP:0x4b12
 *   extern unsigned char far g_info_font_large;          DGROUP:0x4b16
 *   extern int far g_feature_words[];                    SEG20:0xd8d8
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_width_1058_bd2d(RECT *r);

extern void TextColor(col_i16 v);
extern void append_lparen(char *s);
extern void append_rparen(char *s);

extern void say_string(char *buf, col_i16 word);
extern void MSGetDesktopSize(RECT *r);
extern void draw_string_shadowed(void *w, void *font, char *s, col_i16 x, col_i16 y, col_i16 size);

/* The module's rectangle-width helper is defined here because the shipped call to it is the
 * compiler's own `push cs / call `. */


void InfoTerrain2(void *w, col_i16 kind, col_i16 *px, col_i16 *py)
{
    RECT desk;
    char buf[0x50];

    buf[0] = 0;
    TextColor(0xfa);
    append_lparen(buf);
    say_string(buf, g_feature_words[kind]);
    append_rparen(buf);
    MSGetDesktopSize(&desk);
    if (rect_width_1058_bd2d(&desk) <= 640)
        draw_string_shadowed(w, &g_info_font_small, buf, *px, *py, 5);
    else
        draw_string_shadowed(w, &g_info_font_large, buf, *px, *py, 5);
    *py += 17;
}
