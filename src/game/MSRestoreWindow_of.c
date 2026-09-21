/* MSRestoreWindow_of -- shipped at 1060:6809.
 *
 * Ported by tools/port.py from matched/game/MSRestoreWindow_of.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSRestoreWindow(void *w);

void MSRestoreWindow_of(Obj *o)
{
    MSRestoreWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
