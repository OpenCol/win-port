/* trim_both -- shipped at 1040:6a12.
 *
 * Ported by tools/port.py from matched/game/trim_both.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void trim_right(char *s);

/* same object: 4-byte `push cs; call ` (rule 47) */



void trim_both(char *s)
{
    char *p;
    char buf[0x100];

    trim_right(s);
    p = s;
    while (*p != 0 && ((col_i16)*p == ' ' || (col_i16)*p == '\t'))
        p++;
    strcpy(buf, p);
    strcpy(s, buf);
}
