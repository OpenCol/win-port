/* MSShowWindow_of_1068_e66f -- shipped at 1068:e66f.
 *
 * Ported by tools/port.py from matched/gfx/MSShowWindow_of_1068_e66f.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSShowWindow(void *w);

void MSShowWindow_of_1068_e66f(Obj *o)
{
    MSShowWindow((*((void * *)((unsigned char *)(o) + 0x6))));
}
