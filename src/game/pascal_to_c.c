/* pascal_to_c -- shipped at 1040:77ca.
 *
 * Ported by tools/port.py from matched/game/pascal_to_c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void pascal_to_c(char *s)
{
    unsigned char n = s[0];
    col_i16 i;

    for (i = 0; n > i; i++)
        s[i] = s[i + 1];
    s[i] = 0;
}
