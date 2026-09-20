/* quarter_of_100 -- shipped at 1038:0102.
 *
 * Ported by tools/port.py from matched/game/quarter_of_100.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 quarter_of_100(col_i16 v)
{
    col_i16 r;

    if (v < 25)
        r = 0;
    else if (v < 50)
        r = 1;
    else if (v < 75)
        r = 2;
    else
        r = 3;
    return r;
}
