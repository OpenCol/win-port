/* list_find_by_id -- shipped at 1070:8f5a.
 *
 * Ported by tools/port.py from matched/platform/list_find_by_id.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_10_1070_9681(void *item);
void * ctl_next_1070_a09a(void *item);

/* same object: 4-byte `push cs; call ` (rule 47) */


void * list_find_by_id(col_i16 id, void *p)
{
    col_i16 n = id;
    void *q = p;

    while (q != 0) {
        if (get_w_10_1070_9681(q) == n)
            return q;
        q = ctl_next_1070_a09a(q);
    }
    return 0;
}
