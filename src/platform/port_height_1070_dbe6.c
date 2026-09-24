/* port_height_1070_dbe6 -- shipped at 1070:dbe6.
 *
 * Ported by tools/port.py from matched/platform/port_height_1070_dbe6.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 port_height_1070_dd75(void *p);

/* same object: 4-byte `push cs; call ` */


col_i16 port_height_1070_dbe6(void *port)
{
    return port_height_1070_dd75(port);
}
