/* MSMaximizeWindow_of -- shipped at 1048:e1b5.
 *
 * Ported by tools/port.py from matched/game/MSMaximizeWindow_of.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSMaximizeWindow(void *w);

void MSMaximizeWindow_of(Obj *o)
{
    MSMaximizeWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
