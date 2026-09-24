/* on_view_congress_floor_click -- shipped at 1058:6ff7.
 *
 * Ported by tools/port.py from matched/game/on_view_congress_floor_click.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_congress_nation;                        DGROUP:0x4e06
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void show_congress(col_i16 sel, col_i16 mode);

void on_view_congress_floor_click(void)
{
    show_congress(g_congress_nation, -1);
}
