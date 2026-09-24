/* list_delete_items -- shipped at 1060:326b.
 *
 * Ported by tools/port.py from matched/game/list_delete_items.c.
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

extern void __bdele_qnv_rtl(void *p);
extern void list_remove_current(void *list);

/* same object: 4-byte `push cs; call ` */


void list_delete_items(Obj *o)
{
    void *p;

    p = list_first_1060_68db(((char *)((unsigned char *)(o) + 0x83e)));
    while (p != 0) {
        __bdele_qnv_rtl(p);
        list_remove_current(((char *)((unsigned char *)(o) + 0x83e)));
        p = list_first_1060_68db(((char *)((unsigned char *)(o) + 0x83e)));
    }
    o->count_84e = 0;
}
