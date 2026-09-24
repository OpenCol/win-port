/* list_find_by_field6 -- shipped at 1050:0da7.
 *
 * Ported by tools/port.py from matched/game/list_find_by_field6.c.
 *
 * The file's own Item dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x386]; Item *items; } Dialog;

Item * list_find_by_field6(Dialog *d, col_i16 key)
{
    Item *found = 0;
    Item *p = d->items;

    while (p != 0 && found == 0) {
        if ((*((col_i16 *)((unsigned char *)(p) + 0x6))) == key)
            found = p;
        p = (*((void * *)((unsigned char *)(p) + 0x10)));
    }
    return found;
}
