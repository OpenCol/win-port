/* swap_long -- shipped at 1068:976d.
 *
 * Ported by tools/port.py from matched/gfx/swap_long.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_u32 swap_long(col_u32 v)
{
    return ((v & 0xffL) << 24) | ((v & 0xff00L) << 8)
         | ((v & 0xff0000L) >> 8) | ((v & 0xff000000L) >> 24);
}
