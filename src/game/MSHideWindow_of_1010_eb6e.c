/* MSHideWindow_of_1010_eb6e -- shipped at 1010:eb6e.
 *
 * Ported by tools/port.py from matched/game/MSHideWindow_of_1010_eb6e.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSHideWindow(void *w);

void MSHideWindow_of_1010_eb6e(Obj *o)
{
    MSHideWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
