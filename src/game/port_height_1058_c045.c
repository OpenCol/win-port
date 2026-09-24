/* port_height_1058_c045 -- shipped at 1058:c045.
 *
 * Ported by tools/port.py from matched/game/port_height_1058_c045.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 port_height_1058_bd14(void *p);

/* same object: 4-byte `push cs; call ` */


col_i16 port_height_1058_c045(void *win)
{
    return port_height_1058_bd14(win);
}
