/* controls_invalidate -- shipped at 1060:9afa.
 *
 * Ported by tools/port.py from matched/game/controls_invalidate.c.
 *
 * The file's own Ctl, Win dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: hwnd -> hbutton
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void invalidate_hwnd(col_i16 h);

void controls_invalidate(Win *w)
{
    Ctl *c;

    for (c = (*((Ctl * *)((unsigned char *)(w) + 0x8c))); c != 0; c = (*((void * *)((unsigned char *)(c) + 0x12))))
        invalidate_hwnd(c->hbutton);
}
