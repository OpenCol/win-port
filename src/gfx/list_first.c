/* list_first -- shipped at 1068:9dde.
 *
 * Ported by tools/port.py from matched/gfx/list_first.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * deref_long(void *item);

typedef struct { void *head; col_i32 pad; void *cur; } List;
/* same object: 4-byte `push cs; call ` (rule 47) */

void * list_first(List *l)
{
    if (l->head != 0) {
        l->cur = l->head;
        return deref_long(l->head);
    } else
        return 0;
}
