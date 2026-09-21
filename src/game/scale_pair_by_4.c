/* scale_pair_by_4 -- shipped at 1038:af4a.
 *
 * Ported by tools/port.py from matched/game/scale_pair_by_4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void scale_pair_by_4(col_i16 a, col_i16 b)
{
    col_i16 x = a * 4;
    col_i16 y = b * 4;
}
