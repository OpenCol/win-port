/* port_height_1068_df70 -- shipped at 1068:df70.
 *
 * Ported by tools/port.py from matched/gfx/port_height_1068_df70.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 port_height_1068_e530(void *p);

/* same object: 4-byte `push cs; call ` */


col_i16 port_height_1068_df70(void *p)
{
    return port_height_1068_e530(p);
}
