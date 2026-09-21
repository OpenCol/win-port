/* MSShowWindow_of_1048_ecce -- shipped at 1048:ecce.
 *
 * Ported by tools/port.py from matched/game/MSShowWindow_of_1048_ecce.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSShowWindow(void *w);

void MSShowWindow_of_1048_ecce(Obj *o)
{
    MSShowWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
