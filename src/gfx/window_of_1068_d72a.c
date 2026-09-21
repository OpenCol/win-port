/* window_of_1068_d72a -- shipped at 1068:d72a.
 *
 * Ported by tools/port.py from matched/gfx/window_of_1068_d72a.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * window_of_1068_d72a(Obj *o)
{
    return (*((void * *)((unsigned char *)(o) + 0x6)));
}
