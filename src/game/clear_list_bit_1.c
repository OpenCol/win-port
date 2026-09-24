/* clear_list_bit_1 -- shipped at 1050:0e9b.
 *
 * Ported by tools/port.py from matched/game/clear_list_bit_1.c.
 *
 * The file's own Item, Owner dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void clear_list_bit_1(Owner *o)
{
    Item *p;

    for (p = (*((Item * *)((unsigned char *)(o) + 0x36e))); p != 0; p = (*((void * *)((unsigned char *)(p) + 0xa))))
        (*((col_i16 *)((unsigned char *)(p) + 0x0))) &= ~1;
}
