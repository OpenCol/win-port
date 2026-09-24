/* colony_draw_panel6 -- shipped at 1010:8190.
 *
 * Ported by tools/port.py from matched/game/colony_draw_panel6.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_colony_mode;              DGROUP:0x0c0f
 *   extern int g_stub_a;                                 
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void colony_draw_hammers(ColChild *w);
void colony_draw_production(ColChild *w);
void colony_draw_units(ColChild *w);
void control_hide_1010_e54d(void *c);
void copy_rect_to_screen_1010_e6a4(void *w, RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void set_current_colony(col_i16 c);
extern void colony_blit_cell_1010_bd3f(void *o, col_i16 x, col_i16 y, col_i16 w, col_i16 h);

extern void port_set_clip(void *w, RECT *r);
extern void port_apply_rect(void *w);







void colony_draw_panel6(ColChild *w, col_i16 redraw)
{
    RECT r;

    set_current_colony(w->colony);
    colony_blit_cell_1010_bd3f(w, 0x19e, 0x110, 0xba, 0x73);
    SetRect(&r, w->ox + 0x19e, w->oy + 0x110, w->ox + 0x258, w->oy + 0x183);
    port_set_clip(w, &r);
    switch (g_colony_mode) {
    case 0:
        colony_draw_production(w);
        control_hide_1010_e54d((char *)w + 0x950);
        control_hide_1010_e54d((char *)w + 0x972);
        break;
    case 1:
        colony_draw_units(w);
        control_hide_1010_e54d((char *)w + 0x950);
        control_hide_1010_e54d((char *)w + 0x972);
        break;
    case 2:
        colony_draw_hammers(w);
        break;
    }
    port_apply_rect(w);
    if (redraw != 0)
        copy_rect_to_screen_1010_e6a4(w, &r);
}
