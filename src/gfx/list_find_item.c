/* list_find_item -- shipped at 1068:a2ef.
 *
 * Ported by tools/port.py from matched/gfx/list_find_item.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * list_first(void *l);
void * list_next(void *l);

/* same object: 4-byte `push cs; call ` (rule 47) */


void * list_find_item(void *l, void *item)
{
    void *p;

    p = list_first(l);
    while (p != 0) {
        if (p == item)
            return p;
        p = list_next(l);
    }
    return 0;
}
