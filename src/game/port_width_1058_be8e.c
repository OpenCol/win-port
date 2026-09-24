/* port_width_1058_be8e -- shipped at 1058:be8e.
 *
 * Ported by tools/port.py from matched/game/port_width_1058_be8e.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 port_width_1058_bcfb(void *p);

/* same object: 4-byte `push cs; call ` */


col_i16 port_width_1058_be8e(void *p)
{
    return port_width_1058_bcfb(p);
}
