/* rand_scaled_1070_ddfe -- shipped at 1070:ddfe.
 *
 * Ported by tools/port.py from matched/platform/rand_scaled_1070_ddfe.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 rand_rtl(void);

col_i16 rand_scaled_1070_ddfe(col_i16 n)
{
    return (col_i32)rand_rtl() * n / 0x8000L;
}
