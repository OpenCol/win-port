/* append_rparen -- shipped at 1040:a27d.
 *
 * Ported by tools/port.py from matched/game/append_rparen.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void append_rparen(char *s)
{
    strcat(s, ")");
}
