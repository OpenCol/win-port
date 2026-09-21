/* MSShowWindow_of_1040_b3ca -- shipped at 1040:b3ca.
 *
 * Ported by tools/port.py from matched/game/MSShowWindow_of_1040_b3ca.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSShowWindow(void *w);

void MSShowWindow_of_1040_b3ca(Obj *o)
{
    MSShowWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
