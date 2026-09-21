/* MSSetWinTitle_of_1010_edd2 -- shipped at 1010:edd2.
 *
 * Ported by tools/port.py from matched/game/MSSetWinTitle_of_1010_edd2.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetWinTitle(void *w, void *p);

void MSSetWinTitle_of_1010_edd2(Obj *o, void *p)
{
    MSSetWinTitle((*((void * *)((unsigned char *)(o) + 0x6))), p);
}
