/* swap_ints -- shipped at 1040:7b16.
 *
 * Ported by tools/port.py from matched/game/swap_ints.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void swap_ints(col_i16 *a, col_i16 *b)
{
    col_i16 t = *a;

    *a = *b;
    *b = t;
}
