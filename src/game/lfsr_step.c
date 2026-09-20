/* lfsr_step -- shipped at 1040:5e57.
 *
 * Ported by tools/port.py from matched/game/lfsr_step.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 lfsr_step(col_i16 *p, col_i16 k)
{
    col_i16 v = *p;

    if (v & 1) {
        v >>= 1;
        v ^= k;
    } else {
        v >>= 1;
    }
    return v;
}
