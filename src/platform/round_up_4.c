/* round_up_4 -- shipped at 1070:dc61.
 *
 * Ported by tools/port.py from matched/platform/round_up_4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int round_up_4(int n)
{
 return (n + 3) / 4 * 4;
}
