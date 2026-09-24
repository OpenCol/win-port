/* europe_draw_left -- shipped at 1048:978c.
 *
 * Ported by tools/port.py from matched/game/europe_draw_left.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_word_dcda;                          SEG20:0xdcda
 *   extern char far g_report_font_small;                 DGROUP:0x4b0a
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *
 * The file's own EuropeWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_rect_to_screen(EuropeWin *w, RECT *r);
void set_current_nation(col_i16 n);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern char * nth_text_string(col_i16 n);

extern void TextColor(col_i16 v);
extern void draw_string_shadowed(EuropeWin *w, void *font, char *s,
                                     col_i16 x, col_i16 y, col_i16 a);
extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 unit_chain_next(col_i16 u);


/* 5-byte NOP form, so extern (rule 47) */
extern void colony_blit_cell_1048_9d70(EuropeWin *w, col_i16 x, col_i16 y, col_i16 cx, col_i16 cy);
extern void europe_draw_ship(EuropeWin *w, col_i16 u, col_i16 x, col_i16 a, col_i16 b,
                              col_i16 *slot, col_i16 c);

/* same object: 4-byte `push cs; call ` (rule 47) */



void europe_draw_left(EuropeWin *w, col_i16 refresh)
{
    col_i16 slot;
    RECT r;
    char buf[0x50];
    register col_i16 u;
    register col_i16 x;

    set_current_nation(w->nation);
    colony_blit_cell_1048_9d70(w, 2, 0x102, 0x8b, 0x79);
    strcpy(buf, nth_text_string(g_word_dcda));
    TextColor(0x77);
    draw_string_shadowed(w, &g_report_font_small, buf, w->ox + 0x47, w->oy + 0x10c, 0);
    x = 3;
    slot = 0;
    u = unit_at(g_cur_nation - 0x10, g_cur_nation - 0x10);
    while (u >= 0) {
        europe_draw_ship(w, u, x, 0xd, 1, &slot, -1);
        u = unit_chain_next(u);
    }
    u = unit_at(g_cur_nation - 0xc, g_cur_nation - 0xc);
    while (u >= 0) {
        europe_draw_ship(w, u, x, 0xd, 1, &slot, -1);
        u = unit_chain_next(u);
    }
    if (refresh != 0) {
        SetRect(&r, w->ox + 2, w->oy + 0x102, w->ox + 0x8d, w->oy + 0x17b);
        copy_rect_to_screen(w, &r);
    }
}
