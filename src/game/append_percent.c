/* append_percent -- shipped at 1040:a23d.
 *
 * Ported by tools/port.py from matched/game/append_percent.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void append_percent(char *s)
{
    strcat(s, "%");
}
