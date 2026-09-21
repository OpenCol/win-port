/* MSHideWindow_of_1068_d995 -- shipped at 1068:d995.
 *
 * Ported by tools/port.py from matched/gfx/MSHideWindow_of_1068_d995.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSHideWindow(void *w);

void MSHideWindow_of_1068_d995(Obj *o)
{
    MSHideWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
