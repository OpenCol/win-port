/* MSHideWindow_of_1060_691b -- shipped at 1060:691b.
 *
 * Ported by tools/port.py from matched/game/MSHideWindow_of_1060_691b.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSHideWindow(void *w);

void MSHideWindow_of_1060_691b(Obj *o)
{
    MSHideWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
