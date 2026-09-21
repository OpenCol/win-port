/* list_current_value -- shipped at 1068:9e6d.
 *
 * Ported by tools/port.py from matched/gfx/list_current_value.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i32 deref_long(col_i32 *p);

typedef struct { char pad[4]; col_i32 *head; col_i32 *cur; } ListIter;
/* same object: 4-byte `push cs; call ` */


col_i32 list_current_value(ListIter *it)
{
    if (it->cur != 0)
        return deref_long(it->cur);
    else
        return 0L;
}
