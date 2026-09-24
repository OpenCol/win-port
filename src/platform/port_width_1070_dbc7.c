/* port_width_1070_dbc7 -- shipped at 1070:dbc7.
 *
 * Ported by tools/port.py from matched/platform/port_width_1070_dbc7.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 port_width_1070_dd5c(void *p);

/* same object: 4-byte `push cs; call ` */


col_i16 port_width_1070_dbc7(void *port)
{
    return port_width_1070_dd5c(port);
}
