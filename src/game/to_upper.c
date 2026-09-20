/* to_upper -- shipped at 1040:5c97.
 *
 * Ported by tools/port.py from matched/game/to_upper.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 to_upper(col_i16 c)
{
    if (c >= 'a' && c <= 'z')
        c -= 0x20;
    return c;
}
