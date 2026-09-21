/* MSSetFocus_of_1058_c508 -- shipped at 1058:c508.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1058_c508.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetFocus(void *w);

void MSSetFocus_of_1058_c508(Obj *o)
{
    MSSetFocus((*((void * *)((unsigned char *)(o) + 0x6))));
}
