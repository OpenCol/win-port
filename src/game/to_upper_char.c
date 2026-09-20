/* to_upper_char -- shipped at 1040:5c70.
 *
 * Ported by tools/port.py from matched/game/to_upper_char.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

unsigned char to_upper_char(unsigned char c)
{
 if (c >= 'a' && c <= 'z')
 c -= 0x20;
 return c;
}
