/* MSSetFocus_of_1048_e0cd -- shipped at 1048:e0cd.
 *
 * Ported by tools/port.py from matched/game/MSSetFocus_of_1048_e0cd.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetFocus(void *w);

void MSSetFocus_of_1048_e0cd(Obj *o)
{
    MSSetFocus((*((void * *)((unsigned char *)(o) + 0x6))));
}
