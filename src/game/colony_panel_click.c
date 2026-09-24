/* colony_panel_click -- shipped at 1010:508c.
 *
 * Ported by tools/port.py from matched/game/colony_panel_click.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_panel_click_busy;                      DGROUP:0x09fc
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void close_colony_popup(void);

extern void colony_goods_click(void *w, col_i16 x, col_i16 y);
extern void colony_building_click(void *w, col_i16 x, col_i16 y);
extern void colony_tiles_click(void *w, col_i16 x, col_i16 y);



void colony_panel_click(void *w, col_i16 panel, col_i16 x, col_i16 y)
{
    if (g_panel_click_busy != 0)
        return;
    g_panel_click_busy = 1;
    switch (panel) {
    case 5:
        colony_goods_click(w, x, y);
        break;
    case 2:
        colony_building_click(w, x, y);
        break;
    case 1:
        colony_tiles_click(w, x, y);
        break;
    default:
        close_colony_popup();
        break;
    }
    g_panel_click_busy = 0;
}
