/* window_of_1048_e6d0 -- shipped at 1048:e6d0.
 *
 * Ported by tools/port.py from matched/game/window_of_1048_e6d0.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * window_of_1048_e6d0(Obj *o)
{
    return (*((void * *)((unsigned char *)(o) + 0x6)));
}
