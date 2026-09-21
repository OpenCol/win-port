/* MSSetWinTitle_of_1058_c555 -- shipped at 1058:c555.
 *
 * Ported by tools/port.py from matched/game/MSSetWinTitle_of_1058_c555.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSSetWinTitle(void *w, void *p);

void MSSetWinTitle_of_1058_c555(Obj *o, void *p)
{
    MSSetWinTitle((*((void * *)((unsigned char *)(o) + 0x6))), p);
}
