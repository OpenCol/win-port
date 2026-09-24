/* europe_draw_dock_1048_95c1 -- shipped at 1048:95c1.
 *
 * Ported by tools/port.py from matched/game/europe_draw_dock_1048_95c1.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec far g_nation_recs[4];               SEG20:0x7828
 *   extern int far g_misc_words[];                       SEG20:0xdcc8
 *   extern char far g_report_font_small;                 DGROUP:0x4b0a
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *
 * The file's own NationRec, EuropeWin dropped for the merged record in col/records.h.
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

/* 0x34 bytes */
extern void say_string(char *buf, col_i16 word);
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



void europe_draw_dock_1048_95c1(EuropeWin *w, col_i16 refresh)
{
    col_i16 slot;
    RECT r;
    char buf[0x50];
    register col_i16 u;
    register col_i16 x;

    set_current_nation(w->nation);
    colony_blit_cell_1048_9d70(w, 0x8f, 0x102, 0x8b, 0x79);
    buf[0] = 0;
    say_string(buf, g_misc_words[10]);
    TextColor(0x77);
    draw_string_shadowed(w, &g_report_font_small, buf, w->ox + 0xd4, w->oy + 0x10c, 0);
    buf[0] = 0;
    strcat(buf, ((char *)((unsigned char *)&(g_nation_recs[g_cur_nation]) + 0x0)));
    draw_string_shadowed(w, &g_report_font_small, buf, w->ox + 0xd4, w->oy + 0x11b, 0);
    x = 0x90;
    slot = 0;
    u = unit_at(g_cur_nation - 0x1c, g_cur_nation - 0x1c);
    while (u >= 0) {
        europe_draw_ship(w, u, x, 0xd, 1, &slot, -1);
        u = unit_chain_next(u);
    }
    u = unit_at(g_cur_nation - 0x18, g_cur_nation - 0x18);
    while (u >= 0) {
        europe_draw_ship(w, u, x, 0xd, 1, &slot, -1);
        u = unit_chain_next(u);
    }
    if (refresh != 0) {
        SetRect(&r, w->ox + 0x8f, w->oy + 0x102, w->ox + 0x11a, w->oy + 0x17b);
        copy_rect_to_screen(w, &r);
    }
}
