/* list_find_by_key -- shipped at 1050:0d41.
 *
 * Ported by tools/port.py from matched/game/list_find_by_key.c.
 *
 * The file's own Node, Owner dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

Node * list_find_by_key(Owner *o, col_i16 key)
{
    Node *found = 0;
    Node *p = (*((Node * *)((unsigned char *)(o) + 0x36e)));
    while (p != 0 && found == 0) {
        if ((*((col_i16 *)((unsigned char *)(p) + 0x2))) == key)
            found = p;
        p = (*((void * *)((unsigned char *)(p) + 0xa)));
    }
    return found;
}
