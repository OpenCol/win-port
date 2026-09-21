/* MSHideWindow_of_1048_d4d5 -- shipped at 1048:d4d5.
 *
 * Ported by tools/port.py from matched/game/MSHideWindow_of_1048_d4d5.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSHideWindow(void *w);

void MSHideWindow_of_1048_d4d5(Obj *o)
{
    MSHideWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
