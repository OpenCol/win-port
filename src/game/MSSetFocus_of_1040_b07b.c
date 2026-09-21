/* MSSetFocus_of_1040_b07b -- shipped at 1040:b07b.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1040_b07b.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetFocus(void *w);

void MSSetFocus_of_1040_b07b(Obj *o)
{
    MSSetFocus((*((void * *)((unsigned char *)(o) + 0x6))));
}
