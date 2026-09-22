/* route_stop_nibble_get -- shipped at 1028:c515.
 *
 * Ported by tools/port.py from matched/game/route_stop_nibble_get.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * route_stop_nibble_ptr(col_i16 i);

/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 route_stop_nibble_get(col_i16 i)
{
    unsigned char v;

    v = *route_stop_nibble_ptr(i);
    if (i & 1)
        v = v >> 4;
    else
        v = v & 0xf;
    return v;
}
