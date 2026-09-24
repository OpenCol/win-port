/* MapWin_RevealHiddenTerrain -- shipped at 1038:a441.
 *
 * Ported by tools/port.py from matched/game/MapWin_RevealHiddenTerrain.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_map_mode;                           DGROUP:0x1ca4
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern int far g_view_nation;                        SEG20:0x7798
 *   extern char far g_backdrop_port[];                   SEG28:0x0000
 *   extern int g_stub_a;                                 
 *
 * The file's own MapWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1038_dfdc(void *p);
col_i16 rect_width_1038_dfbd(void *p);
void * get_p_22_1038_e355(void *w);
void * get_p_6_1038_e372(void *w);
void copy_rect_to_screen_1038_e38f(void *w, RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void port_set_clip(void *w, void *r);

extern void redraw_map_view(col_i16 n);
extern void wing_curtain_open_h_at(void *a, void *b, void *c, RECT *r, col_i16 x, col_i16 y, col_i16 n);
extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);
extern void port_apply_rect(void *w);







void MapWin_RevealHiddenTerrain(MapWin *w)
{
    RECT r;

    g_map_mode = 1;
    port_set_clip(w, &w->ox);
    SetRect(&r, 0, 0, rect_width_1038_dfbd(&w->ox), rect_height_1038_dfdc(&w->ox));
    redraw_map_view(g_reveal_all != 0 ? -1 : g_view_nation);
    wing_curtain_open_h_at(get_p_22_1038_e355(w), get_p_22_1038_e355(g_backdrop_port), get_p_6_1038_e372(((char *)((unsigned char *)(w) + 0x26))), &r, w->ox, w->oy, 8);
    CopyToPort(g_backdrop_port, w, &r, (RECT *)&w->ox);
    copy_rect_to_screen_1038_e38f(w, (RECT *)&w->ox);
    port_apply_rect(w);
    g_map_mode = 0;
}
