/* list_item_at_count -- shipped at 1060:2af7.
 *
 * Ported by tools/port.py from matched/game/list_item_at_count.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: count -> count_84c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * list_first_1060_68db(void *list);
void * list_next_1060_68fb(void *list);

/* same object: 4-byte `push cs; call ` (rule 47) */


void * list_item_at_count(Obj *o)
{
    void *r = 0;
    col_i16 i;

    if (o->count_84c >= 0) {
        r = list_first_1060_68db(((char *)((unsigned char *)(o) + 0x83e)));
        for (i = 0; o->count_84c > i; i++)
            r = list_next_1060_68fb(((char *)((unsigned char *)(o) + 0x83e)));
    }
    return r;
}
