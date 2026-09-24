/* draw_number_label -- shipped at 1038:3e9d.
 *
 * Ported by tools/port.py from matched/game/draw_number_label.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 font_text_extent_1038_8713(void *font, char *s);
col_i16 get_w_2_1038_8739(void *font);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern char * LTOA1(col_i16 v, char *s, col_i16 radix);

extern void FillColor(void *w, RECT *r, col_i16 color);
extern void TextColor(col_i16 v);
extern void draw_string_shadowed(void *w, void *font, char *s, col_i16 x, col_i16 y, col_i16 a);




void draw_number_label(void *w, void *font, col_i16 n, col_i16 x, col_i16 y, col_i16 color, col_i16 fill)
{
    register col_i16 px;
    register col_i16 py;
    col_i16 tw;
    col_i16 th;
    RECT r;
    char buf[20];

    px = x;
    py = y;
    if (n <= 0)
        goto done;
    py += 2;
    buf[0] = 0;
    LTOA1(n, buf, 10);
    tw = font_text_extent_1038_8713(font, buf);
    th = get_w_2_1038_8739(font);
    if (fill != 0) {
        SetRect(&r, px + 1, py + 1, px + tw + 1, py + th - 1);
        FillColor(w, &r, 0);
    }
    TextColor(color);
    draw_string_shadowed(w, font, buf, px + 1, th / 2 + py, 1);
    draw_string_shadowed(w, font, buf, px + 2, th / 2 + py, 1);
done:
    ;
}
