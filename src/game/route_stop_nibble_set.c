/* route_stop_nibble_set -- shipped at 1028:c568.
 *
 * Ported by tools/port.py from matched/game/route_stop_nibble_set.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * route_stop_nibble_ptr(col_i16 i);

/* same object: 4-byte `push cs; call ` */


void route_stop_nibble_set(register col_i16 i, col_i16 v)
{
    char *p;
    unsigned char b;

    p = route_stop_nibble_ptr(i);
    b = *p;
    if (i & 1) {
        b = b & 0xf;
        b |= v << 4;
    } else {
        b = b & 0xf0;
        b |= v & 0xf;
    }
    *p = b;
}
