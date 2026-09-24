/* draw_gold_box -- shipped at 1040:9de6.
 *
 * Ported by tools/port.py from matched/game/draw_gold_box.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_draw_color;               DGROUP:0x337e
 *   extern char far g_font_small[];                      DGROUP:0x4b1e
 *   extern int g_stub_a;                                 
 *
 * The file's own Win dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: value -> track
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_rect_to_screen_1040_af03(void *w, RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void FillColor(void *w, RECT *r, col_i16 color);
extern void TextColor(col_i16 v);
extern char * LTOA1(col_i16 v, char *s, col_i16 radix);
extern void draw_string_shadowed(void *w, void *font, char *s, col_i16 x, col_i16 y, col_i16 a);



void draw_gold_box(Win *w)
{
    char buf[10];
    RECT r;

    SetRect(&r, 10, 12, 0x3a, 0x3c);
    OffsetRect(&r, w->ox, w->oy);
    FillColor(w, &r, g_draw_color);
    TextColor(12);
    LTOA1(w->track, buf, 10);
    draw_string_shadowed(w, g_font_small, buf, w->ox + 0x23, w->oy + 0x23, 0);
    copy_rect_to_screen_1040_af03(w, &r);
}
