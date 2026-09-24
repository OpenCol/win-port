/* port_width_1010_e3a8 -- shipped at 1010:e3a8.
 *
 * Ported by tools/port.py from matched/game/port_width_1010_e3a8.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 port_width_1010_ebd4(void *p);

/* same object: 4-byte `push cs; call ` */


col_i16 port_width_1010_e3a8(void *p)
{
    return port_width_1010_ebd4(p);
}
