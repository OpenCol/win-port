/* colony_draw_buildings -- shipped at 1010:8dec.
 *
 * Ported by tools/port.py from matched/game/colony_draw_buildings.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern signed char far g_site_building[];            SEG21:0x01dc
 *   extern Pos far g_site_pos[];                         SEG21:0x006a
 *   extern unsigned char far g_site_kind[];              SEG21:0x0000
 *   extern int g_stub_a;                                 
 *
 * The file's own Pos, ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_rect_to_screen_1010_e6a4(void *w, RECT *r);
void draw_empty_site(void *w, col_i16 x, col_i16 y, col_i16 kind);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void set_current_colony(col_i16 c);
extern void colony_blit_cell_1010_bd3f(void *o, col_i16 x, col_i16 y, col_i16 w, col_i16 h);
extern void colony_draw_site(void *w, col_i16 b, col_i16 x, col_i16 y, col_i16 kind);





void colony_draw_buildings(ColChild *w, col_i16 redraw)
{
    register col_i16 i;
    register col_i16 building;
    col_i16 x;
    col_i16 y;
    col_i16 kind;
    RECT r;

    set_current_colony(w->colony);
    colony_blit_cell_1010_bd3f(w, 0, 0, 0x190, 0x109);
    for (i = 0; i < 15; i++) {
        building = g_site_building[i];
        x = g_site_pos[i].x;
        y = g_site_pos[i].y;
        kind = g_site_kind[i];
        if (building >= 0)
            colony_draw_site(w, building, x, y, kind);
        else
            draw_empty_site(w, x, y, kind);
    }
    if (redraw != 0) {
        SetRect(&r, w->ox, w->oy, w->ox + 0x190, w->oy + 0x109);
        copy_rect_to_screen_1010_e6a4(w, &r);
    }
}
