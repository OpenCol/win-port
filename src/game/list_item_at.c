/* list_item_at -- shipped at 1060:3338.
 *
 * Ported by tools/port.py from matched/game/list_item_at.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: count -> count_84e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * list_first_1060_68db(void *list);
void * list_next_1060_68fb(void *list);

/* same object: 4-byte `push cs; call ` (rule 47) */


void * list_item_at(Obj *o, col_i16 idx)
{
    col_i16 n = idx;
    void *p;

    if (o->count_84e <= n)
        return 0;
    p = list_first_1060_68db(((char *)((unsigned char *)(o) + 0x83e)));
    while (n > 0 && p != 0) {
        p = list_next_1060_68fb(((char *)((unsigned char *)(o) + 0x83e)));
        n--;
    }
    return p;
}
