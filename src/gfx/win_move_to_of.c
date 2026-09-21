/* win_move_to_of -- shipped at 1068:df2a.
 *
 * Ported by tools/port.py from matched/gfx/win_move_to_of.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void win_move_to_1078_1ff8(void *w, void *p);

void win_move_to_of(Obj *o, void *p)
{
    win_move_to_1078_1ff8((*((void * *)((unsigned char *)(o) + 0x6))), p);
}
