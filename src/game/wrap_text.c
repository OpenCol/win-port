/* wrap_text -- shipped at 1040:62e0.
 *
 * Ported by tools/port.py from matched/game/wrap_text.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy, STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_font_1ce4;                        DGROUP:0x1ce4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 font_text_extent_1040_a8dd(void *f, const char *s);
col_i16 get_w_2_1040_a8c4(void *r);
col_i16 rect_width_1040_a903(RECT *r);

extern unsigned char TextColor(unsigned char v);
extern void draw_string_shadowed(void *p, void *hf, const char *s,
                                     col_i16 x, col_i16 y, col_i16 align);




/* same object: 4-byte `push cs; call ` (rule 47) */




col_i16 wrap_text(void *port, RECT *r, char *text, col_i16 draw)
{
    char *p;
    col_i16 x;
    col_i16 y;
    col_i16 w;
    char ch;
    char hilite;
    char done;
    char line[0x100];
    char word[0x50];
    register col_i16 n;
    register col_i16 k;

    hilite = 0;
    done = 0;
    p = text;
    x = r->left;
    y = get_w_2_1040_a8c4(g_font_1ce4) / 2 + r->top;
    TextColor(0xfa);
    line[0] = word[0] = 0;
    w = 0;
    n = k = 0;
    while (done == 0) {
        if (*p == 0) {
            ch = '^';
            done = 1;
        } else {
            ch = *p;
            p++;
        }
        if ((col_i16)ch == '^') {
            if (draw != 0)
                draw_string_shadowed(port, g_font_1ce4, word, x, y, 1);
            y += get_w_2_1040_a8c4(g_font_1ce4) + 2;
            x = r->left;
            n = k = 0;
            line[0] = word[0] = 0;
            if ((col_i16)*p == ' ')
                p++;
        } else if ((col_i16)ch == -7) {
            strcat(word, " ");
            k = strlen(word);
            strcat(line, " ");
            n = strlen(line);
        } else if ((col_i16)ch == '{') {
            TextColor(0xc);
        } else if ((col_i16)ch == '}') {
            hilite = 1;
            ch = ' ';
        } else {
            line[n] = ch;
            line[n + 1] = 0;
            w = font_text_extent_1040_a8dd(g_font_1ce4, line);
            word[k] = ch;
            word[k + 1] = 0;
            n++;
            k++;
        }
        if (rect_width_1040_a903(r) < w) {
            y += get_w_2_1040_a8c4(g_font_1ce4) + 2;
            x = r->left;
            strcpy(line, word);
            w = font_text_extent_1040_a8dd(g_font_1ce4, line);
            n = strlen(line);
        }
        if ((col_i16)ch == ' ') {
            if (draw != 0)
                draw_string_shadowed(port, g_font_1ce4, word, x, y, 1);
            x += font_text_extent_1040_a8dd(g_font_1ce4, word);
            word[0] = 0;
            k = 0;
            if (hilite != 0) {
                TextColor(0xfa);
                hilite = 0;
            }
        }
    }
    return y;
}
