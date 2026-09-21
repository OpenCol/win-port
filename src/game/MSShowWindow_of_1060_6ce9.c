/* MSShowWindow_of_1060_6ce9 -- shipped at 1060:6ce9.
 *
 * Ported by tools/port.py from matched/game/MSShowWindow_of_1060_6ce9.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSShowWindow(void *w);

void MSShowWindow_of_1060_6ce9(Obj *o)
{
    MSShowWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
