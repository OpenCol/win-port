/* port_draw_at -- shipped at 1068:b144.
 *
 * Ported by tools/port.py from matched/gfx/port_draw_at.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void wipe_rect(void *port, Obj *o, RECT *r, col_i16 x, col_i16 y,
                              col_i16 a, col_i16 b);

void port_draw_at(Obj *o)
{
    wipe_rect(((char *)((unsigned char *)(o) + 0xc4)), o, &(*((RECT *)((unsigned char *)(o) + 0xee))), (*((RECT *)((unsigned char *)(o) + 0xee))).left, (*((RECT *)((unsigned char *)(o) + 0xee))).top, -1, 10);
}
