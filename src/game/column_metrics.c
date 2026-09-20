/* column_metrics -- shipped at 1010:beae.
 *
 * Ported by tools/port.py from matched/game/column_metrics.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void column_metrics(col_i16 n, col_i16 *px, col_i16 *py, col_i16 *pw, col_i16 *ph)
{
    *px = n * 0x18 + 0xfa;
    *py = 0x161;
    *pw = 10;
    *ph = 12;
}
