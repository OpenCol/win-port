/* dispatch_region_click -- shipped at 1048:19a9.
 *
 * Ported by tools/port.py from matched/game/dispatch_region_click.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void set_current_nation(col_i16 n);
extern void europe_market_click(Win *w, col_i16 x, col_i16 y);
extern void europe_cargo_click(Win *w, col_i16 x, col_i16 y);
extern void europe_ships_click(Win *w, col_i16 x, col_i16 y);
extern void europe_voyage_click(Win *w, col_i16 x, col_i16 y);
extern void europe_dock_click(Win *w, col_i16 x, col_i16 y);

void dispatch_region_click(Win *w, col_i16 r, register col_i16 x, register col_i16 y)
{
    set_current_nation(w->nation);
    switch (r) {
    case 0:
        europe_market_click(w, x, y);
        break;
    case 10:
        europe_cargo_click(w, x, y);
        break;
    case 1:
        europe_ships_click(w, x, y);
        break;
    case 2:
    case 3:
        europe_voyage_click(w, x, y);
        break;
    case 4:
        europe_dock_click(w, x, y);
        break;
    default:
        break;
    }
}
