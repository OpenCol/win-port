/* GRPort_Init_wh -- shipped at 1068:34ed.
 *
 * Ported by tools/port.py from matched/gfx/GRPort_Init_wh.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern col_i16 GRPort_Init(void *p, RECT *r);

col_i16 GRPort_Init_wh(void *p, col_i16 w, col_i16 h)
{
    RECT r;

    SetRect(&r, 0, 0, w, h);
    return GRPort_Init(p, &r);
}
