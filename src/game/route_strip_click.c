/* route_strip_click -- shipped at 1058:87dd.
 *
 * Ported by tools/port.py from matched/game/route_strip_click.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1058_be68(col_i16 a, col_i16 b);
void select_cargo_to_transfer(col_i16 side, col_i16 x, col_i16 y);

extern void set_route(col_i16 slot);
extern void route_set_stop(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

void route_strip_click(register col_i16 x, col_i16 y)
{
    register col_i16 col;
    col_i16 route;

    route = (y - 0x69) / 0x4b;
    route = min_int_1058_be68(route, 3);
    set_route(route);
    col = 0;
    if (x >= 0xd7)
        col = 1;
    if (x >= 0x19f)
        col = 2;
    switch (col) {
    case 0:
        route_set_stop();
        break;
    case 1:
    case 2:
        select_cargo_to_transfer(col == 2, x, y);
        break;
    }
}
