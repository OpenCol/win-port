/* colony_panel_dispatch -- shipped at 1010:4616.
 *
 * Ported by tools/port.py from matched/game/colony_panel_dispatch.c.
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void set_current_colony(col_i16 c);
extern void colony_panel4_click(ColChild *w, col_i16 x, col_i16 y);
extern void colony_row_click(ColChild *w, col_i16 x, col_i16 y);

void colony_panel_dispatch(ColChild *w, col_i16 panel, col_i16 x, col_i16 y)
{
    set_current_colony(w->colony);
    switch (panel) {
    case 5:
        break;
    case 7:
        break;
    case 8:
        break;
    case 4:
        colony_panel4_click(w, x, y);
        break;
    case 0:
        colony_row_click(w, x, y);
        break;
    case 2:
        break;
    case 1:
        break;
    default:
        break;
    }
}
