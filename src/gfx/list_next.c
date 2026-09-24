/* list_next -- shipped at 1068:9ea6.
 *
 * Ported by tools/port.py from matched/gfx/list_next.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * deref_long(void *item);
void * vlist_item_next(void *item);

typedef struct { void *head; col_i32 pad; void *cur; } List;

/* same object: 4-byte `push cs; call ` (rule 47) */

void * list_next(List *l)
{
    if (l->cur != 0) {
        l->cur = vlist_item_next(l->cur);
        if (l->cur == 0)
            return 0;
        else
            return deref_long(l->cur);
    } else
        return 0;
}
