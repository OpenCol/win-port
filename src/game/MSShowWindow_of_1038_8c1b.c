/* MSShowWindow_of_1038_8c1b -- shipped at 1038:8c1b.
 *
 * Ported by tools/port.py from matched/game/MSShowWindow_of_1038_8c1b.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSShowWindow(void *w);

void MSShowWindow_of_1038_8c1b(Obj *o)
{
    MSShowWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
