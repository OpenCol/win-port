/* opening_scroll_step -- shipped at 1010:0cee.
 *
 * Ported by tools/port.py from matched/game/opening_scroll_step.c.
 *
 * The file's own Opening dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void CopyToScreen(void *w, void *port, RECT *src, RECT *dst);

void opening_scroll_step(Opening *w)
{
    RECT r1;
    RECT r2;
    register col_i16 d;

    if (w->state < 3) {
        if (w->scroll <= 0x500) {
            d = 0x500 - w->scroll;
            SetRect(&r1, d, 0x38, d + 0x280, 0x17b);
        } else {
            SetRect(&r1, 0, 0x38, 0x280, 0x17b);
        }
        SetRect(&r2, 0, 0x38, 0x280, 0x17b);
        CopyToScreen((char *)w + 0xc4, (char *)w + 0x26, &r1, &r2);
    }
    w->scroll += 2;
}
