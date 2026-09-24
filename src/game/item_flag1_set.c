/* item_flag1_set -- shipped at 1050:0e0d.
 *
 * Ported by tools/port.py from matched/game/item_flag1_set.c.
 *
 * The file's own Node dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Node * list_find_by_key(void *o, col_i16 key);

/* same object: 4-byte `push cs; call ` (rule 47) */

void item_flag1_set(void *o, col_i16 key, col_i16 on)
{
    Node *n;
    n = list_find_by_key(o, key);
    if (n != 0) {
        if (on)
            (*((col_i16 *)((unsigned char *)(n) + 0x0))) |= 1;
        else
            (*((col_i16 *)((unsigned char *)(n) + 0x0))) &= ~1;
    }
}
