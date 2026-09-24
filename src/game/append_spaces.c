/* append_spaces -- shipped at 1040:a1b2.
 *
 * Ported by tools/port.py from matched/game/append_spaces.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void append_space(char *buf);

/* same object: 4-byte `push cs; call ` (rule 47) */

void append_spaces(char *buf, col_i16 n)
{
    col_i16 i;

    for (i = 0; i < n; i++)
        append_space(buf);
}
