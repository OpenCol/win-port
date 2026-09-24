/* rand_scaled -- shipped at 1068:d840.
 *
 * Ported by tools/port.py from matched/gfx/rand_scaled.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 rand_rtl(void);

col_i16 rand_scaled(col_i16 n)
{
    return (col_i32)rand_rtl() * n / 0x8000L;
}
