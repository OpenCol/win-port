/* MSShowWindow_of_1030_ef7f -- shipped at 1030:ef7f.
 *
 * Ported by tools/port.py from matched/game/MSShowWindow_of_1030_ef7f.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSShowWindow(void *w);

void MSShowWindow_of_1030_ef7f(Obj *o)
{
    MSShowWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
