/* byte_swap_1068_dc4c -- shipped at 1068:dc4c.
 *
 * Ported by tools/port.py from matched/gfx/byte_swap_1068_dc4c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_u16 byte_swap_1068_dc4c(col_u16 v)
{
    return ((v & 0xff) << 8) | ((v & 0xff00) >> 8);
}
