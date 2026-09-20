/* round_up_4 -- shipped at 1070:dc61.
 *
 * Ported by tools/port.py from matched/platform/round_up_4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 round_up_4(col_i16 n)
{
    return (n + 3) / 4 * 4;
}
