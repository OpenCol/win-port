/* append_lbrace -- shipped at 1040:a29d.
 *
 * Ported by tools/port.py from matched/game/append_lbrace.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void append_lbrace(char *s)
{
    strcat(s, "{");
}
