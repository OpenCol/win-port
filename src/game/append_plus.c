/* append_plus -- shipped at 1040:a2dd.
 *
 * Ported by tools/port.py from matched/game/append_plus.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void append_plus(char *s)
{
    strcat(s, "+");
}
