/* colony_panel_rclick -- shipped at 1010:4fd0.
 *
 * Ported by tools/port.py from matched/game/colony_panel_rclick.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void colony_goods_click(void *w, col_i16 x, col_i16 y);
extern void colony_cargo_click(void *w, col_i16 x, col_i16 y);
extern void colony_dock_click(void *w, col_i16 x, col_i16 y);
extern void colony_panel4_click(void *w, col_i16 x, col_i16 y);
extern void colony_row_click(void *w, col_i16 x, col_i16 y);
extern void colony_building_click(void *w, col_i16 x, col_i16 y);
extern void colony_tiles_click(void *w, col_i16 x, col_i16 y);

void colony_panel_rclick(void *w, col_i16 panel, col_i16 x, col_i16 y)
{
    switch (panel) {
    case 5:
        colony_goods_click(w, x, y);
        break;
    case 7:
        colony_cargo_click(w, x, y);
        break;
    case 8:
        colony_dock_click(w, x, y);
        break;
    case 4:
        colony_panel4_click(w, x, y);
        break;
    case 0:
        colony_row_click(w, x, y);
        break;
    case 2:
        colony_building_click(w, x, y);
        break;
    case 1:
        colony_tiles_click(w, x, y);
        break;
    default:
        break;
    }
}
