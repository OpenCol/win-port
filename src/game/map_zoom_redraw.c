/* map_zoom_redraw -- shipped at 1038:a76e.
 *
 * Ported by tools/port.py from matched/game/map_zoom_redraw.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_backdrop_port[];                   SEG28:0x0000
 *   extern char far g_game_palette;                      SEG20:0x01ac
 *   extern char far g_map_overlay;                       SEG20:0x0de0
 *
 * The file's own MapWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1038_dfdc(RECT *r);
col_i16 rect_width_1038_dfbd(RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern char * current_window(void);

extern void grport_init_1068_4eae(void *port, RECT *r);
extern void port_select_if_changed(void *port, void *pal);
extern void scale_map_select(col_i16 a, col_i16 b);
extern void redraw_map_1040_11b7(col_i16 n);
extern void window_relayout(void *rec);





void map_zoom_redraw(void)
{
    MapWin *o;
    RECT rc;
    col_i16 zoom;
    col_i16 eight;
    char *w = current_window();

    o = (MapWin *)(w != 0 ? w - 0x26 : w);
    zoom = o->zoom + 8;
    eight = 8;
    SetRect(&rc, 0, 0, rect_width_1038_dfbd(&(*((RECT *)((unsigned char *)(o) + 0x76a)))), rect_height_1038_dfdc(&(*((RECT *)((unsigned char *)(o) + 0x76a)))));
    grport_init_1068_4eae(g_backdrop_port, &rc);
    port_select_if_changed(g_backdrop_port, &(*g_game_palette));
    scale_map_select(zoom, eight);
    redraw_map_1040_11b7(1);
    window_relayout(&(*g_map_overlay));
    scale_map_select(1, 1);
}
