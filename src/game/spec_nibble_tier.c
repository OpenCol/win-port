/* spec_nibble_tier -- shipped at 1018:4d2b.
 *
 * Ported by tools/port.py from matched/game/spec_nibble_tier.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 colonist_spec_nibble(col_i16 c);

col_i16 spec_nibble_tier(col_i16 c)
{
    register col_i16 r;
    register col_i16 v;

    v = colonist_spec_nibble(c);
    if (v >= 15)
        r = 3;
    else if (v >= 8)
        r = 2;
    else if (v >= 4)
        r = 1;
    else
        r = 0;
    return r;
}
