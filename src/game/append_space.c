/* append_space -- shipped at 1040:a192.
 *
 * Ported by tools/port.py from matched/game/append_space.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void append_space(char *buf)
{
    strcat(buf, " ");
}
