/* list_clear -- shipped at 1068:a54d.
 *
 * Ported by tools/port.py from matched/gfx/list_clear.c.
 *
 * The file's own VList dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void list_first(VList *l);
void list_remove_current(VList *l);

/* same object: reached through the 4-byte `PUSH CS / CALL NEAR`, so it is DEFINED here (rule 47) */


/* same object: 4-byte `push cs; call ` */


void list_clear(VList *l)
{
    list_first(l);
    while (l->count != 0)
        list_remove_current(l);
}
