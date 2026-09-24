/* atol -- shipped at 1050:8de2.
 *
 * Ported by tools/port.py from matched/game/atol.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 ATOL(void *p);

col_i32 atol(void *p)
{
    return ATOL(p);
}
