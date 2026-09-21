/* MSSetWinTitle_of_1038_e517 -- shipped at 1038:e517.
 *
 * Ported by tools/port.py from matched/game/MSSetWinTitle_of_1038_e517.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetWinTitle(void *w, void *p);

void MSSetWinTitle_of_1038_e517(Obj *o, void *p)
{
    MSSetWinTitle((*((void * *)((unsigned char *)(o) + 0x6))), p);
}
