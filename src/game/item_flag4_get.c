/* item_flag4_get -- shipped at 1050:0f15.
 *
 * Ported by tools/port.py from matched/game/item_flag4_get.c.
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

col_i16 item_flag4_get(void *o, col_i16 key)
{
    Node *n;
    col_i16 r = 0;
    n = list_find_by_key(o, key);
    if (n != 0)
        r = ((*((col_i16 *)((unsigned char *)(n) + 0x0))) & 4) != 0;
    return r;
}
