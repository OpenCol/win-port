/* byte_swap_1068_dc4c -- shipped at 1068:dc4c.
 *
 * Ported by tools/port.py from matched/gfx/byte_swap_1068_dc4c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

unsigned byte_swap_1068_dc4c(unsigned v)
{
 return ((v & 0xff) << 8) | ((v & 0xff00) >> 8);
}
