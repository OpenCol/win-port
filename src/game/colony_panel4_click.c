/* colony_panel4_click -- shipped at 1010:5ee7.
 *
 * Ported by tools/port.py from matched/game/colony_panel4_click.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_colony_mode;              DGROUP:0x0c0f
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void colony_units_click(void *w, col_i16 x, col_i16 y);

void colony_panel4_click(void *w, col_i16 x, col_i16 y)
{
    switch (g_colony_mode) {
    case 0:
        break;
    case 1:
        colony_units_click(w, x, y);
        break;
    case 2:
        break;
    }
}
