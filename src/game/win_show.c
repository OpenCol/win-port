/* win_show -- shipped at 1008:d0dd.
 *
 * Ported by tools/port.py from matched/game/win_show.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSShowWindow(void *w);

void win_show(Obj *o)
{
    MSShowWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
