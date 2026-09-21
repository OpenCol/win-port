/* win_hide_1008_d0bc -- shipped at 1008:d0bc.
 *
 * Ported by tools/port.py from matched/game/win_hide_1008_d0bc.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSHideWindow(void *w);

void win_hide_1008_d0bc(Obj *o)
{
    MSHideWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
