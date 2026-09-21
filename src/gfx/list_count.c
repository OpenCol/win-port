/* list_count -- shipped at 1068:a354.
 *
 * Ported by tools/port.py from matched/gfx/list_count.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * list_first(void *l);
void * list_next(void *l);

/* same object: 4-byte `push cs; call ` (rule 47) */


col_i16 list_count(void *l)
{
    col_i16 n = 0;
    void *p;

    p = list_first(l);
    while (p != 0) {
        n++;
        p = list_next(l);
    }
    return n;
}
